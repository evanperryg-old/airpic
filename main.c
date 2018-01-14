/**
 * @file    main.c
 * @brief   Sample main
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

// including airpic.h will include all the other airpic libraries, as well.
// not ideal if some libraries aren't being used, but it doesn't really matter
// since all the libraries are going to be used at one point or another in the 
// main part of the code.
#include "airpic.h"

int xValue;

// This is an interrupt service routine, or ISR for short. This particular one 
// is the Timer 5 ISR, meaning it triggers each time the fifth timer peripheral 
// on the PIC microcontroller reaches its maximum value. You can enable/disable
// this ISR, and set the time interval at which it triggers, using the 
// timer_config() function. More detail on how to do this can be found further 
// down in the code, and in the airpic-timer.h documentation.

void airpic_timer_isr(void)
{
    if(i2c_error_buscollision)
    {
       statusLED_setStatus(STATUSLED_FASTBLINK | STATUSLED_ORANGE);
       //currentColor = STATUSLED_ORANGE;
    }
    
    else if(serial_rcvbuffer_error_overflow)
    {
        statusLED_setStatus(STATUSLED_FASTBLINK | STATUSLED_RED);
        //currentColor = STATUSLED_RED;
    }
    
    gyro1_accumulate();
    
    airpic_timer_isr_exit;              // THIS MUST BE SOMEWHERE IN THE ISR IN ORDER FOR IT TO WORK!
                                        // its location in the ISR doesn't actually matter, but it's 
                                        // generally considered good practice to have it at the end.
}

int main(void)
{
    // When using the airpic libraries, this should ALWAYS be the first thing you do.
    // It sets the clock cycle to 16MHz, disables the analog-to-digital converters
    // on the I/O pins, and makes sure pins are correctly configured as inputs/outputs.
    // It also does some initial setup for the debugger serial port, but it does not
    // enable the debugger serial port.
    airpic_config();
    
    // enable the debugger serial port. 38400 baud is a safe default Other common 
    // baudrates (which all have macros of the format "DEBUGGER_BAUDRATE_xxx") 
    // are 4800, 9600, 14400, 19200, 28800, and 115200.
    airpic_debugger_enable(DEBUGGER_BAUDRATE_38400);
    
    // Set up the airpic-timer library's features. The macros used will enable the 
    // interrupt shown at the top of the code, and tell that interrupt to trigger every 
    // 100 milliseconds. Using the 'AIRPIC_TIMER_DEFAULT' macro will do the exact same 
    // thing, but doing it this way shows how to use bitwise OR operators to append 
    // the macros together.
    timer_config(AIRPIC_TIMER_INTERRUPT_ENABLE | AIRPIC_TIMER_PERIOD_20MS);
    
    // Set up the airpic-i2c library's features. You can select a 400kHz baudrate using 
    // the 'AIRPIC_I2C_BAUDRATE_100K' macro, or you can select a 100kHz baudrate using
    // the 'AIRPIC_I2C_BAUDRATE_400K' macro. This MUST be done before any of the i2c
    // peripherals are initialized.
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
    // infinite while() loop.
    motorController_init(50.0);

    // Configure UART1 to receive TTL-formatted serial data from the GPS. 
    // Unlike the initialization routines for the I2C devices, it is fine to 
    // run this if the GPS isn't actually connected to the microcontroller.
    serialGPS_config();
    
    while(1)
    {
        motor_write(15, 300); //500 is max, 250 is min
        
        gyro1_refresh();
        xValue = gyro1_getX();

       

        // we will compare lastCount to serialGPS_readoutCount() whenever we want to 
        // decide whether we need to run serialGPS_parse().
        unsigned int lastCount = 0;
    
    
        // the value returned by serialGPS_readoutCount() increments by 1 each 
        // time the GPS has sent us a new set of satellite data. By comparing
        // the output of this function to its previous value (the previous value 
        // being stored by our lastCount variable), we can determine when new
        // data needs to be parsed.
        
        // check to see if the GPS has given us new data, and parse it if there 
        // is a new update. Hypothetically, you could also have other conditions 
        // to decide when to parse the updated data.
        // You should parse data no more than once per update. This is 
        // because the parsing process is very complicated, and takes up a lot 
        // of instruction cycles. In essence, if you don't need the new data, 
        // don't parse it.
        if(lastCount < (serialGPS_readoutCount()))
        {
            // set lastCount to be equal to the number of readouts.
            lastCount = serialGPS_readoutCount();
            
            // the GPS gave us new data! before we can use the new data, we have 
            // to tell the MCU to parse the data. Running serialGPS_parse() will 
            // update the values returned by these functions: gpsTime_seconds(),
            // gpsTime_min(), gpsTime_hours(), gpsLatitude(), gpsLongitude(), 
            // gpsAltitude(), gpsLatitudeDirection(), gpsLongitudeDirection()
            serialGPS_parse();
            
            // if the serial debugger port is enabled, print "parsed some data!"
            // followed by a newline character. The '17' is the length of the 
            // string you want to print.
            // ANY TIME YOU USE A COMMAND THAT PRINTS TO THE DEBUGGER PORT, YOU
            // SHOULD MAKE SURE THE DEBUGGER PORT IS ACTUALLY ENABLED FIRST!
            // Q: "But Evan, why not just make those functions check if the port 
            //     is enabled automatically?"
            // A: "Because it takes about 5-15 instruction cycles to enter the 
            //     function, realize it doesn't need to do anything, and return. 
            //     however, by doing the check before entering the function, 
            //     it will only take 2-3 instruction cycles."
            if( airpic_debugger_isenabled ) airpic_debugger_println("parsed some data!", 17);

        }

        if(xValue > 0)
        {
            statusLED_setStatus(STATUSLED_SOLID | STATUSLED_GREEN);
        }
        else
        {
            statusLED_setStatus(STATUSLED_SOLID | STATUSLED_RED);
        }

    }
    
    return 0;
}

