/**
 * @file    airpic-gyro.h
 * @brief   Utilities for communicating with Gyro sensors.
 * <p>
 * This library uses the functions included in airpic-i2c.h to facilitate 
 * communication between the PIC microcontroller and two L3GD20H gyro  
 * sensors. This library is hypothetically compatible with L3GD20-based devices, 
 * as well, though this was not tested.
 * <p>
 * The L3GD20H allows the user to select between one of two 7-bit I2C device 
 * addresses, @c 0x6B and @c 0x6A. To make code more readable, gyros are simply 
 * referred to as 'gyro1' and 'gyro2.'
 * <p><ul>
 * <li> gyro1 uses I2C address @c 0x6B (@c 0x6B=0b1101011, read byte @c 0xD7, write byte @c 0xD6)
 * <li> gyro2 uses I2C address @c 0x6A (@c 0x6A=0b1101010, read byte @c 0xD5, write byte @c 0xD4)
 * </ul><p>
 * @author  Evan Perry Grove 
 * @date    November 8, 2017
 * @see     airpic-i2c.h
 */

#ifndef AIRPIC_GYRO_H
#define	AIRPIC_GYRO_H

/** @brief A macro containing the I2C address of gyro 1. */
#define ADDR_GYRO_1 0b1101011

/** @brief A macro containing the I2C address of gyro 2. */
#define ADDR_GYRO_2 0b1101010

/** @brief represents the resolution of the gyro. Can be 245, 500, or 2000. */
#define DPS_RANGE                   245.0

#include "xc.h"
#include <p24Fxxxx.h>

#ifdef	__cplusplus
extern "C" {
#endif
    
    /**
     * Used internally to organize variables.
     */
    struct Gyroscope{
        int xVel;
        int yVel;
        int zVel;
        
        double x;
        double y;
        double z;
    };
    
    /**
     * Initialize L3GD20H gyro with I2C address \c 1101011.
     */
    void gyro1_init(void);
    
    /**
     * Initialize L3GH20H gyro with I2C address \c 1101010.
     */
    void gyro2_init(void);
    
    /**
     * Retrieve new X, Y, and Z values from Gyro 1.
     */
    void gyro1_refresh(void);
    
    /**
     * Retrieve new X, Y, and Z values from Gyro 2.
     */
    void gyro2_refresh(void);
    
    /**
     * Run the accumulation procedure for Gyro 1.
     */
    void gyro1_accumulate(void);
    
    /**
     * Run the accumulation procedure for Gyro 2.
     */
    void gyro2_accumulate(void);  
    
    /**
     * Get the last X value retrieved from Gyro 1.
     * @return the Gyro 1 X value with respect to the calibration setpoint.
     */
    int gyro1_getX(void);
    
    /**
     * Get the last Y value retrieved from Gyro 1.
     * @return the Gyro 1 Y value with respect to the calibration setpoint.
     */
    int gyro1_getY(void);
    
    /**
     * Get the last Z value retrieved from Gyro 1.
     * @return the Gyro 1 Z value with respect to the calibration setpoint.
     */
    int gyro1_getZ(void);
    
    /**
     * Get the last X value retrieved from Gyro 2.
     * @return the Gyro 2 X value with respect to the calibration setpoint.
     */
    int gyro2_getX(void);
    
    /**
     * Get the last Y value retrieved from Gyro 2.
     * @return the Gyro 2 Y value with respect to the calibration setpoint.
     */
    int gyro2_getY(void);
    
    /**
     * Get the last Z value retrieved from Gyro 2.
     * @return the Gyro 2 Z value with respect to the calibration setpoint.
     */
    int gyro2_getZ(void);
    
    

#ifdef	__cplusplus
}
#endif

#endif	/* AIRPIC_GYRO_H */

