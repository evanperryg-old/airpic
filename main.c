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
#include "airpic-libs/airpic-timer.h"
#include "airpic-libs/airpic-statusled.h"
#include "airpic-libs/airpic-i2c.h"
#include "airpic-libs/airpic-gyro.h"

void airpic_timer_isr(void)
{
    if(i2c_error_buscollision)
        statusLED_setStatus(STATUSLED_FASTBLINK | STATUSLED_ORANGE);
    
    airpic_timer_isr_exit;              //THIS MUST BE AT THE END OF THE ISR IN ORDER FOR IT TO WORK!
}

int main(void)
{
    // When using the airpic libraries, this should ALWAYS be the first thing you do.
    // It sets the clock cycle to 16MHz, disables the analog-to-digital converters
    // on the I/O pins, and makes sure pins are correctly configured as inputs/outputs.
    airpic_config();                
    
    // Set up the airpic-timer library's features. The macros used will enable the 
    // interrupt shown at the top of the code, and tell that interrupt to trigger every 
    // 100 milliseconds. Using the 'AIRPIC_TIMER_DEFAULT' macro will do the exact same 
    // thing, but doing it this way shows how to use bitwise OR operators to append 
    // the macros together.
    timer_config(AIRPIC_TIMER_INTERRUPT_ENABLE | AIRPIC_TIMER_PERIOD_100MS);
    
    // Set up the airpic-i2c library's features. You can select a 400kHz baudrate using 
    // the 'AIRPIC_I2C_BAUDRATE_100K' macro, or you can select a 100kHz baudrate using
    // the 'AIRPIC_I2C_BAUDRATE_400K' macro.
    i2c_config(AIRPIC_I2C_BAUDRATE_400K);
    
    // Enable the status RGB LED feature. If the LED is connected properly, and 
    // everything in code works as expected, the LED will show short, blue flashes
    // with about a second between each blink.
    statusLED_enable();
    
    // Send out configuration messages to Gyro 1. Cannot be done before running i2c_config().
    // This should always be done in the initialization section of the code, to ensure
    // the gyro will behave the way we want it to.
    gyro1_init();
    
    // Send out configuration messages to Gyro 2. Cannot be done before running i2c_config().
    // This should always be done in the initialization section of the code, to ensure
    // the gyro will behave the way we want it to.
    gyro2_init();
    
    while(1)
    {
        
    }
    
    return 0;
}
