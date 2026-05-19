#include "pid.h"

float PID_Compute(PID_Typedef *pid, float setpoint, float process_value) {
    const float error = setpoint - process_value;

    const float p_term = pid->Kp * error;

    pid->integrator += error;
    float i_term = pid->Ki * pid->integrator;

    if (i_term > pid->out_max) {
        i_term = pid->out_max;
        pid->integrator = pid->out_max / pid->Ki;
    } else if (i_term < pid->out_min) {
        i_term = pid->out_min;
        pid->integrator = pid->out_min / pid->Ki;
    }

    const  float d_term = pid->Kd * (error - pid->prev_error);
    pid->prev_error = error;

    float output = p_term + i_term + d_term;

    if (output > pid->out_max) {
        output = pid->out_max;
    } else if (output < pid->out_min) {
        output = pid->out_min;
    }

    return output;
}

void PID_Reset(PID_Typedef *pid) {
    pid->integrator = 0.0f;
    pid->prev_error = 0.0f;
}
