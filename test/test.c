#include <stdio.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <unity.h>
#include <pico/cyw43_arch.h>
#include "unity_config.h"
#include "stuff.h"

void setUp(void) {}

void tearDown(void) {}

void test_change_case()
{
    TEST_ASSERT_EQUAL('A', switch_case('a'));
    TEST_ASSERT_EQUAL('Z', switch_case('z'));
    TEST_ASSERT_EQUAL('a', switch_case('A'));
    TEST_ASSERT_EQUAL('z', switch_case('Z'));
    TEST_ASSERT_EQUAL('g', switch_case('G'));
    TEST_ASSERT_EQUAL('0', switch_case('0'));
    TEST_ASSERT_EQUAL('!', switch_case('!'));
    TEST_ASSERT_EQUAL('\n', switch_case('\n'));
    TEST_ASSERT_EQUAL('\a', switch_case('\a'));
    TEST_ASSERT_EQUAL('A' - 1, switch_case('A' - 1));
    TEST_ASSERT_EQUAL('Z' + 1, switch_case('Z' + 1));
    TEST_ASSERT_EQUAL('a' - 1, switch_case('a' - 1));
    TEST_ASSERT_EQUAL('z' + 1, switch_case('z' + 1));
}

void test_blinking(void)
{
    int count = 0;
    bool on = 0;
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
    for (int i = 0; i < 200; i += 1) {
        int curr_count = count;
        bool next_on = do_blink(on, &count);
        int gpio = cyw43_arch_gpio_get(CYW43_WL_GPIO_LED_PIN);
        TEST_ASSERT_EQUAL_MESSAGE(gpio, on, "GPIO should be set to the state of 'on' variable.");
        TEST_ASSERT_EQUAL_MESSAGE(count, curr_count +1, "Always increment the count");
        if ((i+1) % 11) {
            TEST_ASSERT_NOT_EQUAL_MESSAGE(on, next_on, "Should toggle when count is not a multiple of 11.");
        } else {
            TEST_ASSERT_EQUAL_MESSAGE(on, next_on, "Should not toggle otherwise.");
        }
        on = next_on;
    }
}

int main (void)
{
    stdio_init_all();
    hard_assert(cyw43_arch_init() == PICO_OK);
    while(1) {
	sleep_ms(5000); // Give time for TTY to attach.
	printf("Start tests\n");
	UNITY_BEGIN();
	RUN_TEST(test_change_case);
	RUN_TEST(test_blinking);
	UNITY_END();
    }
}
