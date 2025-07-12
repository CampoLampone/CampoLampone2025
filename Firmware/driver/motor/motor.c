#include "motor.h"
#include "hardware/pwm.h"
#include <hardware/gpio.h>


void setup_motor_pin(uint8_t motor){
    gpio_set_function(GET_MOTOR_PWM_PIN(motor), GPIO_FUNC_PWM);
    gpio_init(GET_MOTOR_DIR_PIN(motor));
    gpio_set_dir(GET_MOTOR_DIR_PIN(motor), GPIO_OUT);
}

void setup_pwm(uint8_t motor){
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 4.0f);
    pwm_init(GET_MOTOR_SLICE(motor), &config, true);
}

void motor_init(void){
    for (int motor = 0; motor < MOTOR_COUNT; motor++){
        setup_motor_pin(motor);
        setup_pwm(motor);
    }
}

void motor_brake(uint8_t motor){
    gpio_put(GET_MOTOR_DIR_PIN(motor), true);
    pwm_set_gpio_level(GET_MOTOR_PWM_PIN(motor), 0xFFFF);
    // TODO: Improve brake functionality
}

void motor_coast(uint8_t motor){
    gpio_put(GET_MOTOR_DIR_PIN(motor), false);
    pwm_set_gpio_level(GET_MOTOR_PWM_PIN(motor), 0x0);
}

void motor_set_pwm(uint8_t motor, bool direction, uint16_t level){
    gpio_put(GET_MOTOR_DIR_PIN(motor), direction);
    if (direction){
        pwm_set_gpio_level(GET_MOTOR_PWM_PIN(motor), 0xFFFF-level);
    }
    else{
        pwm_set_gpio_level(GET_MOTOR_PWM_PIN(motor), level);
    }

}
