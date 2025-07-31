#include "control_speed.h"
#include "pico/stdio_usb.h"
#include <pico/time.h>
#include "motor.h"
#include "encoder.h"

enum command {
    COMMAND_SPEED,
    COMMAND_POSITION,
    COMMAND_STOP,
    COMMAND_COAST
};

uint16_t control[2] = {0, 0};

int main() {
    motor_init();
    stdio_usb_init();
    encoder_init();
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
        sleep_ms(5);

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
}
