/**
 * @file airpic-i2c.h
 * @brief Airpic-i2c contains functions used to communicate with I2C devices 
 * using the PIC24 microcontroller.
 * The functions included here are mostly designed around master 
 * communication in a single-master environment, as this is what is required 
 * for the project the Airpic libraries were originally written for. I may 
 * eventually add some functions relating to slave communication, in an effort 
 * to increase the versatility of the library.
 * @author Evan Perry Grove
 * @date 9/26/2017
 */
#ifndef AIRPIC_I2C_H
#define	AIRPIC_I2C_H

#include "xc.h"
#include <p24Fxxxx.h>

#ifdef	__cplusplus
extern "C" {
#endif
    
    /** 
     * initialize the second I2C interface on the device.
     * @return void
     */
    void I2C2_init(void);
    
    /**
     * Reset important parameters in the I2C2 module to defaults.
     */
    void I2C2_reset(void);
    
    /**
     * Wait for the \c MI2C2IF flag to be triggered, then reset it.
     */
    void I2C2_waitFlag(void);
    
    /**
     * Wait for the acknowledgement signal from a device on the I2C2 bus.
     */
    void I2C2_waitForACK(void);
    
    /**
     * As a master, transmit one byte of data to a specified I2C device.
     * @param writeAddr the address of the I2C device you want to communicate with.
     * @param toWrite the byte given to that device as an instruction, data to store, etc.
     */
    void I2C2_transmit2(unsigned char writeAddr, unsigned char toWrite);
    
    /** 
     * As a master, transmit two bytes of data to a specified I2C device.
     * @param writeAddr the address of the I2C device you want to communicate with.
     * @param writeAddrReg the first byte of information sent to the device, usually an address to a particular register on that device.
     * @param toWrite the second byte of information sent to the device, usually the value you want to store in a given register on that device.
     */
    void I2C2_transmit3(unsigned char writeAddr, unsigned char writeAddrReg, unsigned char toWrite);
    
    /** 
     * As a master, receive data from the specified I2C device.
     * @param readAddr the address of the I2C device you want to communicate with.
     * @return an unsigned 16-bit value corresponding to the byte received from the I2C device.
     */
    unsigned char I2C2_receive(unsigned char readAddr);


#ifdef	__cplusplus
}
#endif


#endif	/* AIRPIC_I2C_H */

