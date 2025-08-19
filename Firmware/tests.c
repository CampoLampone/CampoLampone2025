#include "config.h"
#if TEST_MODE != 0
#include "spi.h"
#include "speed_controller.h"
#include <stdint.h>
#include <stdio.h>
#include "tests.h"
#include "encoder.h"
#include "motor.h"
#include "display.h"


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
        encoders_init();
        while (true){
            motor_set_pwm(MOTOR_RIGHT, 65000);
            motor_set_pwm(MOTOR_LEFT, -65000);
            sleep_ms(2000);
            motor_set_pwm(MOTOR_RIGHT, -65000);
            motor_set_pwm(MOTOR_LEFT, 65000);
            sleep_ms(2000);
        }
    #elif TEST_MODE == 2
        encoders_init();
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
        encoders_init();
        extern substep_state_t encoders_states[ENCODER_COUNT];
        int16_t control[2] = {0, 0};
        speed_controller_init(PID_KP, PID_KI, PID_KD);
        reset_emergency_stop();

        display_data_t display_struct;

        i2c_init(i2c_default, 400 * 1000);
        ssd1306_gpio_init(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN);

        ssd1306_t disp;
        disp.external_vcc=false;
        ssd1306_init(&disp, 128, 64, true, 0x3C, i2c_default);
        ssd1306_clear(&disp);

        absolute_time_t last_time = get_absolute_time();
        int wrum_time = last_time;
        while (true) {
            absolute_time_t current_time = get_absolute_time();
            int64_t delta_us = absolute_time_diff_us(last_time, current_time);
            last_time = current_time;

            compute_encoders_rpm(delta_us / 1000.0);
            control_speed(control, delta_us / 1000.0);

            snprintf(display_struct.ip, DISP_BUF(IP_SCALE), "HoStNaMe!!");
            snprintf(display_struct.msg, DISP_BUF(MSG_SCALE), get_emergency_stop() ? "ESTOP" : "     ");
            snprintf(display_struct.stuff, DISP_BUF(STUFF_SCALE), "%.0f, %.0f", get_rpm(MOTOR_LEFT), get_rpm(MOTOR_RIGHT));
            ssd1306_draw_struct(&disp, &display_struct);
            ssd1306_show(&disp);

            sleep_ms(10);
            if (current_time - wrum_time > 5e4) {
                wrum_time = current_time;
                if (control[0] == 0) {
                    control[0] = 60;
                    control[1] = -60;
                } else {
                    control[0] = 50;
                    control[1] = -50;
                }
            }
        }
    #else
        #error "End of tests"
    #endif
}
#endif
