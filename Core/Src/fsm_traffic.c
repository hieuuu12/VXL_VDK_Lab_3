/*
 * fsm_traffic.c - FSM den giao thong o Mode 1 (normal)
 *
 *  Duong 1 = 2 LED 7 doan ben trai, Duong 2 = 2 LED ben phai.
 *  Rang buoc: durRed = durAmber + durGreen.
 *
 *  INIT -> R1_G2 -(het xanh 2)-> R1_A2 -(het vang 2)-> G1_R2
 *       -(het xanh 1)-> A1_R2 -(het vang 1)-> R1_G2 ...
 */
#include "global.h"
#include "fsm_traffic.h"

typedef enum { TL_INIT, TL_R1_G2, TL_R1_A2, TL_G1_R2, TL_A1_R2 } TrafficState;

static TrafficState tlState = TL_INIT;
static int time1 = 0;   /* thoi gian con lai cua duong 1 */
static int time2 = 0;   /* thoi gian con lai cua duong 2 */

void fsm_traffic_reset(void) {
    tlState = TL_INIT;
}

void fsm_traffic_run(void) {
    int tick = 0;

    if (tlState == TL_INIT) {
        tlState = TL_R1_G2;
        time1 = durRed;
        time2 = durGreen;
        setRoad1(LIGHT_RED);
        setRoad2(LIGHT_GREEN);
        setTimer(TIMER_SECOND, 1000);
    } else if (isTimerExpired(TIMER_SECOND)) {
        setTimer(TIMER_SECOND, 1000);
        time1--;
        time2--;
        tick = 1;
    }

    if (tick) {
        switch (tlState) {
        case TL_R1_G2:
            if (time2 <= 0) {
                tlState = TL_R1_A2;
                time2 = durAmber;
                setRoad2(LIGHT_AMBER);
            }
            break;
        case TL_R1_A2:
            if (time2 <= 0) {
                tlState = TL_G1_R2;
                time1 = durGreen;
                time2 = durRed;
                setRoad1(LIGHT_GREEN);
                setRoad2(LIGHT_RED);
            }
            break;
        case TL_G1_R2:
            if (time1 <= 0) {
                tlState = TL_A1_R2;
                time1 = durAmber;
                setRoad1(LIGHT_AMBER);
            }
            break;
        case TL_A1_R2:
            if (time1 <= 0) {
                tlState = TL_R1_G2;
                time1 = durRed;
                time2 = durGreen;
                setRoad1(LIGHT_RED);
                setRoad2(LIGHT_GREEN);
            }
            break;
        default:
            tlState = TL_INIT;
            break;
        }
    }

    set7SEGBuffer(time1, time2);
}
