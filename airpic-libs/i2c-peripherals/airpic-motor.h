/**
 * @file    airpic-motor.h
 * @brief   Control Adafruit PWM servo driver using I2C.
 * airpic-motor.h uses the PIC24F microcontroller's I2C peripheral 2 to control 
 * the Adafruit 16-Channel 12-bit PWM servo driver (product ID# 815). This board 
 * is based on NXP Semiconductors' PCA9685 chip (Digikey part number 
 * 568-5931-1-ND).
 * <p>
 * This particular implementation uses the PCA9685 as a motor controller. The 
 * datasheet for the PCA9685 describes its use as an LED driver, but the basic 
 * operation of the device remains the same.
 * <p>
 * This library borrows heavily from Adafruit's PCA9685 library, available on 
 * GitHub as "Adafruit-PWM-Servo-Driver-Library."

 * @author  Evan Perry Grove
 * @date    November 23, 2017
 */

#ifndef AIRPIC_MOTOR_H
#define	AIRPIC_MOTOR_H

/** @brief The 7-bit I2C address for the motor controller board.
 * The MSB is fixed, and the next 6 bits can be set in hardware. A5 on the board
 * is the second-most significant bit in the address, A0 is the least significant
 * bit in the address. 
 */
#define ADDR_MOTORCONTROLLER 0b1000000

#include "xc.h"
#include <p24Fxxxx.h>

#ifdef	__cplusplus
extern "C" {
#endif
    
    /**
     * @brief Initialize the motor controller.
     * Initialize the I2C PWM motor controller to use the specified frequency
     * @param pwmFrequency The desired frequency of the PWM output, in Hz.
     */
    void motorController_init(float pwmFrequency);
    
    /**
     * Set the motor to the specified PWM duty cycle. This is an easier-to-use 
     * version of motor_setPWM() that still meets the normal requirements for 
     * controlling a motor.
     * @param motor The motor to be written to. Acceptable range: 0-15.
     * @param speed The duty cycle of the PWM output going to the motor. 
     * Acceptable range: 0-4095.
     */
    void motor_write(unsigned int motor, unsigned int speed);
    
    /**
     * @brief Set the specific PWM waveform to be delivered to the motor.
     * Set the PWM output for a motor to have the specified delay time (delay 
     * being the % of the cycle before the signal goes high) and the specified 
     * duty cycle (% of the cycle that the signal is high). Setting the duty 
     * variable to 4096 is effectively the same as the motor_stop() function.
     * <p>
     * Be careful! Unpredictable behavior can occur if (delay + duty) > 4096. 
     * Any problems like this can be prevented by simply keeping delay at zero, 
     * or another very low number.
     * @param The motor to be written to. Acceptable range: 0-15.
     * @param Delay before the signal goes high. Acceptable range: 0-4095.
     * @param Duty cycle of the signal. Duty cycle will be equal to (duty / 4095). 
     * Acceptable range: 0-(4095 - delay).
     */
    void motor_setPWM(unsigned int motor, unsigned int delay, unsigned int duty);
    
    /**
     * @brief Stop the specified motor.
     * Force the specified motor to be fully off by setting the LEDx_OFF_H[4] 
     * bit. More about how this works can be read on page 23-24 of the PCA9685 
     * datasheet. Writing a duty cycle to the motor motor_write() or 
     * motor_setPWM() will start the motor again.
     * @param motor
     */
    void motor_stop(unsigned int motor);
    
    



#ifdef	__cplusplus
}
#endif

#endif	/* AIRPIC_MOTOR_H */

