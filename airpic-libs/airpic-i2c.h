/**
 * @file airpic-i2c.h
 * @brief Communicate with devices connected to I2C2.
 * <p>
 * The functions included here are designed around master 
 * communication in a single-master environment, as this is what is required 
 * for the project the Airpic libraries were originally written for. I may 
 * eventually add some functions relating to slave communication, in an effort 
 * to increase the versatility of the library.
 * @author  Evan Perry Grove
 * @date    September 26, 2017
 */
#ifndef AIRPIC_I2C_H
#define	AIRPIC_I2C_H

#include "xc.h"
#include <p24Fxxxx.h>

/** @brief The value to be written into I2C2BRG for a 400kHz baudrate. */
#define AIRPIC_I2C_BAUDRATE_400K 0x0025
/** @brief The value to be written into I2C2BRG for a 100kHz baudrate. */
#define AIRPIC_I2C_BAUDRATE_100K 0x009D

/** @brief A macro for making the I2C2 bus collision status bit more readable. */
#define i2c_error_buscollision (I2C2STATbits.BCL)

#ifdef	__cplusplus
extern "C" {
#endif
    
    /**
     * Send a "stop" sequence on the I2C2 bus.
     */
    void i2c_stop();
    
    /**
     * Transmit a 7-bit slave device address with a read bit. The binary value 
     * sent by this function is equal to ((slaveAddr << 1) + 1). The function 
     * will return once the transmission is finished.
     * @param slaveAddr The 7-bit slave device address.
     */
    void i2c_start_read(unsigned short slaveAddr);
    
    /**
     * Transmit a 7-bit slave device address with a write bit. The binary value 
     * sent by this function is equal to ((slaveAddr << 1) + 0). The function 
     * will return once the transmission is finished.
     * @param slaveAddr The 7-bit slave device address.
     */
    void i2c_start_write(unsigned short slaveAddr);
    
    /**
     * Transmit a byte on the I2C bus.
     * @param byte  The byte to be sent.
     */
    void i2c_transmit(unsigned short byte);
    
    /**
     * Receive a byte from an I2C device.
     * @return the byte received from the I2C slave.
     */
    unsigned short i2c_receive(void);
    
    /**
     * Receive the last byte sent by an I2C slave.
     * @return the byte received from the I2C slave.
     */
    unsigned short i2c_lastReceive(void);
    
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

