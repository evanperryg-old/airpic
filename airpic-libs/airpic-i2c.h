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
     * 
     * @param deviceId
     * @param readNotWrite
     * @param byte_1
     * @param byte_2
     */
    inline void i2c_trn_2(unsigned short deviceId, unsigned short readNotWrite, unsigned short byte_1, unsigned short byte_2);
    
    /**
     * 
     * @param baudrate the 16-bit value to be written into I2C2BRG. The AIRPIC_I2C_BAUDRATE_xxxx
     * macros defined at the top of airpic-i2c.h make this a lot easier.
     */
    void i2c_config(unsigned int baudrate);
    
    /**
     * 
     * @param motorId
     * @param value
     */
    void setMotor(unsigned short motorId, unsigned short value);

#ifdef	__cplusplus
}
#endif


#endif	/* AIRPIC_I2C_H */

