#pragma once

#include "pico/stdlib.h" // IWYU pragma: keep
#include <hardware/pio.h>


typedef struct substep_state_t {
    // configuration data:
    uint calibration_data[4]; // relative phase sizes
    uint clocks_per_us;       // save the clk_sys frequency in clocks per us
    uint idle_stop_samples;   // after these samples without transitions, assume the encoder is stopped
    PIO pio;
    uint sm;

    // internal fields to keep track of the previous state:
    uint prev_trans_pos, prev_trans_us;
    uint prev_step_us;
    uint prev_low, prev_high;
    uint idle_stop_sample_count;
    int speed_2_20;
    int stopped;

    // output of the encoder update function:
    int speed;     // estimated speed in substeps per second
    uint position; // estimated position in substeps

    uint raw_step; // raw step count
} substep_state_t;

enum encoder_num{
    ENCODER_RIGHT,
    ENCODER_LEFT,
    ENCODER_COUNT
};

const static PIO encoder_pio = pio0;

const static uint8_t encoder_sm[] = {
    [ENCODER_RIGHT] = 0,
    [ENCODER_LEFT]  = 1
};

const static uint8_t encoder_pins[] = {
    [ENCODER_RIGHT] = BOARD_ENCODER2_PIN_A,
    [ENCODER_LEFT]  = BOARD_ENCODER1_PIN_A
};

extern substep_state_t encoders_states[ENCODER_COUNT];

#define GET_ENCODER_PIO(encoder) (encoder_pio[encoder])
#define GET_ENCODER_SM(encoder) (encoder_sm[encoder])
#define GET_ENCODER_PIN(encoder) (encoder_pins[encoder])

static void substep_init_state(PIO pio, int sm, int pin_a, substep_state_t *state);
void substep_update(substep_state_t *state);
void substep_calibrate_phases(PIO pio, uint sm);
void substep_set_calibration_data(substep_state_t *state, int step0, int step1, int step2);
void encoder_init();
