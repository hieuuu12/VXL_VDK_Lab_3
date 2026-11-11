/*
 * fsm_mode.c - FSM chon mode (Exercise 5 -> 9)
 *
 *  BUTTON1 (index 0): chuyen mode 1 -> 2 -> 3 -> 4 -> 1
 *  BUTTON2 (index 1): tang gia tri tam (1..99, qua 99 quay ve 1), giu > 1s tu tang
 *  BUTTON3 (index 2): luu gia tri tam cho mau dang chinh
 *
 *  Mode 2/3/4: LED cua mau tuong ung nhay 2Hz,
 *              7 doan trai = gia tri dang chinh, 7 doan phai = so mode.
 */
#include "global.h"
#include "fsm_traffic.h"
#include "fsm_mode.h"

static LightColor modeColor(int m) {
    if (m == MODE_RED)   return LIGHT_RED;
    if (m == MODE_AMBER) return LIGHT_AMBER;
    if (m == MODE_GREEN) return LIGHT_GREEN;
    return LIGHT_OFF;
}

/* Luu gia tri va giu rang buoc durRed = durAmber + durGreen.
 * Tra ve 1 neu luu duoc, 0 neu gia tri khong hop le (giu nguyen gia tri cu). */
static int saveValue(int m, int value) {
    switch (m) {
    case MODE_RED:       /* doi do -> tinh lai xanh */
        if (value <= durAmber) return 0;
        durRed   = value;
        durGreen = durRed - durAmber;
        return 1;
    case MODE_AMBER:     /* doi vang -> tinh lai do */
        if (value + durGreen > DURATION_MAX) return 0;
        durAmber = value;
        durRed   = durAmber + durGreen;
        return 1;
    case MODE_GREEN:     /* doi xanh -> tinh lai do */
        if (value + durAmber > DURATION_MAX) return 0;
        durGreen = value;
        durRed   = durAmber + durGreen;
        return 1;
    default:
        return 0;
    }
}

static void enterMode(int m) {
    allLightsOff();
    switch (m) {
    case MODE_NORMAL:
        fsm_traffic_reset();
        break;
    case MODE_RED:   tempValue = durRed;   break;
    case MODE_AMBER: tempValue = durAmber; break;
    case MODE_GREEN: tempValue = durGreen; break;
    default: break;
    }
    if (m != MODE_NORMAL) setTimer(TIMER_BLINK, BLINK_TOGGLE_MS);
}

void fsm_mode_init(void) {
    mode = MODE_NORMAL;
    enterMode(mode);
}

void fsm_mode_run(void) {
    /* BUTTON1: doi mode */
    if (isButtonPressed(0)) {
        mode++;
        if (mode > MODE_GREEN) mode = MODE_NORMAL;
        enterMode(mode);
    }

    switch (mode) {
    case MODE_NORMAL:
        /* bo qua su kien cua BUTTON2, BUTTON3 o mode 1 */
        (void)isButtonPressed(1);
        (void)isButtonPressed(2);
        fsm_traffic_run();
        break;

    case MODE_RED:
    case MODE_AMBER:
    case MODE_GREEN:
        if (isTimerExpired(TIMER_BLINK)) {
            toggleColor(modeColor(mode));
            setTimer(TIMER_BLINK, BLINK_TOGGLE_MS);
        }
        if (isButtonPressed(1)) {
            tempValue++;
            if (tempValue > DURATION_MAX) tempValue = DURATION_MIN;
        }
        if (isButtonPressed(2)) {
            if (!saveValue(mode, tempValue)) {
                /* khong hop le -> tra ve gia tri dang luu */
                if (mode == MODE_RED)   tempValue = durRed;
                if (mode == MODE_AMBER) tempValue = durAmber;
                if (mode == MODE_GREEN) tempValue = durGreen;
            }
        }
        set7SEGBuffer(tempValue, mode);
        break;

    default:
        mode = MODE_NORMAL;
        enterMode(mode);
        break;
    }
}
