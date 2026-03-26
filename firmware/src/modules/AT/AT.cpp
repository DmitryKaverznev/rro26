#include "project_config.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

static struct pt task;
static char cmd_buffer[32]; 
static int buf_idx = 0;

enum {
    CMD_UNKNOWN = 0,
    CMD_AT,
    CMD_ALV
};

static void to_uppercase(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

int get_cmd_id(const char* str) {
    if (strcmp(str, "AT") == 0) return CMD_AT;
    if (strcmp(str, "AT+ALV") == 0) return CMD_ALV;
    
    return CMD_UNKNOWN;
}

void handle_command(const char* cmd_name, int argc, int p1, int p2) {
    switch (get_cmd_id(cmd_name)) {
        case CMD_AT:
            Serial.println("OK");
            break;

        case CMD_ALV:
            Serial.println("OK");
            break;

        default:
            Serial.print("ERROR: UNKNOWN COMMAND");
            break;
    }
}

static PT_THREAD(task_at_thread(struct pt *p)) {
    PT_BEGIN(p);

    for(;;) {
        PT_WAIT_UNTIL(p, Serial.available() > 0);

        char c = Serial.read();

        if (c == '\n' || c == '\r') {
            if (buf_idx > 0) {
                cmd_buffer[buf_idx] = '\0';
                to_uppercase(cmd_buffer);
                
                char cmd_name[16];
                int p1 = 0, p2 = 0;
                int res = 0;

                char* eq_pos = strchr(cmd_buffer, '=');
                if (eq_pos == NULL) {
                    res = sscanf(cmd_buffer, "%15s", cmd_name);
                } else {
                    res = sscanf(cmd_buffer, "%15[^=]=%d,%d", cmd_name, &p1, &p2);
                }
                
                if (res > 0) {
                    handle_command(cmd_name, res - 1, p1, p2);
                }
                
                buf_idx = 0;
            }
        } else if (buf_idx < (int)sizeof(cmd_buffer) - 1) {
            if (c != ' ') {
                cmd_buffer[buf_idx++] = c;
            }
        }
    }

    PT_END(p);
}

void Task_at() {
    static bool initialized = false;
    if (!initialized) {
        PT_INIT(&task);
        initialized = true;
    }
    task_at_thread(&task);
}
