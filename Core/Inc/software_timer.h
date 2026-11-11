/*
 * software_timer.h - Nhieu software timer chay tren 1 timer interrupt (TIM2)
 *
 * Exercise 4: Chi can sua TIMER_CYCLE khi doi chu ky ngat cua TIM2.
 * Moi thoi gian khac trong chuong trinh deu tinh bang ms nen khong bi anh huong.
 */
#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

/* Chu ky ngat TIM2 (ms). Prescaler 7999, Period 9 voi clock 8MHz -> 10ms */
#define TIMER_CYCLE   10

/* Danh sach software timer */
enum {
    TIMER_SECOND = 0,   /* dem nguoc 1 giay cho den giao thong */
    TIMER_BLINK,        /* nhay LED 2Hz trong mode 2,3,4         */
    TIMER_SCAN,         /* quet 4 LED 7 doan                     */
    NUM_OF_TIMERS
};

void setTimer(int id, int duration_ms);
int  isTimerExpired(int id);     /* tra ve 1 neu het gio (va tu xoa co) */
void timerRun(void);             /* goi trong ngat TIM2                 */

/* Doi ms -> so lan ngat, toi thieu la 1 */
#define MS_TO_TICKS(ms)  (((ms) / TIMER_CYCLE) > 0 ? ((ms) / TIMER_CYCLE) : 1)

#endif /* INC_SOFTWARE_TIMER_H_ */
