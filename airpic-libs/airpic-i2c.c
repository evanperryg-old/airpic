#include <p24Fxxxx.h>
#include "airpic-i2c.h"

void i2c_config(unsigned int baudrate)
{
    I2C2CON = 0x0000;
    I2C2BRG = baudrate;
    
    I2C2CON = 0x8000;
    
    IFS3bits.MI2C2IF  = 0;          //Make sure flag is off again
    
}
