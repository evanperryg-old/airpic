/**
 * @file airpic-timer.h
 * @brief Airpic-timer contains utilities for setting up and using the on-board 
 * timers.
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

#define AIRPIC_TIMER_INTERRUPT_DISABLE  0b00000000  //0 (default)
#define AIRPIC_TIMER_INTERRUPT_ENABLE 0b00000001    //1
#define AIRPIC_TIMER_PERIOD_20MS   0b00000010       //2
#define AIRPIC_TIMER_PERIOD_50MS   0b00000100       //4
#define AIRPIC_TIMER_PERIOD_100MS  0b00000000       //0 (default)
#define AIRPIC_TIMER_PERIOD_200MS  0b00000110       //6
#define AIRPIC_TIMER_PERIOD_500MS  0b00001000       //8
#define AIRPIC_TIMER_PERIOD_1000MS 0b00001010       //10
#define AIRPIC_TIMER_PERIOD_2000MS 0b00001100       //12
#define AIRPIC_TIMER_PERIOD_5000MS 0b00001110       //14

#define airpic_timer_isr __attribute__((__interrupt__,no_auto_psv))_T5Interrupt
#define airpic_timer_isr_exit (IFS1bits.T5IF = 0)

#ifdef	__cplusplus
extern "C" {
#endif
    
    
    /**
     * Configure the device for using the airpic-timer library.
     * <p>
     * airpic-timer uses Timer4 and Timer5. The conf variable decides some of 
     * the properties of the airpic-timer library.
     * <p><ul>
     * <li> AIRPIC_TIMER_INTERRUPT_ENABLE: enable the Timer5 interrupt (default).
     * <li> AIRPIC_TIMER_INTERRUPT_DISABLE: do not enable the Timer5 interrupt.
     * <li> AIRPIC_TIMER_PERIOD_xxxx: sets the period of the timer. Replace xxxx 
     * with 20MS, 50MS, 100MS, 200MS, 500MS, 1000MS, 2000MS, or 5000MS. (default is 100MS)
     * </ul><p>
     * @param  conf Configuration value for airpic-timer. Use the #define macros 
     * at the top of the header appended together by the | operator for ease of use.
     * @return void  
     */
    void timer_config(unsigned short conf);
    
    void sleep_msec(unsigned int delay);


#ifdef	__cplusplus
}
#endif

#endif	/* AIRPIC_TIMER_H */

