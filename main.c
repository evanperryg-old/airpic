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
#include "airpic-libs/i2c-peripherals/airpic-gyro.h"
#include "airpic-libs/airpic-serialgps.h"


//Stuff I added ================================================================

int currentColor;
int xValue;


void airpic_timer_isr(void)
{
    if(i2c_error_buscollision)
    {
       statusLED_setStatus(STATUSLED_FASTBLINK | STATUSLED_ORANGE);
       currentColor = STATUSLED_ORANGE;
    }
    
    else if(serial_rcvbuffer_error_overflow)
    {
        statusLED_setStatus(STATUSLED_FASTBLINK | STATUSLED_RED);
        currentColor = STATUSLED_RED;
    }
    
//    else if(serial_receiver_active)
//    {
//       statusLED_setStatus(STATUSLED_SOLID | STATUSLED_MAGENTA);
//       allGood = 0;
//       currentColor = STATUSLED_MAGENTA;
//    }
    
    
 //   else
 //       allGood = 1;
     //   statusLED_setStatus(STATUSLED_LONGBLINK | STATUSLED_BLUE);
    
   
 
    
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
    timer_config(AIRPIC_TIMER_INTERRUPT_ENABLE | AIRPIC_TIMER_PERIOD_20MS);
    
    // Set up the airpic-i2c library's features. You can select a 400kHz baudrate using 
    // the 'AIRPIC_I2C_BAUDRATE_100K' macro, or you can select a 100kHz baudrate using
    // the 'AIRPIC_I2C_BAUDRATE_400K' macro.
    i2c_config(AIRPIC_I2C_BAUDRATE_400K);
    
    // Enable the status RGB LED feature. If the LED is connected properly, and 
    // everything in code works as expected, the LED will show long, blue flashes
    // with about a second between each blink.
    statusLED_enable();
    
    // Send out configuration messages to Gyro 1. Cannot be done before running i2c_config().
    // This should always be done in the initialization section of the code, to ensure
    // the gyro will behave the way we want it to. If the gyro is not connected, trying to 
    // initialize it may result in the microcontroller hanging in an infinite while() loop,
    // so don't initialize Gyros that aren't actually rhere.
    gyro1_init();
    //gyro1_calibrate();
    
    // Send out configuration messages to Gyro 2. Cannot be done before running i2c_config().
    // This should always be done in the initialization section of the code, to ensure
    // the gyro will behave the way we want it to. If the gyro is not connected, trying to 
    // initialize it may result in the microcontroller hanging in an infinite while() loop,
    // so don't initialize Gyros that aren't actually rhere.
    gyro2_init();
    
    // Configure UART1 to receive TTL-formatted serial data from the GPS.
    serialGPS_config();
    
    
    motorController_init(50.0);
    
    

    
    
    
    
    while(1)
    {
        motor_write(15, 300); //500 is max, 250 is min
        
        gyro1_refresh();
        xValue = gyro1_getX();
        
        
        if(xValue < 0)
        {
            if(currentColor != STATUSLED_GREEN)
                statusLED_setStatus(STATUSLED_SOLID | STATUSLED_GREEN);
        }
        else
        {
            if(currentColor != STATUSLED_TEAL)
                statusLED_setStatus(STATUSLED_SOLID | STATUSLED_TEAL);
        }

    }
    
    return 0;
}

