/*
 * input_reading.c - Debounce 3 nut (active low, co pull-up)
 *
 * Moi BUTTON_SCAN_MS doc 1 lan. Chi chap nhan trang thai moi khi 3 lan doc
 * lien tiep giong nhau (N = 3) -> loc doi phim va nhieu.
 */
#include "main.h"
#include "software_timer.h"
#include "input_reading.h"

#define BUTTON_IS_PRESSED   GPIO_PIN_RESET
#define BUTTON_IS_RELEASED  GPIO_PIN_SET

#define LONG_PRESS_TICKS   (LONG_PRESS_MS  / BUTTON_SCAN_MS)
#define AUTO_REPEAT_TICKS  (AUTO_REPEAT_MS / BUTTON_SCAN_MS)

static GPIO_TypeDef* const buttonPort[NO_OF_BUTTONS] =
    { BUTTON1_GPIO_Port, BUTTON2_GPIO_Port, BUTTON3_GPIO_Port };
static const uint16_t buttonPin[NO_OF_BUTTONS] =
    { BUTTON1_Pin, BUTTON2_Pin, BUTTON3_Pin };

/* Nut nao duoc phep tu dong lap khi giu: chi BUTTON2 (tang gia tri) */
static const uint8_t autoRepeatEnable[NO_OF_BUTTONS] = { 0, 1, 0 };

/* 3 thanh ghi debounce + trang thai on dinh */
static GPIO_PinState keyReg0[NO_OF_BUTTONS];
static GPIO_PinState keyReg1[NO_OF_BUTTONS];
static GPIO_PinState keyReg2[NO_OF_BUTTONS];
static GPIO_PinState buttonBuffer[NO_OF_BUTTONS];  /* ket qua sau debounce */

static uint16_t counterForButtonPress1s[NO_OF_BUTTONS];
static uint16_t counterForRepeat[NO_OF_BUTTONS];
static volatile uint8_t flagForButtonPress1s[NO_OF_BUTTONS];
static volatile uint8_t flagPressed[NO_OF_BUTTONS];   /* su kien nhan */

static uint8_t initialized = 0;

void button_reading(void) {
    if (!initialized) {
        for (int i = 0; i < NO_OF_BUTTONS; i++) {
            keyReg0[i] = keyReg1[i] = keyReg2[i] = buttonBuffer[i] = BUTTON_IS_RELEASED;
        }
        initialized = 1;
    }

    for (int i = 0; i < NO_OF_BUTTONS; i++) {
        keyReg2[i] = keyReg1[i];
        keyReg1[i] = keyReg0[i];
        keyReg0[i] = HAL_GPIO_ReadPin(buttonPort[i], buttonPin[i]);

        /* Chi xu ly khi 3 lan doc giong nhau (da het doi) */
        if (keyReg0[i] == keyReg1[i] && keyReg1[i] == keyReg2[i]) {

            if (buttonBuffer[i] != keyReg2[i]) {
                /* Trang thai thay doi */
                buttonBuffer[i] = keyReg2[i];
                if (buttonBuffer[i] == BUTTON_IS_PRESSED) {
                    flagPressed[i] = 1;          /* canh xuong: 1 lan nhan */
                    counterForButtonPress1s[i] = 0;
                } else {
                    counterForButtonPress1s[i] = 0;
                    flagForButtonPress1s[i] = 0;
                }
            } else if (buttonBuffer[i] == BUTTON_IS_PRESSED) {
                /* Dang giu nut */
                if (counterForButtonPress1s[i] < LONG_PRESS_TICKS) {
                    counterForButtonPress1s[i]++;
                    if (counterForButtonPress1s[i] == LONG_PRESS_TICKS) {
                        flagForButtonPress1s[i] = 1;
                        counterForRepeat[i] = AUTO_REPEAT_TICKS;
                    }
                } else if (autoRepeatEnable[i]) {
                    /* Giu > 1s: cu AUTO_REPEAT_MS tao them 1 su kien nhan */
                    if (--counterForRepeat[i] == 0) {
                        flagPressed[i] = 1;
                        counterForRepeat[i] = AUTO_REPEAT_TICKS;
                    }
                }
            }
        }
    }
}

unsigned char is_button_pressed(unsigned char index) {
    if (index >= NO_OF_BUTTONS) return 0;
    return (buttonBuffer[index] == BUTTON_IS_PRESSED);
}

unsigned char is_button_pressed_1s(unsigned char index) {
    if (index >= NO_OF_BUTTONS) return 0;
    return (flagForButtonPress1s[index] == 1);
}

unsigned char isButtonPressed(unsigned char index) {
    if (index >= NO_OF_BUTTONS) return 0;
    if (flagPressed[index]) {
        flagPressed[index] = 0;
        return 1;
    }
    return 0;
}
