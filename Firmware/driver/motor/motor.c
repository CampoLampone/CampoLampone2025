#include "motor.h"
#include "hardware/pwm.h"
#include <hardware/gpio.h>


void setup_motor_pin(uint8_t motor){
    gpio_set_function(GET_MOTOR_A_PIN(motor), GPIO_FUNC_PWM);
    gpio_set_function(GET_MOTOR_B_PIN(motor), GPIO_FUNC_PWM);
}

void setup_pwm(uint8_t motor){
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 4.0f);
    pwm_init(GET_MOTOR_SLICE(motor), &config, true);
}

void motor_init(void){
    gpio_init(BOARD_MOTOR_EEP);
    gpio_set_dir(BOARD_MOTOR_EEP, GPIO_OUT);
    gpio_put(BOARD_MOTOR_EEP, true);
    for (int motor = 0; motor < MOTOR_COUNT; motor++){
        setup_motor_pin(motor);
        setup_pwm(motor);
    }
}

void motor_brake(uint8_t motor){
    pwm_set_gpio_level(GET_MOTOR_A_PIN(motor), 0xFFFF);
    pwm_set_gpio_level(GET_MOTOR_B_PIN(motor), 0xFFFF);
}

void motor_coast(uint8_t motor){
    pwm_set_gpio_level(GET_MOTOR_A_PIN(motor), 0x0);
    pwm_set_gpio_level(GET_MOTOR_B_PIN(motor), 0x0);
}


void motor_set_pwm(uint8_t motor, int32_t level){
    if (level >= 0){
        pwm_set_gpio_level(GET_MOTOR_A_PIN(motor), level);
        pwm_set_gpio_level(GET_MOTOR_B_PIN(motor), 0);
    }
    else{
        pwm_set_gpio_level(GET_MOTOR_A_PIN(motor), 0);
        pwm_set_gpio_level(GET_MOTOR_B_PIN(motor), -level);
    }
}
