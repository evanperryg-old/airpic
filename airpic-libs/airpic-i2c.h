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
#include <i2c.h>

#define AIRPIC_I2C_BAUDRATE_400K 0x0025
#define AIRPIC_I2C_BAUDRATE_100K 0x009D

#define ADDR_GYRO_1
#define ADDR_GYRO_2
#define ADDR_ACCELEROMETER
#define ADDR_MOTORDRIVER

#ifdef	__cplusplus
extern "C" {
#endif
    
    /**
     * Send a "start" signal on the I2C2 bus.
     */
    static inline void i2c_start()
    {
        I2C2CONbits.SEN = 1;    
        while(I2C2CONbits.SEN);
        IFS3bits.MI2C2IF = 0;
        
    }
    
    static inline void i2c_stop()
    {
        I2C2CONbits.PEN = 1;
        while(I2C2CONbits.PEN);
        IFS3bits.MI2C2IF = 0;
        
    }
    
    /**
     * Transmit a 7-bit slave device address with a read bit.
     * @param slaveAddr The 7-bit slave device address.
     */
    static inline void i2c_init_read(unsigned short slaveAddr)
    {
        I2C2TRN = (slaveAddr << 1) + 1;
        while(!IFS3bits.MI2C2IF);
        IFS3bits.MI2C2IF = 0;
        
    }
    
    /**
     * Transmit a 7-bit slave device address with a write bit.
     * @param slaveAddr The 7-bit slave device address.
     */
    static inline void i2c_init_write(unsigned short slaveAddr)
    {
        I2C2TRN = (slaveAddr << 1);
        while(!IFS3bits.MI2C2IF);
        IFS3bits.MI2C2IF = 0;
        
    }
    
    /**
     * Transmit a byte on the I2C bus.
     * @param byte  The byte to be sent.
     */
    static inline void i2c_transmit(unsigned short byte)
    {
        I2C2TRN = byte;
        while(!IFS3bits.MI2C2IF);
        IFS3bits.MI2C2IF = 0;
        
    }
    
    static inline unsigned short i2c_receive(void)
    {
        I2C2CONbits.RCEN = 1;
        while(!IFS3bits.MI2C2IF);
        IFS3bits.MI2C2IF = 0;
        
        unsigned short out = I2C2RCV;
        // if we are doing some kind of check, do it here
        
        I2C2CONbits.ACKEN = 1;
        while(!IFS3bits.MI2C2IF);
        IFS3bits.MI2C2IF = 0;
        
        return out;
    }
    
    /**
     * Do the necessary configuration to set up the I2C2 peripheral and attached devices.
     * @param baudrate the 16-bit value to be written into I2C2BRG. The AIRPIC_I2C_BAUDRATE_xxxx
     * macros defined at the top of airpic-i2c.h make this a lot easier.
     */
    void i2c_config(unsigned int baudrate);

#ifdef	__cplusplus
}
#endif


#endif	/* AIRPIC_I2C_H */

