# airpic
Libraries for Small-Scale Automated Aviation Systems Using PIC24F Microcontrollers

### [In-depth documentation is available in Github Pages.](https://evanperryg.github.io/airpic/html/index.html)

## Introduction
The Airpic Libraries include various tools to simplify and accelerate the development of C code for the PIC24 microcontroller. Airpic was designed for use with Project IRIS, a project created by Innovative Engineers at the University of Minnesota Twin Cities.

## Microcontroller
These libraries were written for the PIC24FJ64GA002 microcontroller, but they are theoretically compatible with any PIC24FJ device. It should be relatively easy to adapt these libraries for use on virtually any PIC24 microcontroller with sufficient program memory.

Airpic uses a variety of peripherals and pins on the PIC device:

- airpic-timer uses Timer peripherals 1, 4, and 5.
- airpic-statusled uses Timer peripheral 2, and pins RB15, RB14, and RB13 (physical pins 26, 25, and 24, respectively).
- airpic-i2c uses the I2C2 peripheral, and pins SDA2 and SCL2 (physical pins 6 and 7, respectively)
- airpic-serialgps uses the UART1 peripheral, and pins RP10 and RP11 (physical pins 21 and 22, respectively).
- If the serial debugger messages are enabled, they will use RP6 and RP7 (physical pins 15 and 16, respectively).

More detailed information about pin connections are shown below: 

![Imgur utqZF0K](http://i.imgur.com/utqZF0K.png)

BE CAREFUL! The PIC microcontroller is a 3.3V device. If something (like an arduino's RX pin) tries to send data with a 5V logic level, there is a high chance you will damage the PIC! However, the PIC's outputs can be connected to inputs on a 5V device (for example, the debugging TX pin can be connected to the TX pin on an Arduino).

## Compiler/Technical Information
Airpic was compiled using the XC16 compiler, version 1.32B. The IDE used was MPLAB X, version 4.00. 
