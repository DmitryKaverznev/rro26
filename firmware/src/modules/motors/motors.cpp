#include "project_config.h"

#include "motors.h"
#include "pid/pid.h"

static struct pt task;

#define MOTORS_COUNT 2
Motor_Typedef motorsLeft[MOTORS_COUNT] = {{0, 0, 0, 0}, {0, 0, 0, 0}};
Motor_Typedef motorsRight[MOTORS_COUNT] = {{0, 0, 0, 0}, {0, 0, 0, 0}};
Encoder_Typedef encoderLeft[MOTORS_COUNT] = {{0, 0}, {0, 0}};
Encoder_Typedef encoderRight[MOTORS_COUNT] = {{0, 0}, {0, 0}};
PID motorsPIDLeft[MOTORS_COUNT] = {{1, 0, 0}, {1, 0, 0}};
PID motorsPIDRight[MOTORS_COUNT] = {{1, 0, 0}, {1, 0, 0}};

void isrL0() {
    if(digitalRead(encoderLeft[0].pin_dir)) encoderLeft[0].value++; 
    else encoderLeft[0].value--;
}

void isrL1() {
    if(digitalRead(encoderLeft[1].pin_dir)) encoderLeft[1].value++; 
    else encoderLeft[1].value--;
}

void isrR0() {
    if(digitalRead(encoderRight[0].pin_dir)) encoderRight[0].value++; 
    else encoderRight[0].value--;
}

void isrR1() {
    if(digitalRead(encoderRight[1].pin_dir)) encoderRight[1].value++; 
    else encoderRight[1].value--;
}

void MotorEncoder_Init() {
    Encoder_Init(encoderLeft[0], isrL0);
    Encoder_Init(encoderLeft[1], isrL1);
    Encoder_Init(encoderRight[0], isrR0);
    Encoder_Init(encoderRight[1], isrR1);
    
    for(int i = 0; i < MOTORS_COUNT; i++) {
        Motor_Init(motorsLeft[i]);
        Motor_Init(motorsRight[i]);
    }
}

void Motor_Init(Motor_Typedef &motor)
{
    pinMode(motor.pin_pir, OUTPUT);
    pinMode(motor.pin_speed, OUTPUT);
}

void Encoder_Init(Encoder_Typedef &encoder, void (*isr)()) {
    pinMode(encoder.pin_int, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(encoder.pin_int), isr, RISING);
}

void Motors_Set(Motor_Typedef &motor, uint16_t _speed, uint16_t _acceleration)
{
    motor.speed = _speed;
    motor.acceleration = _acceleration;
}
\
void Motor_Update(Motor_Typedef &motor, int16_t pid_out) {
    uint8_t pwm = constrain(abs(pid_out), 0, 255);

    analogWrite(motor.pin_speed, pwm);
    digitalWrite(motor.pin_pir, pid_out >= 0 ? HIGH : LOW);
}

#define DELAY_LOOP 20
// Задача моторов
static PT_THREAD(task_motor_thread(struct pt *p)) {
    static uint32_t last_time;

    PT_BEGIN(p);

    for(;;) {
        for(int i = 0; i < MOTORS_COUNT; i++) {
            int16_t outL = PID_Output(motorsPIDLeft[i], motorsLeft[i].speed_now, motorsLeft[i].speed);
            int16_t outR = PID_Output(motorsPIDRight[i], motorsRight[i].speed_now, motorsRight[i].speed);

            Motor_Update(motorsLeft[i], outL);
            Motor_Update(motorsRight[i], outR);
        }


        PT_DELAY(p, last_time, DELAY_LOOP);
    }

    PT_END(p);
}

void Task_motor() {
    static bool initialized = false;
    if (!initialized) {
        PT_INIT(&task);
        initialized = true;
    }
    task_motor_thread(&task);
}

