/**
 * @file airpic-timer.h
 * @brief Configure and use timed sleep functions and Timer4/5 ISR.
 * Airpic's Timer library simplifies the usage of the PIC24 device's on-board 
 * timers by taking the user's parameters and handles the configuration of the 
 * timers accordingly.
 * @author Evan Perry Grove
 * @date 9/29/2017
 */

#ifndef AIRPIC_TIMER_H
#define	AIRPIC_TIMER_H

#include "xc.h"
#include <p24Fxxxx.h>

/** @brief Configure airpic-timer to the default settings (interrupt enabled with a 100ms period between interrupts) */
#define AIRPIC_TIMER_DEFAULT    0b00000000

/** @brief Timed ISR is not enabled (default). */
#define AIRPIC_TIMER_INTERRUPT_ENABLE  0b00000000  //0 (default)

/** @brief Timed ISR is enabled. */
#define AIRPIC_TIMER_INTERRUPT_DISABLE 0b00000001  //1

/** @brief Period between timed ISR events is 20ms. */
#define AIRPIC_TIMER_PERIOD_20MS   0b00000010       //2

/** @brief Period between timed ISR events is 50ms. */
#define AIRPIC_TIMER_PERIOD_50MS   0b00000100       //4

/** @brief Period between timed ISR events is 100ms (default). */
#define AIRPIC_TIMER_PERIOD_100MS  0b00000000       //0 (default)

/** @brief Period between timed ISR events is 200ms. */
#define AIRPIC_TIMER_PERIOD_200MS  0b00000110       //6

/** @brief Period between timed ISR events is 500ms. */
#define AIRPIC_TIMER_PERIOD_500MS  0b00001000       //8

/** @brief Period between timed ISR events is 1000ms. */
#define AIRPIC_TIMER_PERIOD_1000MS 0b00001010       //10

/** @brief Period between timed ISR events is 2000ms. */
#define AIRPIC_TIMER_PERIOD_2000MS 0b00001100       //12

/** @brief Period between timed ISR events is 5000ms. */
#define AIRPIC_TIMER_PERIOD_5000MS 0b00001110       //14

/** @brief Macro to make the timed ISR more readable. */
#define airpic_timer_isr __attribute__((__interrupt__,no_auto_psv))_T5Interrupt

/** @brief Macro to make the clearing of the T5IF flag more readable. */
#define airpic_timer_isr_exit (IFS1bits.T5IF = 0)

#ifdef	__cplusplus
extern "C" {
#endif
    
    /**
     * Configure the device for using the airpic-timer library.
     * airpic-timer uses Timer4 and Timer5. The conf variable decides some of 
     * the properties of the airpic-timer library.
     * <p><ul>
     * <li> AIRPIC_TIMER_INTERRUPT_ENABLE: enable the Timer5 interrupt.
     * <li> AIRPIC_TIMER_INTERRUPT_DISABLE: do not enable the Timer5 interrupt.
     * <li> AIRPIC_TIMER_PERIOD_xxxx: sets the period of the timer. Replace xxxx 
     * with 20MS, 50MS, 100MS, 200MS, 500MS, 1000MS, 2000MS, or 5000MS.
     * </ul><p>
     * @param conf Configuration value for airpic-timer. Use the macros 
     * at the top of the header appended together by the | operator for ease of use. 
     */
    void timer_config(unsigned short conf);
    
    /**
     * Do nothing for a specified number of milliseconds. If an ISR is triggered 
     * while sleep_msec() is running, the interrupt will still run.
     * @param delay length of delay in milliseconds
     */
    void sleep_msec(unsigned int delay);


#ifdef	__cplusplus
}
#endif

#endif	/* AIRPIC_TIMER_H */

