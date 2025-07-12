
#include "includes/pid.h"

void pid_compute(PID_t *pid) {
    float error = pid->setpoint - pid->measured_value;

    pid->integral += error * pid->dt;
    float derivative = (error - pid->prev_error) / pid->dt;

    pid->output = pid->kp * error + pid->ki * pid->integral + pid->kd * derivative;
    pid->prev_error = error;
}
