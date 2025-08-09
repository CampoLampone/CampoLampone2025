#include "control_speed.h"
#include <stdio.h>
#include "pico/stdio_usb.h"
#include <pico/time.h>
#include "motor.h"
#include "encoder.h"
#include "config.h"

enum command {
    COMMAND_SPEED,
    COMMAND_POSITION,
    COMMAND_STOP,
    COMMAND_COAST
};

uint16_t control[2] = {0, 0};
substep_state_t encoders_states[ENCODER_COUNT];

int main() {
    motor_init();
    stdio_usb_init();
    encoder_init();
#if TEST_MODE == 0
    absolute_time_t last_time = get_absolute_time();
    int wrum_time = last_time;
    while (true) {
        absolute_time_t current_time = get_absolute_time();
        int64_t delta_us = absolute_time_diff_us(last_time, current_time);
        last_time = current_time;
        switch (COMMAND_SPEED) {
            case (COMMAND_SPEED):
            control_speed(control, delta_us / 1000.0);
            break;
        }
        sleep_ms(10);

        if (current_time - wrum_time > 5e6) {
            wrum_time = current_time;
            if (control[0] == 0) {
                control[0] = 60;
                control[1] = 60;
            } else {
                control[0] = 0;
                control[1] = 0;
            }
        }
    }
#elif TEST_MODE == 1
    while (true){
        motor_set_pwm(MOTOR_RIGHT, 65000);
        motor_set_pwm(MOTOR_LEFT, -65000);
        sleep_ms(2000);
        motor_set_pwm(MOTOR_RIGHT, -65000);
        motor_set_pwm(MOTOR_LEFT, 65000);
        sleep_ms(2000);
    }
#elif TEST_MODE == 2
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
#else
    #error "End of tests"
#endif
}
