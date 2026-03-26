#include <Arduino.h>

#include "tasks.h"

void setup() {
    Serial.begin(9600);
    

}

void loop() {
    Task_motor();
    // Task_global();
    Task_at();
    
}