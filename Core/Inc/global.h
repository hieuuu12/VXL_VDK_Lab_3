/*
 * global.h - Bien toan cuc dung chung cho Lab 3 (Traffic light + 3 buttons)
 */
#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include "main.h"
#include "software_timer.h"
#include "input_reading.h"
#include "led_display.h"

/* ---- Cac mode ---- */
#define MODE_NORMAL   1
#define MODE_RED      2
#define MODE_AMBER    3
#define MODE_GREEN    4

/* ---- Thoi gian cho 1 lan nhay LED 2Hz: chu ky 500ms -> toggle moi 250ms ---- */
#define BLINK_TOGGLE_MS   250

/* ---- Gioi han gia tri thoi gian ---- */
#define DURATION_MIN  1
#define DURATION_MAX  99

extern int mode;          /* mode hien tai: 1..4 */
extern int durRed;        /* thoi gian den do (s)   */
extern int durAmber;      /* thoi gian den vang (s) */
extern int durGreen;      /* thoi gian den xanh (s) */
extern int tempValue;     /* gia tri dang chinh trong mode 2,3,4 */

#endif /* INC_GLOBAL_H_ */
