#ifndef FIRMWARE_PID_H
#define FIRMWARE_PID_H

typedef struct {
    float Kp, Ki, Kd;
    float out_min, out_max;
    float integrator, prev_error;
} PID_Typedef;

float PID_Compute(PID_Typedef *pid, float setpoint, float process_value);
void PID_Reset(PID_Typedef *pid);

#endif //FIRMWARE_PID_H
