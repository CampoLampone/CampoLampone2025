#include "control_speed.h"
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

#else
    #error "End of tests"
#endif
}
