/*
 * led_display.c - Output driver
 *  - LED giao thong: catot noi vao chan MCU -> ghi RESET = sang
 *  - 7 doan anode chung, segment ghi RESET = sang
 *  - EN0..EN3 qua transistor PNP -> ghi RESET = bat LED do
 */
#include "main.h"
#include "software_timer.h"
#include "led_display.h"

#define LED_ON    GPIO_PIN_RESET
#define LED_OFF   GPIO_PIN_SET

/* ---------- 7 doan ---------- */
static GPIO_TypeDef* const segPort[7] = {
    SEG0_GPIO_Port, SEG1_GPIO_Port, SEG2_GPIO_Port, SEG3_GPIO_Port,
    SEG4_GPIO_Port, SEG5_GPIO_Port, SEG6_GPIO_Port };
static const uint16_t segPin[7] = {
    SEG0_Pin, SEG1_Pin, SEG2_Pin, SEG3_Pin, SEG4_Pin, SEG5_Pin, SEG6_Pin };

static GPIO_TypeDef* const enPort[4] = {
    EN0_GPIO_Port, EN1_GPIO_Port, EN2_GPIO_Port, EN3_GPIO_Port };
static const uint16_t enPin[4] = { EN0_Pin, EN1_Pin, EN2_Pin, EN3_Pin };

/* bit i = 1 nghia la segment i (a..g) SANG */
static const uint8_t digitCode[10] = {
    0x3F, /* 0 */ 0x06, /* 1 */ 0x5B, /* 2 */ 0x4F, /* 3 */ 0x66, /* 4 */
    0x6D, /* 5 */ 0x7D, /* 6 */ 0x07, /* 7 */ 0x7F, /* 8 */ 0x6F  /* 9 */
};

static int led_buffer[4] = { 0, 0, 0, 0 };
static int scanIndex = 0;

void display7SEG(int num) {
    uint8_t code = (num >= 0 && num <= 9) ? digitCode[num] : 0x00;
    for (int i = 0; i < 7; i++) {
        HAL_GPIO_WritePin(segPort[i], segPin[i], (code & (1 << i)) ? LED_ON : LED_OFF);
    }
}

void update7SEG(int index) {
    if (index < 0 || index > 3) return;
    for (int i = 0; i < 4; i++) HAL_GPIO_WritePin(enPort[i], enPin[i], LED_OFF);
    display7SEG(led_buffer[index]);
    HAL_GPIO_WritePin(enPort[index], enPin[index], LED_ON);
}

void scan7SEG_run(void) {
    if (isTimerExpired(TIMER_SCAN)) {
        update7SEG(scanIndex);
        scanIndex = (scanIndex + 1) % 4;
        setTimer(TIMER_SCAN, SCAN_MS);
    }
}

void set7SEGBuffer(int left2, int right2) {
    if (left2 < 0)   left2 = 0;
    if (left2 > 99)  left2 = 99;
    if (right2 < 0)  right2 = 0;
    if (right2 > 99) right2 = 99;
    led_buffer[0] = left2 / 10;
    led_buffer[1] = left2 % 10;
    led_buffer[2] = right2 / 10;
    led_buffer[3] = right2 % 10;
}

/* ---------- Den giao thong ---------- */
static void writeRoad(GPIO_TypeDef* rP, uint16_t r,
                      GPIO_TypeDef* aP, uint16_t a,
                      GPIO_TypeDef* gP, uint16_t g, LightColor c) {
    HAL_GPIO_WritePin(rP, r, (c == LIGHT_RED)   ? LED_ON : LED_OFF);
    HAL_GPIO_WritePin(aP, a, (c == LIGHT_AMBER) ? LED_ON : LED_OFF);
    HAL_GPIO_WritePin(gP, g, (c == LIGHT_GREEN) ? LED_ON : LED_OFF);
}

void setRoad1(LightColor c) {
    writeRoad(RED1_GPIO_Port, RED1_Pin, AMBER1_GPIO_Port, AMBER1_Pin,
              GREEN1_GPIO_Port, GREEN1_Pin, c);
}

void setRoad2(LightColor c) {
    writeRoad(RED2_GPIO_Port, RED2_Pin, AMBER2_GPIO_Port, AMBER2_Pin,
              GREEN2_GPIO_Port, GREEN2_Pin, c);
}

void allLightsOff(void) {
    setRoad1(LIGHT_OFF);
    setRoad2(LIGHT_OFF);
}

void toggleColor(LightColor c) {
    switch (c) {
    case LIGHT_RED:
        HAL_GPIO_TogglePin(RED1_GPIO_Port, RED1_Pin);
        HAL_GPIO_TogglePin(RED2_GPIO_Port, RED2_Pin);
        break;
    case LIGHT_AMBER:
        HAL_GPIO_TogglePin(AMBER1_GPIO_Port, AMBER1_Pin);
        HAL_GPIO_TogglePin(AMBER2_GPIO_Port, AMBER2_Pin);
        break;
    case LIGHT_GREEN:
        HAL_GPIO_TogglePin(GREEN1_GPIO_Port, GREEN1_Pin);
        HAL_GPIO_TogglePin(GREEN2_GPIO_Port, GREEN2_Pin);
        break;
    default: break;
    }
}
