/*
 * File:   main.c
 * Author: evan
 *
 * Created on September 26, 2017, 2:34 PM
 */

//CW1
#pragma config ICS = PGx1       //comm channel select
#pragma config FWDTEN = OFF     //watchdog timer enable
#pragma config GWRP = OFF       //general code segment write protect
#pragma config GCP = OFF        //general code segment code protect
#pragma config JTAGEN = OFF     //JTAG port enable

//CW2
#pragma config I2C1SEL = PRI    //I2C pin location select (default)
#pragma config IOL1WAY = OFF    //IOLOCK protection
#pragma config OSCIOFNC = ON    //Primary oscillator IO
#pragma config FCKSM = CSECME   //clock switching and monitor (enabled)
#pragma config FNOSC = FRCPLL   //oscillator select


#include "xc.h"
#include <p24Fxxxx.h>

#include "airpic.h"
#include "airpic-libs/airpic-i2c.h"

int main(void) {
    return 0;
}
