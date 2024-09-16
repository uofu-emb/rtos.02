#include <stdio.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <unity.h>
#include "unity_config.h"
#include "stuff.h"

void setUp(void) {}

void tearDown(void) {}

void test_change_case()
{
    TEST_ASSERT_EQUAL('A', switch_case('a'));
    TEST_ASSERT_EQUAL('Z', switch_case('z'));
    TEST_ASSERT_EQUAL('a', switch_case('Z'));
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
    for (int t = 0; t < 2; t++) {
        for (int i = 0; i < 3000; i += 37) {
            int j = i;
            bool o = do_blink(t, &j);
            TEST_ASSERT_EQUAL_MESSAGE(j, i+1, "Always increment the count");
            if ((i+1) % 11) {
                TEST_ASSERT_EQUAL_MESSAGE(o, t, "Should only toggle when count is not a multiple of 11.");
            } else {
                TEST_ASSERT_NOT_EQUAL_MESSAGE(o, t, "Should toggle otherwise.");
            }
        }
    }
}

int main (void)
{
    stdio_init_all();
    sleep_ms(5000); // Give time for TTY to attach.
    printf("Start tests\n");
    UNITY_BEGIN();
    RUN_TEST(test_change_case);
    RUN_TEST(test_blinking);
    sleep_ms(5000);
    return UNITY_END();
}
