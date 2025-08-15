#include "encoder.h"
#include "motor.h"
#include "pid.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "speed_controller.h"

PID_t motors_pid[MOTOR_COUNT];

float compute_encoder_rpm(uint8_t encoder, float delta_ms){
    substep_update(&encoders_states[encoder]);
    int measured_speed = encoders_states[encoder].speed;
    return (float) measured_speed / SUBSTEPS_PER_PULSE / PULSES_PER_WHEEL_ROTATION * 60.0;
}

int clamp_pid_to_pwm(int val) {
    val = val * PID_TO_PWM_SCALE;
    if (val < -PWM_MAX) return -PWM_MAX;
    if (val > PWM_MAX) return PWM_MAX;
    return val;
}

void control_motor_speed(int16_t target_speed, uint8_t side, float delta_ms){
    float measured_rpm = compute_encoder_rpm(side, delta_ms);
    motors_pid[side].measured_value = measured_rpm;
    motors_pid[side].dt = delta_ms;
    motors_pid[side].setpoint = target_speed;
    pid_compute(&motors_pid[side]);
    motor_set_pwm(side, clamp_pid_to_pwm(motors_pid[side].output));
    printf("%f,%i,", measured_rpm, target_speed);
}

void control_speed(int16_t target_speed[MOTOR_COUNT], float delta_ms) {
    for (int motor = 0; motor < MOTOR_COUNT; motor++) {
        control_motor_speed(target_speed[motor], motor, delta_ms);
    }
    printf("\n");
}

void clear_pid_cache(){
    for (int motor = 0; motor < MOTOR_COUNT; motor++) {
        motors_pid[motor].integral = 0;
        motors_pid[motor].prev_error = 0;
        motors_pid[motor].prev_error = 0;
        motors_pid[motor].output = 0;
    }
}

void speed_controller_init(float kp, float ki, float kd) {
    for (int motor = 0; motor < MOTOR_COUNT; motor++) {
        motors_pid[motor].kp = kp;
        motors_pid[motor].ki = ki;
        motors_pid[motor].kd = kd;
    }
}
