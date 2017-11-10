#include <p24Fxxxx.h>
/**
 * Configure the microcontroller to be used with the Airpic libraries.
 */
void airpic_config(void)
{
    CLKDIVbits.RCDIV = 0; //RCDIV = 1:1 (default 2:1)
    AD1PCFG = 0x9fff;
    TRISA   = 0x0000;
    // Pin usages:
    //          AAADDDD     BBCC
    TRISB   = 0b0001100000000011;
    // A = Status LED
    // B = I2C
    // C = Programmer
    // D = UART
    
    
    // RP4 - RP12 available for PPS, 
    // assuming all other libraries are in use.
    // Remember, the programmer uses RP0, RP1, so we
    // probably don't want to connect anything to those
    // pins.
    __builtin_write_OSCCONL(OSCCON & 0xbf);
    RPOR5bits.RP10R      = 3;       // UART1's TX pin is on RP10 (physical pin 21)
    RPOR4bits.RP9R       = 4;       // UART1's 'ready to send' pin is on RP9 (physical pin 18)
    __builtin_write_OSCCONL(OSCCON & 0x40);
    
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    RPINR18bits.U1RXR   = 11;       // UART1's RX pin is on RP11 (physical pin 22)
    RPINR18bits.U1CTSR  = 12;       // UART1's 'clear to send' pin is on RP12 (physical pin 23)
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock PPS
    
        
}
