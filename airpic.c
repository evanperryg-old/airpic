#include "airpic.h"

/**
 * Configure the microcontroller to be used with the Airpic libraries.
 */
void airpic_config(void)
{
    CLKDIVbits.RCDIV = 0; //RCDIV = 1:1 (default 2:1)
    AD1PCFG = 0x9fff;
    TRISA   = 0x0000;
    TRISB   = 0x0003;
        
}
