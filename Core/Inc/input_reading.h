/*
 * input_reading.h - Doc va chong doi (debounce) 3 nut nhan
 */
#ifndef INC_INPUT_READING_H_
#define INC_INPUT_READING_H_

#define NO_OF_BUTTONS        3
#define BUTTON_SCAN_MS       10     /* doc nut moi 10ms (>= TIMER_CYCLE)       */
#define LONG_PRESS_MS        1000   /* nhan giu > 1s thi bat dau tu dong lap    */
#define AUTO_REPEAT_MS       200    /* khi giu, cu 200ms tao them 1 lan nhan    */

void button_reading(void);                          /* goi trong ngat TIM2     */
unsigned char is_button_pressed(unsigned char index);     /* muc: dang nhan?   */
unsigned char is_button_pressed_1s(unsigned char index);  /* dang giu > 1s?    */
unsigned char isButtonPressed(unsigned char index);       /* su kien: 1 lan nhan (tu xoa co) */

#endif /* INC_INPUT_READING_H_ */
