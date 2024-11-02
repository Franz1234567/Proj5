#include <unity.h>
#include <Arduino.h>

#include "global.h"

int max_speed = 2800;

void test_normal_PI_controller(){

    PI_Controller pi_controller;
    pi_controller.init(0.05, 0.1, 0.1);
    double u = pi_controller.update(10, 5);
    TEST_ASSERT_EQUAL_FLOAT(0.5, u);

    u = pi_controller.update(10, 5);
    TEST_ASSERT_EQUAL_FLOAT(0.75, u);

    u = pi_controller.update(10, 5);
    TEST_ASSERT_EQUAL_FLOAT(1, u);
}

void test_saturate_PI_controller(){
    PI_Controller pi_controller;  // Set max_output to 2800
    pi_controller.init(0.05, 0.1, 0.1);

    double u = pi_controller.update(2000, 0); 
    double current_sum_error = pi_controller.get_sum_error();
    TEST_ASSERT_EQUAL_FLOAT(200, current_sum_error);  // Access sum_error through getter
    
    u = pi_controller.update(30000, 0); // saturates at max_output
    current_sum_error = pi_controller.get_sum_error();
    TEST_ASSERT_EQUAL_FLOAT(200, current_sum_error);  // sum_error should not change after saturation
}

void setup(){
    delay(2000);
    UNITY_BEGIN();
    RUN_TEST(test_normal_PI_controller);
    RUN_TEST(test_saturate_PI_controller);

    UNITY_END();
}

void loop(){
    delay(500);
}