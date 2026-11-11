/*
 * led_display.h - Output driver: 6 kenh den giao thong + 4 LED 7 doan
 */
#ifndef INC_LED_DISPLAY_H_
#define INC_LED_DISPLAY_H_

#define SCAN_MS   10    /* thoi gian sang moi LED 7 doan (ms) */

typedef enum { LIGHT_OFF, LIGHT_RED, LIGHT_AMBER, LIGHT_GREEN } LightColor;

void display7SEG(int num);
void update7SEG(int index);
void scan7SEG_run(void);                      /* goi trong super loop */
void set7SEGBuffer(int left2, int right2);    /* 2 so: [0][1] va [2][3] */

void setRoad1(LightColor c);
void setRoad2(LightColor c);
void allLightsOff(void);
void toggleColor(LightColor c);               /* toggle ca 2 duong cua 1 mau */

#endif /* INC_LED_DISPLAY_H_ */
