#include <p24Fxxxx.h>
#include <math.h>
#include "./../airpic-i2c.h"

#include "airpic-motor.h"

void motorController_init(float pwmFrequency)
{
    pwmFrequency *= 0.9;            // noted in Adafruit library as a bugfix related
                                    // to frequency overshoot
    float prescaleVal = 6103.51563; // most precise: 6103.515625 = (25,000,000 / 4096)
    prescaleVal /= pwmFrequency;
    --prescaleVal;
    
    unsigned int prescaler = floor(prescaleVal + 0.5);
    
    prescaler = (prescaler & 0xff); // truncate to 8 bits. Any frequency that
                                    // meets the datasheet specs will not be
                                    // affected by this.
    
    // grab the current value stored in the MODE1 register on the motor driver
    unsigned int oldMode;
    
    i2c_start_write(ADDR_MOTORCONTROLLER);
    i2c_transmit(0x0);
    i2c_stop();
    
    i2c_start_read(ADDR_MOTORCONTROLLER);
    oldMode = i2c_lastReceive();
    i2c_stop();
    
    // put the motor driver to sleep
    unsigned int newMode;
    newMode = (oldMode & 0x7f) | 0x10;
    
    i2c_start_write(ADDR_MOTORCONTROLLER);
    i2c_transmit(0x0);
    i2c_transmit(newMode);
    i2c_stop();
    
    // set the prescaler
    i2c_start_write(ADDR_MOTORCONTROLLER);
    i2c_transmit(0xFE);
    i2c_transmit(prescaler);
    i2c_stop();
    
    // wake up the motor driver again
    i2c_start_write(ADDR_MOTORCONTROLLER);
    i2c_transmit(0x0);
    i2c_transmit(oldMode);
    i2c_stop();
    
    // wait for 500us, we could use #7999 to be at exactly 500us,
    // but just to be safe
    asm("repeat #8000");
    asm("nop");
    
    // set up auto-incrementing on writes. This allows us to write to all 4
    // registers relating to a motor in a single I2C transmission.
    i2c_start_write(ADDR_MOTORCONTROLLER);
    i2c_transmit(0x0);
    i2c_transmit( oldMode | 0xa1 );
    
}

void motor_write(unsigned int motor, unsigned int speed)
{
    speed = speed % 4096;
    if(speed == 0) speed = 4096;        // special case for "off"
    
    i2c_start_write(ADDR_MOTORCONTROLLER);
    i2c_transmit(0x6 + (4 * motor));
    i2c_transmit( 0x0 );
    i2c_transmit( 0x0 );
    i2c_transmit( speed );
    i2c_transmit( speed >> 8 );
    i2c_stop();
    
}

void motor_setPWM(unsigned int motor, unsigned int delay, unsigned int duty)
{
    delay = delay % 4096;
    unsigned int off = (duty + delay);
    
    i2c_start_write(ADDR_MOTORCONTROLLER);
    i2c_transmit(0x6 + (4 * motor));
    i2c_transmit( delay );
    i2c_transmit( delay >> 8 );
    i2c_transmit( off );
    i2c_transmit( off >> 8 );
    i2c_stop();
    
}

void motor_stop(unsigned int motor)
{
    i2c_start_write(ADDR_MOTORCONTROLLER);
    i2c_transmit(0x6 + (4 * motor));
    i2c_transmit(0x00);
    i2c_transmit(0x00);
    i2c_transmit(0x00);
    i2c_transmit(0x10);
    i2c_stop();
    
}

/*
 * 
 * setPWM(num, on, off) pseudocode:
 * - begin transmission to an address
 * - write (0x6 + 4 * [num])
 * - write [on] lower byte
 * - write [on] higher byte ([on] >> 8)
 * - write [off] lower byte
 * - write [off] higher byte ([off] >> 8)
 * - end transmission
 * 
 * LED_ON  = ((desired delay %) * 4096) - 1
 * LED_OFF = ((desired duty %) * 4096) + LED_ON - 1
 * 
 */