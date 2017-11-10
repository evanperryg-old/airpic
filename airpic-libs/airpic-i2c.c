#include <p24Fxxxx.h>

void i2c_stop()
{
    I2C2CONbits.PEN = 1;
    while(I2C2CONbits.PEN);
    IFS3bits.MI2C2IF = 0;
    //bus collision is occuring when we get to the above line after doing a set of
    //receive commands.

}

void i2c_start_read(unsigned short slaveAddr)
{
    I2C2CONbits.SEN = 1;    
    while(I2C2CONbits.SEN == 1);
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = (slaveAddr << 1) + 1;
    while(!IFS3bits.MI2C2IF);
    IFS3bits.MI2C2IF = 0;

}

void i2c_start_write(unsigned short slaveAddr)
{
    I2C2CONbits.SEN = 1;    
    while(I2C2CONbits.SEN == 1);
    IFS3bits.MI2C2IF = 0;
    
    I2C2TRN = (slaveAddr << 1);
    while (IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
}

void i2c_transmit(unsigned short byte)
{
    while(I2C2STATbits.TRSTAT);
    I2C2TRN = byte;
    
    while(I2C2STATbits.ACKSTAT);
    while(!IFS3bits.MI2C2IF);
    IFS3bits.MI2C2IF = 0;

}

unsigned short i2c_receive(void)
{
    while(I2C2STATbits.TRSTAT);
    
    I2C2CONbits.RCEN = 1;
    
    while(!I2C2STATbits.RBF);
    unsigned short out = I2C2RCV;
    I2C2STATbits.RBF = 0;       //should be cleared by hardware, but just to be safe

    I2C2CONbits.ACKEN = 1;
    while(I2C2CONbits.ACKEN);
    IFS3bits.MI2C2IF = 0;

    return out;
}

unsigned short i2c_lastReceive(void)
{
    while(I2C2STATbits.TRSTAT);
    
    I2C2CONbits.RCEN = 1;
    
    while(!I2C2STATbits.RBF);
    unsigned short out = I2C2RCV;
    I2C2STATbits.RBF = 0;       //should be cleared by hardware, but just to be safe

    I2C2CONbits.ACKDT = 1;
    I2C2CONbits.ACKEN = 1;
    while(I2C2CONbits.ACKEN);
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.ACKDT = 0;

    return out;
}

void i2c_config(unsigned int baudrate)
{
    I2C2CON = 0x0000;
    I2C2BRG = baudrate;
    
    IFS3bits.MI2C2IF  = 0;          //Make sure flag is off again
    
    I2C2CON = 0x8000;
    
}
