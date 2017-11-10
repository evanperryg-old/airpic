#include <p24Fxxxx.h>

void __attribute__((__interrupt__,no_auto_psv)) _U1RXInterrupt(void)
{
    
    IFS0bits.U1RXIF     = 0;    // configure and enable the UART1 Receive interrupt.
}

void serial_config(void)
{
    U1MODE  = 0x0000;
    
    //U1MODE.BRGH = 0
    U1BRG   = 103;      // approximately 9,615 baud (0.16% error from desired baudrate)
    
    // Notes from EE2361 state that this config is basically
    // identical to 5-wire TTL RS-232:
    //      - 8 data bits
    //      - no parity bit
    //      - one stop bit
    //      - hardware handshake using CTS and RTS
    
    U1MODEbits.UEN      = 0b10; // 0b10 = 10 =  UxTX, UxRX, !UxCTS and !UxRTS pins are enabled and used
                                // 0b01 =  UxTX, UxRX and UxRTS pins are enabled and used; UxCTS pin is controlled by port latches
    
    U1MODEbits.PDSEL    = 0b00; // 0b00 = 8-bit data, no parity
    U1MODEbits.STSEL    = 0;    // 0    = 1 stop bit

    U1STAbits.URXISEL   = 0b00; // 0b0x = interrupt triggers when a character is received
                                // 0b10 =     "         "      "  receive buffer is 3/4ths full
                                // 0b11 =     "         "      "  receive buffer is full
    
    IFS0bits.U1RXIF     = 0;    // configure and enable the UART1 Receive interrupt.
    //IPC2bits.U1RXIP     = 3;
    IEC0bits.U1RXIE     = 1;
    
    U1MODEbits.UARTEN   = 1;    // enable the UART module
}
