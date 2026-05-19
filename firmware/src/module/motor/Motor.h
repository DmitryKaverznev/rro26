#ifndef FIRMWARE_MOTOR_H
#define FIRMWARE_MOTOR_H

#include <stdint.h>
#include "PID.h"

typedef struct {
    uint8_t pin_a, pin_b;
    PID_Typedef pid;
} Motor_Typedef;

void Motor_Init(Motor_Typedef* motor);
void Motor_Set(Motor_Typedef* motor);
void Motor_Update(Motor_Typedef* motor);


#endif //FIRMWARE_MOTOR_H