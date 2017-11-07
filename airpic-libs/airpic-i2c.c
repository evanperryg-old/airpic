#include "airpic-i2c.h"
#include <p24Fxxxx.h>
#include "xc.h"

inline void i2c_trn_2(unsigned short deviceId, unsigned short readNotWrite, unsigned short byte_1, unsigned short byte_2)
{
    I2C2CONbits.SEN = 1;
    while(I2C2CONbits.SEN);
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = ( (deviceId << 1) + (readNotWrite & 0b1) );
    while(!IFS3bits.MI2C2IF);
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = byte_1;
    while(!IFS3bits.MI2C2IF);
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = byte_2;
    while(!IFS3bits.MI2C2IF);
    IFS3bits.MI2C2IF = 0;
    
    I2C2CONbits.PEN = 1;
    while(I2C2CONbits.PEN == 1);
}

void i2c_config(unsigned int baudrate)
{
    I2C2CON = 0x0000;
    I2C2BRG = baudrate;
    
    I2C2CON &= ~(1 << 11);          //IMPIEN = 0, datasheet says so bc master mode
    
    I2C2CONbits.I2CEN = 1;
    
    IFS3bits.MI2C2IF  = 0;          //Make sure flag is off again
    
}
