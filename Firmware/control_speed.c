#include "encoder.h"
#include "motor.h"
#include "pid.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

PID_t motors_pid[MOTOR_COUNT] = { [0 ... MOTOR_COUNT-1] = {
    .kp = 5,
    .ki = 0.0,
    .kd = 0.0,
}};

float compute_encoder_rpm(uint8_t encoder, float delta_ms){
    static int last_count[ENCODER_COUNT] = {0};
    int count_now = encoder_get_count(encoder);
    int delta_count = count_now - last_count[encoder];
    last_count[encoder] = count_now;
    int measured_speed = (float) delta_count / delta_ms * 1.0e3;
    return (float) measured_speed / 617.3544 * 60.0;
}

float filter_speed_rpm(uint8_t encoder, float v) {
    static float v_prev[ENCODER_COUNT] = {0};
    static float v_filt[ENCODER_COUNT] = {0};

    v_filt[encoder] = 0.854 * v_filt[encoder] + 0.0728 * v + 0.0728 * v_prev[encoder];
    v_prev[encoder] = v;

    return  v_filt[encoder];
}

int clamp_int(int val, int min, int max) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

void control_motor_speed(int target_speed, uint8_t side, float delta_ms){
    PID_t* motor_pid = &motors_pid[side];
    float measured_rpm = compute_encoder_rpm(side, delta_ms);
    float filtered_rpm = filter_speed_rpm(side, measured_rpm);
    // printf("Encoder speed: %i delta: %f\n", measured_speed, delta_ms);
    printf("%f,%i,", filtered_rpm, target_speed);
    motor_pid->measured_value = filtered_rpm;
    motor_pid->dt = delta_ms;
    motor_pid->setpoint = target_speed;
    pid_compute(motor_pid);
    int pwm_value = motor_pid->output * 100;
    pwm_value = clamp_int(pwm_value, 0, 0xFFFF);
    motor_set_pwm(side, DIRECTION_FORWARD, pwm_value);
}

void control_speed(uint16_t target_speed[MOTOR_COUNT], float delta_ms) {
    for (int motor; motor < MOTOR_COUNT; motor++) {
        control_motor_speed(target_speed[motor], motor, delta_ms);
    }
    printf("\n");
}
