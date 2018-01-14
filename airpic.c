#include <p24Fxxxx.h>
#include "airpic.h"

void airpic_config(void)
{
    CLKDIVbits.RCDIV = 0; //RCDIV = 1:1 (default 2:1), sets Fcy to 16MHz
    AD1PCFG = 0x9fff;
    TRISA   = 0x0000;
    // Pin usages:
    //          AAADDDD EE  BBCC
    TRISB   = 0b0001100010000011;
    // A = Status LED
    // B = I2C
    // C = Programmer
    // D = GPS UART
    // E = debug UART
    
    
    // RP4, RP5, RP8 - RP12 available for PPS, 
    // assuming all other libraries are in use.
    // Remember, the programmer uses RP0, RP1, so we
    // probably don't want to connect anything to those
    // pins.
    __builtin_write_OSCCONL(OSCCON & 0xbf);
    RPOR5bits.RP10R      = 3;       // UART1's TX pin is on RP10 (physical pin 21)
    __builtin_write_OSCCONL(OSCCON & 0x40);
    
    __builtin_write_OSCCONL(OSCCON & 0xbf);
    RPINR18bits.U1RXR    = 11;      // UART1's RX pin is on RP11 (physical pin 22)
    __builtin_write_OSCCONL(OSCCON | 0x40);
    
    __builtin_write_OSCCONL(OSCCON & 0xbf);
    RPOR3bits.RP6R       = 5;       // UART2's TX pin is on RP6 (physical pin 21)
    __builtin_write_OSCCONL(OSCCON & 0x40);
    
    __builtin_write_OSCCONL(OSCCON & 0xbf);
    RPINR19bits.U2RXR    = 7;       // UART2's RX pin is on RP7 (physical pin 16)
    __builtin_write_OSCCONL(OSCCON | 0x40);
    
    
    U2MODE              = 0x0000;       // U2BRGH = 0  (high speed off)
                                        // UEN    = 00 (RX and TX enabled, no CTS or RTS)
                                        // PDSEL  = 00 (8 bit data, no parity)
                                        // STSEL  = 0  (one stop bit)
                                        // RXINV  = 0  (RX idle state is '1')
    
    U2BRG               = 103;          // put a safe value in here
    
    IFS1bits.U2RXIF     = 0;            // make sure interrupt flags are clear
    IFS1bits.U2TXIF     = 0;
    
    U2STAbits.UTXISEL0  = 0;            // transmitter interrupt flag triggers
    U2STAbits.UTXISEL1  = 0;            // when there is at least one open spot
                                        // in the transmit buffer
    U2STAbits.URXISEL   = 0b00;         // receive interrupt triggers when a
                                        // character is received
    
        
}

void airpic_debugger_enable(unsigned int baudSelect)
{   
    U2BRG               = baudSelect;
    
    IFS1bits.U2RXIF     = 0;
    IFS1bits.U2TXIF     = 0;
    //IEC1bits.U2RXIE     = 1;
    
    U2MODEbits.UARTEN   = 1;
    U2STAbits.UTXEN     = 1;            // enable the transmitter
    
    asm volatile ("REPEAT, #1680"); Nop();
    
}

void airpic_debugger_disable(void)
{
    IEC1bits.U2RXIE     = 0;
    U2MODEbits.UARTEN   = 0;
    
}

void airpic_debugger_print(char* str, unsigned int len)
{
    unsigned int i;
    for(i = 0; i < len; i++)
    {
        while(!U2STAbits.TRMT);
        U2TXREG = str[i];
        
    }
    
}

void airpic_debugger_println(char* str, unsigned int len)
{
    unsigned int i;
    for(i = 0; i < len; i++)
    {
        while(!U2STAbits.TRMT);
        U2TXREG = str[i];
        
    }
    
    while(!U2STAbits.TRMT);
    U2TXREG = '\n';
    
}

void airpic_debugger_printnum(unsigned int val, unsigned short format)
{ 
    switch( format )
    {
        case BIN:
        {
            unsigned int check[16] = {0x8000, 0x4000, 0x2000, 0x1000,
                                      0x0800, 0x0400, 0x0200, 0x0100,
                                      0x0080, 0x0040, 0x0020, 0x0010,
                                      0x0008, 0x0004, 0x0002, 0x0001};
            char out[16] = "0000000000000000";
            
            // avoid usage of standard C libraries, saves progmem/CPU time
            unsigned int i;
            for(i = 0; i < 16; i++)
            {
                if( val & ( check[i] ) ) check[i] = '1';
                
            }
            
            airpic_debugger_print(out, 16);
            break;
        }
        case DEC:
        {
            // this was originally done using stdio.h. achieved 500% reduction in 
            // program memory usage by replacing it with this.
            unsigned int len = 5;
            unsigned long k = 10;
            char out[] = "00000";
            
            // TODO: this could probably be done more efficiently with a do while.
            //       not a very high priority, but would be nice.
            if(val == 0)
            {
                --len;
            }
            else while(val != 0)    // not sure if this is frowned upon
            {
                --len;
                unsigned long d = val % k;
                out[len] = ((10 * d) / k) + 48;
                val -= d;
                k *= 10;
            }
            
            airpic_debugger_print(out + len, 5 - len);
            break;
        }
        case HEX:
        {
            char out[4] = "0000";
            
            unsigned int i, j;
            for(i = 0; i < 4; i++)
            {
                j = ( (val >> (4 * (3-i))) & 0xF );     //j = <0:15>
                if(j < 0xA) out[i] = (j + '0');
                else out[i] = (j + 55);
                
            }
            
            airpic_debugger_print(out, 4);
            break;
        }
        default:
        {
            // XC16 doesn't like it when an unsigned int is passed directly into 
            // a char argument, so this is a little gross but it will have to do.
            char k = val;
            airpic_debugger_print(&k, 1);
            break;
        }
        
    }
    
}