#include "encoder.h"
#include "motor.h"
#include "pid.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

PID_t motor_pid = {
    .kp = 2.0,
    .ki = 1.0,
    .kd = 0,
};

int compute_encoder_speed(uint8_t encoder){
    static int last_count[ENCODER_COUNT] = {0};
    int count_now = encoder_get_count(encoder);
    int delta_count = count_now - last_count[encoder];
    last_count[encoder] = count_now;
    return delta_count;
}

int clamp_int(int val, int min, int max) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

void control_motor_speed(int target_speed, uint8_t side, float delta_ms){
    int measured_speed = compute_encoder_speed(side);
    printf("Encoder speed: %i delta: %f\n", measured_speed, delta_ms);
    motor_pid.measured_value = measured_speed;
    motor_pid.dt = delta_ms;
    motor_pid.setpoint = target_speed;
    pid_compute(&motor_pid);
    int pwm_value = motor_pid.output * 1000;
    pwm_value = clamp_int(pwm_value, 0, 0xFFFF);
    motor_set_pwm(side, DIRECTION_FORWARD, 0.5*0xFFFF);
}

void control_speed(uint16_t target_speed[2], float delta_ms) {
    for (int motor; motor < MOTOR_COUNT; motor++) {
        control_motor_speed(target_speed[motor], motor, delta_ms);
    }
}
