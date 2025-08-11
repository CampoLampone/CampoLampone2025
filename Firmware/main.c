#include "speed_controller.h"
#include <stdio.h>
#include "pico/stdio_usb.h"
#include <pico/time.h>
#include "motor.h"
#include "encoder.h"
#include "config.h"
#include "spi.h"

#if TEST_MODE > 0
#include "tests.h"
#endif

enum command {
    COMMAND_SPEED,
    COMMAND_POSITION,
    COMMAND_STOP,
    COMMAND_COAST
};

int16_t control[2] = {0, 0};
substep_state_t encoders_states[ENCODER_COUNT];

void spi_callback(uint8_t *data){
    printf("Received: ");
        for (int i = 0; i < 8; i++) {
            printf("%02X ", data[i]);
        }
        printf("\n");
        switch (data[0]) {
            case 0x1A:
                control[0] = (data[1] << 8) | data[2];
                break;
            case 0x2A:
                control[1] = (data[1] << 8) | data[2];
                break;
        }
}


int main() {
    sleep_ms(2000);
    printf("Program Started\n");
#if TEST_MODE == 0
    motor_init();
    stdio_usb_init();
    encoder_init();
    spi_init(spi_callback);
    speed_controller_init(PID_KP, PID_KI, PID_KD);
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
#elif TEST_MODE > 0
    do_tests();
#endif

}
