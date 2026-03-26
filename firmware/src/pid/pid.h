#pragma once

typedef struct {
    float kp, ki, kd;
    float integral;
    float prev_error;
    float max_ki;
} PID_Typedef;

float PID_Compute(PID_Typedef &pid, float setpoint, float input, float dt);