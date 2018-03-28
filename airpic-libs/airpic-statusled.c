#include "airpic-statusled.h"
#include <p24FJ64GA002.h>

 unsigned int statled_accumulator, statled_on, statled_pr_on, statled_pr_off, statled_clr;
   
void __attribute__((__interrupt__,no_auto_psv))_T2Interrupt(void)
{
    if(statled_on)
    {
        LATB = (LATB & 0x1fff) | (statled_clr);
                
        if(statled_accumulator < statled_pr_on) ++statled_accumulator;
        else
        {
            statled_accumulator = 0;
            statled_on = 0;
        }
    }
    else
    {
        LATB = (LATB & 0x1fff);
        
        if(statled_accumulator < statled_pr_off) ++statled_accumulator;
        else
        {
            statled_accumulator = 0;
            statled_on = 1;
        }
    }
    
    IFS0bits.T2IF = 0;
}

void statusLED_enable(void)
{
    statled_accumulator = 0;
    statled_on = 1;
    
    T2CON   = 0x0000;
    TMR2    = 0x0000;
    PR2     = 0x0096;       //statled is updated every 2.4ms
    
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;
    
    T2CON   = 0x8030;
    statusLED_setStatus(STATUSLED_BLUE | STATUSLED_LONGBLINK);
}

void statusLED_setStatus(unsigned int stat)
{
    statled_clr = ( stat & 0xf000 );
    
    switch( stat & (0x000f) )
    {
        case STATUSLED_OFF:
            statled_pr_on  = 0;
            statled_pr_off = 0xffff;
            statled_on = 0;
            break;
        case STATUSLED_SOLID:
            statled_pr_on  = 0xffff;
            statled_pr_off = 0;
            break;
        case STATUSLED_SHORTBLINK:
            statled_pr_on  = 40;
            statled_pr_off = 600;
            break;
        case STATUSLED_LONGBLINK:
            statled_pr_on  = 600;
            statled_pr_off = 100;
            break;
        case STATUSLED_FASTBLINK:
            statled_pr_on  = 60;
            statled_pr_off = 60;
            break;
    }
    
}