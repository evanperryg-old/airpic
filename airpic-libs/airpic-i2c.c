#include "xc.h"
#include <p24Fxxxx.h>

void I2C2_init(void)
{
    I2C2CON |=  (1 << 15);          //I2CEN bit = 1
    I2C2CON &= ~(1 << 11);          //IMPIEN = 0, datasheet says so bc master mode
    
}

void I2C2_reset(void)
{
    //set ACKEN, PEN, SEN, RCEN, RSEN low
    I2C2CON = (I2C2CON & (~0x001f)) | (0 << 0);
    
}

void I2C2_waitFlag()
{
    while(!IFS3bits.MI2C2IF);       //flag goes high at the end of various I2C
    IFS3bits.MI2C2IF = 0;           //  operations
    
}

void I2C2_waitForACK(void)
{
    while(I2C2STATbits.ACKSTAT);    //ACKSTAT goes low when a device sends its
    I2C2STATbits.ACKSTAT = 1;       //  acknowledgement. Pretty sure we have to
                                    //  manually set it again afterwards.
}

void I2C2_transmit2(unsigned char writeAddr, unsigned char toWrite)
{
    I2C2_reset();           //reset everything, just to be safe
    I2C2CON |=  (1 << 0);   //setting this bit begins the Start sequence (SEN)
    I2C2_waitFlag();        //wait for start to finish
    
    I2C2TRN = writeAddr;    //write the transmit address to transmit reg
    I2C2_waitFlag();        //wait for transmission process to finish
    I2C2_waitForACK();      //wait for slave acknowledge
    
    I2C2TRN = toWrite;      //write the word to transmit reg
    I2C2_waitFlag();        //wait for transmission process to finish
    I2C2_waitForACK();      //wait for slave acknowledge
    
    I2C2_reset();           //reset important bits
    I2C2CON |=  (1 << 2);   //setting this bit begins the stoP sequence (PEN)
    I2C2_waitFlag();        //wait for stoP sequence to end
    
}

void I2C2_transmit3(unsigned char writeAddr, unsigned char writeAddrReg, unsigned char toWrite)
{
    I2C2_reset();           //reset everything, just to be safe
    I2C2CON |=  (1 << 0);   //setting this bit begins the Start sequence (SEN)
    I2C2_waitFlag();        //wait for start to finish
    
    I2C2TRN = writeAddr;    //write the transmit address to transmit reg
    I2C2_waitFlag();        //wait for transmission process to finish
    I2C2_waitForACK();      //wait for slave acknowledge
    
    I2C2TRN = writeAddrReg; //write the word address to transmit reg
    I2C2_waitFlag();        //wait for transmission process to finish
    I2C2_waitForACK();      //wait for slave acknowledge
    
    I2C2TRN = toWrite;      //write the word to transmit reg
    I2C2_waitFlag();        //wait for transmission process to finish
    I2C2_waitForACK();      //wait for slave acknowledge
    
    I2C2_reset();           //reset important bits
    I2C2CON |=  (1 << 2);   //setting this bit begins the stoP sequence (PEN)
    I2C2_waitFlag();        //wait for stoP sequence to end
    
}

unsigned char I2C2_receive(unsigned char readAddr)
{
    I2C2_reset();           //reset everything, just to be safe
    I2C2CON |=  (1 << 3);   //set RCEN
    I2C2CON |=  (1 << 0);   //set SEN
    I2C2_waitFlag();        //wait for SEN process to complete
    
    I2C2TRN = readAddr;     //write the transmit address to transmit reg
    I2C2_waitFlag();        //wait for transmission process to finish
    I2C2_waitForACK();      //wait for slave acknowledge
    
    while(!I2C2STATbits.RBF);
    
    return I2C2RCV;         //reading out of I2C2RCV clears I2C2STAT_RBF
}
