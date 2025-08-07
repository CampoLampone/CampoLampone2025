#pragma once

#include "pico/stdlib.h" // IWYU pragma: keep
#include <hardware/pio.h>

enum encoder_num{
    ENCODER_RIGHT,
    ENCODER_LEFT,
    ENCODER_COUNT
};

const static uint8_t encoder_sm[] = {
    [ENCODER_RIGHT] = 0,
    [ENCODER_LEFT]  = 1
};

const static uint8_t encoder_pins[] = {
    [ENCODER_RIGHT] = BOARD_ENCODER2_PIN_A,
    [ENCODER_LEFT]  = BOARD_ENCODER1_PIN_A
};

#define GET_ENCODER_PIO(encoder) (encoder_pio[encoder])
#define GET_ENCODER_SM(encoder) (encoder_sm[encoder])
#define GET_ENCODER_PIN(encoder) (encoder_pins[encoder])

void encoder_init(void);
int encoder_get_count(uint8_t encoder);
