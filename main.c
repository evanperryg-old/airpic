/*
 * File:   main.c
 * Author: evan
 *
 * Created on September 26, 2017, 2:34 PM
 */

//CW1
#pragma config ICS = PGx1       //comm channel select
#pragma config FWDTEN = OFF     //watchdog timer enable
#pragma config GWRP = OFF       //general code segment write protect
#pragma config GCP = OFF        //general code segment code protect
#pragma config JTAGEN = OFF     //JTAG port enable

//CW2
#pragma config I2C1SEL = PRI    //I2C pin location select (default)
#pragma config IOL1WAY = OFF    //IOLOCK protection
#pragma config OSCIOFNC = ON    //Primary oscillator IO
#pragma config FCKSM = CSECME   //clock switching and monitor (enabled)
#pragma config FNOSC = FRCPLL   //oscillator select


#include "xc.h"
#include <p24Fxxxx.h>

#include "airpic.h"

int i = 0;
void airpic_timer_isr(void)
{
    
    airpic_timer_isr_exit;          //THIS MUST BE AT THE END OF THE ISR!!!!
}

int main(void)
{
    airpic_config();
    
    timer_config(AIRPIC_TIMER_INTERRUPT_ENABLE | AIRPIC_TIMER_PERIOD_500MS);
    
    enable_statusLED();
    
    led_setStatus(STATUSLED_LONGBLINK | STATUSLED_RED);
    
    
    while(1)
    {
        sleep_msec(100);
    }
    
    return 0;
}
