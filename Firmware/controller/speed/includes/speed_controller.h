#pragma once

#include <stdint.h>

#define SUBSTEPS_PER_PULSE 64.0
#define PULSES_PER_MOTOR_ROTATION 12.0
#define GEARBOX_RATIO 51.4462

#define PULSES_PER_WHEEL_ROTATION (PULSES_PER_MOTOR_ROTATION * GEARBOX_RATIO)

#define PID_TO_PWM_SCALE 256

#define PWM_MAX 0xFFFF

void speed_controller_init(float kp, float ki, float kd);
void control_speed(int16_t target_speed[2], float delta_ms);
