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
 * Airpic uses a variety of peripherals on the PIC device. airpic-timer.h uses 
 * Timer peripherals 1, 4, and 5. airpic-statusled.h uses Timer peripheral 2. 
 * airpic-i2c.h uses the I2C2 peripheral.
 * 
 * \subsection techinfo Compiler/Technical Information
 * Airpic was compiled using the XC16 compiler, version 1.32B. The IDE used was 
 * MPLAB X, version 4.00.
 * 
 */

/**
 * @file airpic.h
 * @brief Configure microcontroller to use Airpic.
 * Including this library in your source code will add all of the 
 * libraries in the airpic-libs directory. airpic.h consists of functions that 
 * are used to configure the PIC device to the specifications needed for the 
 * rest of the Airpic libraries to work.
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
     */
    void airpic_config(void);


#ifdef	__cplusplus
}
#endif

#endif	/* AIRPIC_H */

