/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "pico/stdlib.h"
#include "pico/multicore.h"

#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

// Whether to flash the led
#ifndef USE_LED
#define USE_LED 1
#endif

// Whether to busy wait in the led thread
#ifndef LED_BUSY_WAIT
#define LED_BUSY_WAIT 1
#endif

// Delay between led blinking
#define LED_DELAY_MS 2000

// Priorities of our threads - higher numbers are higher priority
#define MAIN_TASK_PRIORITY      ( tskIDLE_PRIORITY + 2UL )
#define BLINK_TASK_PRIORITY     ( tskIDLE_PRIORITY + 1UL )
#define WORKER_TASK_PRIORITY    ( tskIDLE_PRIORITY + 4UL )

// Stack sizes of our threads in words (4 bytes)
#define MAIN_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define BLINK_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define WORKER_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

#include "pico/async_context_freertos.h"
static async_context_freertos_t async_context_instance;

// Create an async context
static async_context_t *example_async_context(void) {
    async_context_freertos_config_t config = async_context_freertos_default_config();
    config.task_priority = WORKER_TASK_PRIORITY; // defaults to ASYNC_CONTEXT_DEFAULT_FREERTOS_TASK_PRIORITY
    config.task_stack_size = WORKER_TASK_STACK_SIZE; // defaults to ASYNC_CONTEXT_DEFAULT_FREERTOS_TASK_STACK_SIZE
    if (!async_context_freertos_init(&async_context_instance, &config))
        return NULL;
    return &async_context_instance.core;
}

#if USE_LED
// Turn led on or off
static void pico_set_led(bool led_on) {
#if defined PICO_DEFAULT_LED_PIN
    gpio_put(PICO_DEFAULT_LED_PIN, led_on);
#elif defined(CYW43_WL_GPIO_LED_PIN)
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
#endif
}

// Initialise led
static void pico_init_led(void) {
#if defined PICO_DEFAULT_LED_PIN
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
#elif defined(CYW43_WL_GPIO_LED_PIN)
    hard_assert(cyw43_arch_init() == PICO_OK);
    pico_set_led(false); // make sure cyw43 is started
#endif
}

void blink_task(__unused void *params) {
    bool on = false;
    printf("blink_task starts\n");
    pico_init_led();
    while (true) {
        pico_set_led(on);
        on = !on;
        sleep_ms(LED_DELAY_MS);
    }
}
#endif // USE_LED

// async workers run in their own thread when using async_context_freertos_t with priority WORKER_TASK_PRIORITY
static void do_work(async_context_t *context, async_at_time_worker_t *worker) {
    async_context_add_at_time_worker_in_ms(context, worker, 10000);
    static uint32_t count = 0;
    printf("Hello from worker count=%u\n", count++);
}
async_at_time_worker_t worker_timeout = { .do_work = do_work };

void main_task(__unused void *params) {
    async_context_t *context = example_async_context();
    // start the worker running
    async_context_add_at_time_worker_in_ms(context, &worker_timeout, 0);
#if USE_LED
    // start the led blinking
    xTaskCreate(blink_task, "BlinkThread", BLINK_TASK_STACK_SIZE, NULL, BLINK_TASK_PRIORITY, NULL);
#endif
    int count = 0;
    while(true) {
        printf("Hello from main task count=%u\n", count++);
        vTaskDelay(3000);
    }
    async_context_deinit(context);
}

void vLaunch( void) {
    TaskHandle_t task;
    xTaskCreate(main_task, "MainThread", MAIN_TASK_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, &task);


    /* Start the tasks and timer running. */
    vTaskStartScheduler();
}

int main( void )
{
    stdio_init_all();

    /* Configure the hardware ready to run the demo. */
    const char *rtos_name;
    rtos_name = "FreeRTOS";
    vLaunch();
    return 0;
}
