#pragma once

#include "pico/stdlib.h" // IWYU pragma: keep

enum motor_num{
    MOTOR_RIGHT,
    MOTOR_LEFT,
    MOTOR_COUNT
};

static const uint8_t motor_pwm_pins[] = {
    [MOTOR_RIGHT] = BOARD_MOTOR_PIN_3,
    [MOTOR_LEFT]  = BOARD_MOTOR_PIN_1
};

static const uint8_t motor_dir_pins[] = {
    [MOTOR_RIGHT] = BOARD_MOTOR_PIN_4,
    [MOTOR_LEFT]  = BOARD_MOTOR_PIN_2
};

#define GET_MOTOR_PWM_PIN(motor) (motor_pwm_pins[motor])
#define GET_MOTOR_DIR_PIN(motor) (motor_dir_pins[motor])

// PWM GPIO slice numbers
#define PWM_GPIO_SLICE_NUM_CONST(gpio) \
    ((gpio) < 32 ? (((gpio) >> 1u) & 7u) : (8u + (((gpio) >> 1u) & 3u))) // compile time constant

static const uint8_t motor_slices[] = {
    [MOTOR_RIGHT] = PWM_GPIO_SLICE_NUM_CONST(GET_MOTOR_PWM_PIN(MOTOR_RIGHT)),
    [MOTOR_LEFT]  = PWM_GPIO_SLICE_NUM_CONST(GET_MOTOR_PWM_PIN(MOTOR_LEFT))
};

#define GET_MOTOR_SLICE(motor) (motor_slices[motor])

enum direction{
    DIRECTION_FORWARD,
    DIRECTION_BACKWARD
};

void motor_init(void);
void motor_set_pwm(uint8_t motor, bool direction, uint16_t level);
void motor_brake(uint8_t motor);
void motor_coast(uint8_t motor);
