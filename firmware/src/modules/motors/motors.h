#pragma once
#include <stdint.h>
#include <Arduino.h> // Нужно для pinMode и attachInterrupt

typedef struct {
    uint8_t pin_speed;
    uint8_t pin_pir;
    int16_t speed_now;
    int16_t speed;
    uint16_t acceleration;
} Motor_Typedef;

typedef struct {
    uint8_t pin_int;
    uint8_t pin_dir;
    volatile int32_t value;
} Encoder_Typedef;

// Прототипы функций
void MotorEncoder_Init();
void Motor_Init(Motor_Typedef &motor);
void Encoder_Init(Encoder_Typedef &encoder, void (*isr)());
void Motors_Set(Motor_Typedef &motor, int16_t _speed, uint16_t _acceleration);
void Motor_Update(Motor_Typedef &motor, int16_t pid_out); 
