#pragma once

#include <stdint.h>

#define SUBSTEPS_PER_PULSE 64.0
#define PULSES_PER_MOTOR_ROTATION 12.0
#define GEARBOX_RATIO 51.4462

#define PULSES_PER_WHEEL_ROTATION (PULSES_PER_MOTOR_ROTATION * GEARBOX_RATIO)

#define PID_TO_PWM_SCALE 256
#define PID_I_MAX 256       // TODO: **Magic value** (find working number)

#define PWM_MAX 0xFFFF

#define STALL_THRESHOLD 1.0  // in RPM
#define STALL_TIME_MS 100    // Time in milliseconds

void speed_controller_init(float kp, float ki, float kd);
void reset_emergency_stop();
void clear_pid_cache();
void control_speed(int16_t target_speed[2], float delta_ms);
