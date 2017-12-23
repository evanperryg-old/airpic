/**
 * @file    main.c
 * @brief   Sample code
 * The main.c included in this library shows examples of the usage of the Airpic 
 * libraries. Heavily annotated to help walk the user through the libraries.
 * @author  Evan Perry Grove
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
#include "airpic-libs/i2c-peripherals/airpic-motor.h"
#include "airpic-libs/airpic-serialgps.h"

void airpic_timer_isr(void)
{
    if(i2c_error_buscollision)
        statusLED_setStatus(STATUSLED_FASTBLINK | STATUSLED_ORANGE);
    
    else if(serial_rcvbuffer_error_overflow)
        statusLED_setStatus(STATUSLED_FASTBLINK | STATUSLED_RED);
    
    else if(serial_receiver_active)
        statusLED_setStatus(STATUSLED_SOLID | STATUSLED_MAGENTA);
    
    else
        statusLED_setStatus(STATUSLED_LONGBLINK | STATUSLED_BLUE);
    
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
    // everything in code works as expected, the LED will show long, blue flashes
    // with about a second between each blink.
    statusLED_enable();
    
    // Send out configuration messages to Gyro 1. Cannot be done before running i2c_config().
    // This should always be done in the initialization section of the code, to ensure
    // the gyro will behave the way we want it to. If the gyro is not connected, trying to 
    // initialize it may result in the microcontroller hanging in an infinite while() loop,
    // so don't initialize Gyros that aren't actually there.
    gyro1_init();
    
    // Send out configuration messages to Gyro 2. Cannot be done before running i2c_config().
    // This should always be done in the initialization section of the code, to ensure
    // the gyro will behave the way we want it to. If the gyro is not connected, trying to 
    // initialize it may result in the microcontroller hanging in an infinite while() loop,
    // so don't initialize Gyros that aren't actually there.
    gyro2_init();
    
    // Send out configuration messages to the motor controller. To ensure that the PWM 
    // frequency is passed as a floating-point value, make sure there's a decimal point
    // in the number. XC16 tends to be a little unforgiving with type discrepancies,
    // so this is very important. Just like the gyros, if the motor controller is not actually
    // attached to the I2C bus, trying to initialize it will make your program hang in an 
    // infinite while() loop.)
    motorController_init(50.0);
    
    // Configure UART1 to receive TTL-formatted serial data from the GPS. It is fine to 
    // run this if the GPS isn't connected to the microcontroller.
    serialGPS_config();
    
    // we will compare lastCount to serialGPS_readoutCount() whenever we want to 
    // decide whether we need to run serialGPS_parse().
    unsigned int lastCount = 0;
    
    while(1)
    {
        
        // check to see if the GPS has given us new data, and parse it if there 
        // is a new update. Hypothetically, you could also have other conditions 
        // to decide when to parse the updated data. You should only parse data
        // once per update, although parsing the same data multiple times doesn't
        // hurt anything.
        if(lastCount != serialGPS_readoutCount())
        {
            // the GPS gave us new data! before we can use the new data, we have 
            // to tell the MCU to parse the data. Running serialGPS_parse() will 
            // update the values returned by these functions: gpsTime_seconds(),
            // gpsTime_min(), gpsTime_hours(), gpsLatitude(), gpsLongitude(), 
            // gpsAltitude(), gpsLatitudeDirection(), gpsLongitudeDirection()
            serialGPS_parse();
            
            // set lastCount to be equal to the number of readouts.
            lastCount = serialGPS_readoutCount();
            
        }
        
    }
    
    return 0;
}
