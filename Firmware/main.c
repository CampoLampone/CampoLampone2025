#include "speed_controller.h"
#include <stdint.h>
#include <stdio.h>
#include "pico/stdio_usb.h"
#include <pico/time.h>
#include "motor.h"
#include "encoder.h"
#include "config.h"
#include "spi.h"
#include "spi_cmds.h"

#if TEST_MODE > 0
#include "tests.h"
#endif

enum command {
    COMMAND_SPEED,
    COMMAND_POSITION,
    COMMAND_BRAKE,
    COMMAND_STOP,
    COMMAND_COAST
};

int16_t speed_target[MOTOR_COUNT] = {0, 0};
int8_t position_target[MOTOR_COUNT] = {0, 0};
uint8_t current_cmd = COMMAND_COAST;
substep_state_t encoders_states[ENCODER_COUNT];

void spi_callback(uint8_t *data){
        switch (data[0]) {
            case SPI_CMD_SET_SPEED_BASE:
                current_cmd = COMMAND_SPEED;
                speed_target[0] = (data[1] << 8) | data[2];
                clear_pid_cache();
                break;
            case SPI_CMD_SET_SPEED_BASE + SPI_CMD_NEXT_MOTOR:
                current_cmd = COMMAND_SPEED;
                speed_target[1] = (data[1] << 8) | data[2];
                clear_pid_cache();
                break;
            case SPI_CMD_SET_POSITION_BASE:
                current_cmd = COMMAND_POSITION;
                position_target[0] = data[1];
                speed_target[0] = (data[2] << 8) | data[3];
                break;
            case SPI_CMD_SET_POSITION_BASE + SPI_CMD_NEXT_MOTOR:
                current_cmd = COMMAND_POSITION;
                position_target[1] = data[1];
                speed_target[1] = (data[2] << 8) | data[3];
                break;
            case SPI_CMD_COAST:
                current_cmd = COMMAND_COAST;
                break;
            case SPI_CMD_BRAKE:
                current_cmd = COMMAND_BRAKE;
                break;
            case SPI_CMD_REL_ESTOP:
                reset_emergency_stop();
                break;
        }
}


int main() {
    stdio_usb_init();
    sleep_ms(2000);
    printf("Program Started\n");
#if TEST_MODE == 0
    motor_init();
    encoder_init();
    spi_init(spi_callback);
    speed_controller_init(PID_KP, PID_KI, PID_KD);
    reset_emergency_stop();
    absolute_time_t last_time = get_absolute_time();
    int wrum_time = last_time;
    while (true) {
        absolute_time_t current_time = get_absolute_time();
        int64_t delta_us = absolute_time_diff_us(last_time, current_time);
        last_time = current_time;
        switch (current_cmd) {
            case (COMMAND_SPEED):
                control_speed(speed_target, delta_us / 1000.0);
                break;
            case (COMMAND_POSITION):
                // TODO: Implement position control
                // control_position(position_target, speed_target, delta_us / 1000.0);
                break;
            case (COMMAND_COAST):
                for (int motor = 0; motor < MOTOR_COUNT; motor++) {
                    motor_coast(motor);
                }
                break;
            case (COMMAND_BRAKE):
                for (int motor = 0; motor < MOTOR_COUNT; motor++) {
                    motor_brake(motor);
                }
                break;
        }
        sleep_ms(10); // loop delay for PID
    }
#elif TEST_MODE > 0
    do_tests();
#endif

}
