/*
 * timer.c - Callback ngat TIM2 (moi TIMER_CYCLE ms)
 *  !! Xoa ham HAL_TIM_PeriodElapsedCallback cu trong main.c (neu co tu Lab 2)
 */
#include "main.h"
#include "software_timer.h"
#include "input_reading.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    static int buttonTick = 0;
    if (htim->Instance == TIM2) {
        timerRun();
        if (++buttonTick >= MS_TO_TICKS(BUTTON_SCAN_MS)) {
            buttonTick = 0;
            button_reading();
        }
    }
}
