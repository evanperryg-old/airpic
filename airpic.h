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
 * 
 * \subsection connections Connections
 * <p>
 * Airpic uses a variety of peripherals and pins on the PIC device.
 * <p><ul>
 * <li> airpic-timer.h uses Timer peripherals 1, 4, and 5. 
 * <li> airpic-statusled.h uses Timer peripheral 2, and pins RB15, RB14, and 
 * RB13 (physical pins 26, 25, and 24, respectively).
 * <li> airpic-i2c.h uses the I2C2 peripheral, and pins SDA2 and SCL2 (physical 
 * pins 6 and 7, respectively)
 * <li> airpic-serialgps.h uses the UART1 peripheral, and pins RP10 and RP11 
 * (physical pins 21 and 22, respectively).
 * <li> if the serial debugger messages are enabled, they will use RP6 and RP7 
 * (physical pins 15 and 16, respectively).
 * <p></ul>
 * More detailed information about pin connections are shown below:
 * <p>
 * \htmlonly
 * <img src="https://i.imgur.com/utqZF0K.png" alt="d001-connections diagram" width="800">
 * \endhtmlonly
 * <p>
 * WARNING! The PIC microcontroller is a 3.3V device. If something (like an
 * arduino's RX pin) tries to send data with a 5V logic level, there is a high 
 * chance you will damage the PIC! However, the PIC's outputs can be connected 
 * to inputs on a 5V device.
 * 
 * \subsection arduino Using the Arduino Serial Monitor to Receive Data from Airpic Serial Port
 * <p>
 * Follow these instructions to view the output of Airpic's serial debug port 
 * using the Arduino IDE's Serial Monitor tool. These instructions assume the 
 * user has a cursory understanding of the Arduino IDE.
 * <p>
 * Step 1: Connect an Arduino UNO to a computer. Using the Arduino IDE, upload 
 * a completely empty sketch to the Arduino. (this means that the void setup() 
 * and void loop() are both empty)
 * <p>
 * Step 2: Connect any of the GND pins on the Arduino to any GND pin of the PIC 
 * microcontroller.
 * <p>
 * Step 3: Attach pin 15 of the PIC microcontroller to the TX pin of an Arduino 
 * UNO. (The 'TX' pin on the Arduino is attached to the RX pin of the Arduino's 
 * on-board serial-to-USB converter)
 * <p>
 * Step 4: Open the Arduino IDE.
 * <p>
 * Step 5: In the main function currently running on the PIC microcontroller, 
 * find the call to airpic_debugger_enable(unsigned int baudSelect). If the 
 * given value of baudSelect is one of the predefined macros (ex: DEBUGGER_BAUDRATE_38400) 
 * then the baudrate of the serial port is the number specified in the macro's 
 * name (ex: for DEBUGGER_BAUDRATE_9600, the baudrate is 9600). If the specified 
 * value for baudSelect is not specified using a macro, use this formula to 
 * determine the baudrate: ((1,000,000) / (baudSelect + 1))
 * <p>
 * Step 6: In the Arduino IDE, go to Tools > Serial Monitor. Set the Serial 
 * Monitor's baudrate to the value found in step 5.
 * <p>
 * Any information sent by the Airpic serial debugger port will now appear in
 * the Arduino Serial Monitor.
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
 * <p>
 * In addition, the functions used to enable and disable the serial debugging
 * prompts are included here. The easiest way to see the debugger prompts is to 
 * attach an Arduino UNO's TX pin (digital pin 1) to the debugger TX pin. 
 * Yes, the TX pins go to each other. The Arduino UNO has its TX and RX pins 
 * labeled backwards relative to its USB port. Also: if you do it this way, do
 * NOT connect the UNO's RX pin directly to the PIC! The Arduino uses 5V logic
 * levels, which can damage the PIC microcontroller!
 * @author Evan Perry Grove
 * @date September 26, 2017
 */

#ifndef AIRPIC_H
#define	AIRPIC_H

#include "airpic-libs/airpic-i2c.h"
#include "airpic-libs/airpic-serialgps.h"
#include "airpic-libs/airpic-statusled.h"
#include "airpic-libs/airpic-timer.h"
#include "airpic-libs/i2c-peripherals/airpic-gyro.h"
#include "airpic-libs/i2c-peripherals/airpic-motor.h"

/** @brief Pass this macro into airpic_debugger_enable() to start the serial port with a 4800 baudrate. */
#define DEBUGGER_BAUDRATE_4800      (207)
/** @brief Pass this macro into airpic_debugger_enable() to start the serial port with a 9600 baudrate. */
#define DEBUGGER_BAUDRATE_9600      (103)
/** @brief Pass this macro into airpic_debugger_enable() to start the serial port with a 14400 baudrate. */
#define DEBUGGER_BAUDRATE_14400     (68)
/** @brief Pass this macro into airpic_debugger_enable() to start the serial port with a 19200 baudrate. */
#define DEBUGGER_BAUDRATE_19200     (51)
/** @brief Pass this macro into airpic_debugger_enable() to start the serial port with a 28800 baudrate. */
#define DEBUGGER_BAUDRATE_28800     (34)
/** @brief Pass this macro into airpic_debugger_enable() to start the serial port with a 38400 baudrate. */
#define DEBUGGER_BAUDRATE_38400     (25)
/** @brief Pass this macro into airpic_debugger_enable() to start the serial port with a 115200 baudrate. */
#define DEBUGGER_BAUDRATE_115200    (8)

/** @brief Pass this macro as the second argument of airpic_debugger_printnum() to print the first argument in binary form. */
#define BIN                         (1)
/** @brief Pass this macro as the second argument of airpic_debugger_printnum() to print the first argument in decimal form. */
#define DEC                         (2)
/** @brief Pass this macro as the second argument of airpic_debugger_printnum() to print the first argument in hexadecimal form. */
#define HEX                         (4)

/** @brief A human-friendly name for the bit that indicates whether the debugger port is enabled. */
#define airpic_debugger_isenabled   (U2MODEbits.UARTEN)


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
    
    /**
     * Enable the debugger serial port. Messages are sent in the standard TTL 
     * format (RS-232, but with 0 and 3.3V logic levels).
     * <p>
     * If the debugger port is already enabled, the baudrate will not be 
     * changed. The baudrate of the port can only be changed if it has been 
     * disabled.
     * @param baudSelect The value written to U2BRG. The DEBUGGER_BAUDRATE_xxx
     * macros are a convenient way of setting the UART to common baudrates.
     */
    void airpic_debugger_enable(unsigned int baudSelect);
    
    /**
     * Disable the airpic debugger serial port. The pins will still be attached 
     * to the UART peripheral, but the peripheral is turned off.
     */
    void airpic_debugger_disable(void);
    
    /**
     * Print an ASCII-encoded string to the airpic debugger TX pin.
     * @param str The ASCII-encoded string to be sent.
     * @param len The length of the string given by str.
     */
    void airpic_debugger_print(char* str, unsigned int len);
    
    /**
     * Print an ASCII-encoded string to the airpic debugger TX pin, adding a 
     * newline character at the end.
     * @param str The ASCII-encoded string to be sent.
     * @param len The length of the string given by str.
     */
    void airpic_debugger_println(char* str, unsigned int len);
    
    /**
     * Print a single value (up to 16 bits in length) in the given
     * format.
     * @param val The 16-bit value to be printed.
     * @param format Describes the way 'val' is printed. Use the macros BIN,
     * DEC, or HEX to print the 'val' in binary, decimal, or hex respectively. 
     * If the value of 'format' is not one of the recognized values, 'val' will 
     * be printed as a single ASCII character. Binary and hex formats both will 
     * show leading zeroes; decimal format does not show leading zeroes.
     */
    void airpic_debugger_printnum(unsigned int val, unsigned short format);
    
    /**
     * Print device status information.
     */
    void airpic_debugger_printStatus(void);


#ifdef	__cplusplus
}
#endif

#endif	/* AIRPIC_H */

