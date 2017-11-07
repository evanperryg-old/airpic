/*! \mainpage Airpic Library Documentation
 * 
 * \section intro_sec Introduction
 * The Airpic Libraries include various tools to simplify and accelerate the 
 * development of C code for the PIC24 microcontroller. 
 * 
 * \subsection compatibility Compatibility
 * These libraries were written for the PIC24FJ64GA002 microcontroller, but 
 * they are theoretically compatible with any PIC24FJ device. It should be 
 * relatively easy to adapt these libraries for use on virtually any PIC24 
 * microcontroller.
 * 
 * \subsection techinfo Compiler/Technical Information
 * Airpic was compiled using the XC16 compiler, version 1.32B. The IDE used was 
 * MPLAB X, version 4.00.
 */

/**
 * @file airpic.h
 * @brief This is the base library for Airpic.
 * Including this library in your source code will add all of the 
 * libraries in the airpic-libs directory. The tools found in the airpic-libs 
 * folder are mostly functions that simplify the usage of many of the 
 * peripherals included on the PIC24. These libraries are tested on a 
 * PIC24FJ64GA002 microcontroller. I cannot guarantee that the Airpic libraries 
 * will work on other PIC24 devices without some small modifications.
 * @author Evan Perry Grove
 * @date 9/26/2017
 */

#ifndef AIRPIC_H
#define	AIRPIC_H

#include "airpic-libs/airpic-i2c.h"
#include "airpic-libs/airpic-timer.h"

#ifdef	__cplusplus
extern "C" {
#endif
    
    /**
     * ensure the device is configured to work with Airpic.
     * @return void
     */
    void airpic_config(void);
    



#ifdef	__cplusplus
}
#endif

#endif	/* AIRPIC_H */

