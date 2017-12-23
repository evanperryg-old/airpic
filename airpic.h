/*! \mainpage Airpic Library Documentation
 * 
 * \section intro_sec Introduction
 * The Airpic Libraries include various tools to simplify and accelerate the 
 * development of C code for the PIC24 microcontroller. Airpic was designed for 
 * use with Project IRIS, a project created by Innovative Engineers at 
 * the University of Minnesota Twin Cities.
 * 
 * \subsection microcontroller Microcontroller
 * These libraries were written for the PIC24FJ64GA002 microcontroller, but 
 * they are theoretically compatible with any PIC24FJ device. It should be 
 * relatively easy to adapt these libraries for use on virtually any PIC24 
 * microcontroller.
 * <p>
 * Airpic uses a variety of peripherals and pins on the PIC device. 
 * <p><ul>
 * <li> airpic-timer.h uses Timer peripherals 1, 4, and 5. 
 * <li> airpic-statusled.h uses Timer peripheral 2, and pins RB15, RB14, and 
 * RB13 (physical pins 26, 25, and 24, respectively).
 * <li> airpic-i2c.h uses the I2C2 peripheral, and pins SDA2 and SCL2 (physical 
 * pins 6 and 7, respectively)
 * <li> airpic-serialgps.h uses the UART1 peripheral, and pins RP10 and RP11 
 * (physical pins 21 and 22).
 * <p></ul>
 * 
 * \subsection techinfo Compiler/Technical Information
 * Airpic was compiled using the XC16 compiler, version 1.32B. The IDE used was 
 * MPLAB X, version 4.00.
 * 
 */

/**
 * @file airpic.h
 * @brief Configure microcontroller to use Airpic.
 * airpic.h consists of functions that are used to configure the PIC 
 * microcontroller to the specifications needed for the rest of the Airpic 
 * libraries to work.
 * @author Evan Perry Grove
 * @date September 26, 2017
 */

#ifndef AIRPIC_H
#define	AIRPIC_H


#ifdef	__cplusplus
extern "C" {
#endif
    
    /**
     * Configure the microcontroller for the Airpic library. 
     * Running airpic_config will do the following:
     * <p><ul>
     * <li> Set the microcontroller RC clock divider to 1:1 (16MHz, assuming 
     * a PIC24F device using the internal oscillator)
     * <li> Set all pins to be digital, by disconnecting them from the Analog-to
     * -Digital Converter.
     * <li> Set all RAx pins to be digital outputs.
     * <li> Set RB0, RB1, RB11, and RB12 to be digital inputs. All other RBx 
     * pins are digital outputs.
     * <li> Set the UART1 peripheral's TX pin to pin RP10
     * <li> Set the UART1 peripheral's RX pin to pin RP11
     * </ul><p>
     */
    void airpic_config(void);


#ifdef	__cplusplus
}
#endif

#endif	/* AIRPIC_H */

