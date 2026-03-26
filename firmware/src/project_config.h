#pragma once

#include <Arduino.h>
#include <pt.h>

#define PT_DELAY(pt, last_time, ms) \
    last_time = millis(); \
    PT_WAIT_UNTIL(pt, millis() - last_time >= (ms));
