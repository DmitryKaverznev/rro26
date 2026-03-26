#include "project_config.h"

static struct pt task;
static unsigned long last_time = 0;

static PT_THREAD(task_global_thread(struct pt *p)) {
    PT_BEGIN(p);
    
    for(;;) {
        PT_WAIT_UNTIL(p, millis() - last_time >= 1000);
        
        last_time = millis();
        
        Serial.print("Task Global is alive! Uptime: ");
        Serial.print(last_time / 1000);
        Serial.println(" sec");
    }

    PT_END(p);
}

void Task_global() {
    static bool initialized = false;
    if (!initialized) {
        PT_INIT(&task);
        initialized = true;
    }
    task_global_thread(&task);
}
