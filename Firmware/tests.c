#include "config.h"
#if TEST_MODE != 0
#include "speed_controller.h"
#include <stdint.h>
#include <stdio.h>
#include "tests.h"
#include "encoder.h"
#include "motor.h"


void test_spi_callback(uint8_t *data){
    printf("Received: ");
        for (int i = 0; i < 8; i++) {
            printf("%02X ", data[i]);
        }
        printf("\n");
}

void do_tests(){
    #if TEST_MODE == 0
    #elif TEST_MODE == 1
        motor_init();
        stdio_usb_init();
        encoder_init();
        while (true){
            motor_set_pwm(MOTOR_RIGHT, 65000);
            motor_set_pwm(MOTOR_LEFT, -65000);
            sleep_ms(2000);
            motor_set_pwm(MOTOR_RIGHT, -65000);
            motor_set_pwm(MOTOR_LEFT, 65000);
            sleep_ms(2000);
        }
    #elif TEST_MODE == 2
        encoder_init();
        extern substep_state_t encoders_states[ENCODER_COUNT];
        uint last_position = 0;
        int last_speed = 0;
        uint last_raw_step = 0;
        while (true){
            // read the PIO and update the state data
            substep_state_t* state = &encoders_states[ENCODER_RIGHT];
            substep_update(state);

            if (last_position != state->position || last_speed != state->speed || last_raw_step != state->raw_step) {
                // print out the result
                printf("pos: %-10d  speed: %-10d  raw_steps: %-10d\n", state->position, state->speed, state->raw_step);
                last_position = state->position;
                last_speed = state->speed;
                last_raw_step = state->raw_step;
            }

            // run at roughly 100Hz
            sleep_ms(10);
        }
    #elif TEST_MODE == 3
        spi_init(test_spi_callback);
        while (true){
        }

    #elif TEST_MODE == 4
        motor_init();
        stdio_usb_init();
        encoder_init();
        extern substep_state_t encoders_states[ENCODER_COUNT];
        int16_t control[2] = {0, 0};
        speed_controller_init(PID_KP, PID_KI, PID_KD);
        absolute_time_t last_time = get_absolute_time();
        int wrum_time = last_time;
        while (true) {
            absolute_time_t current_time = get_absolute_time();
            int64_t delta_us = absolute_time_diff_us(last_time, current_time);
            last_time = current_time;
            control_speed(control, delta_us / 1000.0);
            sleep_ms(10);
        }
    #else
        #error "End of tests"
    #endif
}
#endif
