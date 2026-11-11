#include "software_timer.h"

static volatile int timer_counter[NUM_OF_TIMERS];
static volatile int timer_flag[NUM_OF_TIMERS];

void setTimer(int id, int duration_ms) {
    if (id < 0 || id >= NUM_OF_TIMERS) return;
    timer_flag[id]    = 0;
    timer_counter[id] = MS_TO_TICKS(duration_ms);
}

int isTimerExpired(int id) {
    if (id < 0 || id >= NUM_OF_TIMERS) return 0;
    if (timer_flag[id]) {
        timer_flag[id] = 0;
        return 1;
    }
    return 0;
}

void timerRun(void) {
    for (int i = 0; i < NUM_OF_TIMERS; i++) {
        if (timer_counter[i] > 0) {
            timer_counter[i]--;
            if (timer_counter[i] == 0) timer_flag[i] = 1;
        }
    }
}
