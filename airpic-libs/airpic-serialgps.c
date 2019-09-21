#include <p24Fxxxx.h>
#include <stdlib.h>
#include <string.h>
#include "./../airpic.h"

#include "airpic-serialgps.h"

// the currentread buffer holds the NMEA line currently being received by the 
// PIC. fullString holds the most recent fully completed GPGGA sequence.
char fullString[NMEASTR_BUFFLEN];
unsigned int strLen;

char currentRead[NMEASTR_BUFFLEN];
unsigned int lineIndex;             // length of string = lineIndex + 1;
unsigned int readyToFeed;
unsigned int stringIsRelevant = 0;
unsigned int readOutCount;

int fix, satellites, checksum, time_h, time_m;
long double latitude, longitude, hdop, altitude, geoid, time_s;  // IEEE 754 single-precision float
char latDir, lonDir, altUnit, geoUnit;

char timeStr[] = "  :  :  ";

void __attribute__((__interrupt__,no_auto_psv)) _U1RXInterrupt(void)
{
    // URXDA is high if there is a byte available to read from the UART buffer
    while(U1STAbits.URXDA)
    {
        
       
        currentRead[ lineIndex ] = U1RXREG;
        
        // once we get to index 5, poll the 6 characters we have and see if this is
        // a line that we care about.
        if(lineIndex == 5)
        {
            if( (currentRead[1] == 'G') &&
                (currentRead[2] == 'P') &&
                (currentRead[3] == 'G') &&
                (currentRead[4] == 'G') &&
                (currentRead[5] == 'A')    )
            {
                stringIsRelevant = 1;
            }
            
        }
        
        // check to see if we've reached the end of this NMEA sequence.
        if( currentRead[ lineIndex ] == 0xA )
        {
            // end of this NMEA string has been reached.
            // if this is a GPGGA string, then we marked it as relevant earlier.
            if( stringIsRelevant )
            {
                // If the string was marked as one we want, then move it into
                // the currentRead array and increment the readout counter.
                // it's fine to not have the last character array in the string,
                // because it's just the line feed.
                unsigned int i;
                for(i = 0; i < lineIndex; i++)
                {
                    fullString[i] = currentRead[i];
                }
                for(i = lineIndex; i < NMEASTR_BUFFLEN; i++)
                {
                    currentRead[i] = ' ';
                }
                strLen = lineIndex;
                
                ++readOutCount;
                stringIsRelevant = 0;
                
            }
            
            lineIndex = 0;
            
        }
        else
        {
            //this string still has more to go
            ++lineIndex;
        }
    }
    
    IFS0bits.U1RXIF     = 0;
    
}

void serialGPS_config(void)
{
    lineIndex        = 0;
    stringIsRelevant = 1;
    readOutCount     = 0;
    
    U1MODE  = 0x0000;
    
    //U1MODE.BRGH = 0
    U1BRG   = 103;      // approximately 9,615 baud (0.16% error from desired baudrate)
    
    // Notes from EE2361 state that this config is basically
    // identical to 5-wire TTL RS-232:
    //      - 8 data bits
    //      - no parity bit
    //      - one stop bit
    
    U1MODEbits.UEN      = 0b00; // 0b10 = UxTX, UxRX, !UxCTS and !UxRTS pins are enabled and used
                                // 0b00 = UxTX, UxRX are enabled and used
    
    U1MODEbits.PDSEL    = 0b00; // 0b00 = 8-bit data, no parity
    U1MODEbits.STSEL    = 0;    // 0    = 1 stop bit
    U1MODEbits.RXINV    = 0;    // RX idle state is '1'

    U1STAbits.URXISEL   = 0b00; // 0b0x = interrupt triggers when a character is received
                                // 0b10 =     "         "      "  receive buffer is 3/4ths full
                                // 0b11 =     "         "      "  receive buffer is full
    
    IFS0bits.U1RXIF     = 0;    // configure and enable the UART1 Receive interrupt.
    //IPC2bits.U1RXIP     = 3;
    IEC0bits.U1RXIE     = 1;
    
    U1MODEbits.UARTEN   = 1;    // enable the UART module
    
}

unsigned int serial_strLen()
{
    return strLen;
}

void serialGPS_parse()
{
    char * pch;
    
    pch = strtok( fullString, ",");
    short i = 0;
    
    while(pch != NULL)
    {
        // process the item in pch
        switch(i)
        {
            case 0:
                // sequence identifier, in this case pch = "$GPGGA"
                // we already identified it as a GPGGA string during the
                // UART1 RX interrupt, so we don't need to do anything here.
            break;
            case 1:
            {
                // utc time string is formatted as [h][h][m][m][s][s].[s][s][s]
                char subbuff_timehr[3];
                char subbuff_timemin[3];
                char subbuff_timesec[ strlen(pch) - 3];
                memcpy( subbuff_timehr,  &pch[0], 2 );
                memcpy( subbuff_timemin, &pch[2], 2 );
                memcpy( subbuff_timesec, &pch[4], strlen(pch) - 3);
                subbuff_timehr[2]  = '\0';
                subbuff_timemin[2] = '\0';
                subbuff_timesec[ strlen(subbuff_timesec) - 1 ] = '\0';
                
                timeStr[0] = subbuff_timehr[0];
                timeStr[1] = subbuff_timehr[1];
                timeStr[3] = subbuff_timemin[0];
                timeStr[4] = subbuff_timemin[1];
                timeStr[6] = subbuff_timesec[0];
                timeStr[7] = subbuff_timesec[1];
                
                time_h  =   atoi( subbuff_timehr  );
                time_m  =   atoi( subbuff_timemin );
                time_s  =   atof( subbuff_timesec );
                
            }
            break;
            case 2:
            {
                // latitude from string is in [deg][deg][m][m].[m][m][m][m]
                // convert to degrees
                if(strlen(pch) > 2)
                {
                    char subbuff_latdeg[3];
                    char subbuff_latmin[ strlen(pch)-1 ];
                    memcpy( subbuff_latdeg, &pch[0], 2 );
                    memcpy( subbuff_latmin, &pch[2], strlen(pch) - 2);
                    subbuff_latdeg[2] = '\0';
                    subbuff_latmin[ strlen(subbuff_latmin) - 1 ] = '\0';

                    double latminval = atof( subbuff_latmin );
                    double latmaxval = atof( subbuff_latdeg );

                    latitude     = ( latminval / 60.0 ) +
                                   ( latmaxval );
                
                }
                               
            }
            break;
            case 3:
                latDir       = pch[0];
            break;
            case 4:
            {
                // longitude from string is in [deg][deg][deg][m][m].[m][m][m][m]
                // convert to degrees
                if(strlen(pch) > 3)
                {
                    char subbuff_londeg[4];
                    char subbuff_lonmin[ strlen(pch) - 1];
                    memcpy( subbuff_londeg, &pch[0], 3);
                    memcpy( subbuff_lonmin, &pch[3], strlen(pch) - 3);
                    subbuff_londeg[3] = '\0';
                    subbuff_lonmin[ strlen(subbuff_lonmin) - 1 ] = '\0';

                    longitude    = ( atof( subbuff_lonmin ) / 60.0 ) +
                                   ( atof( subbuff_londeg ));
                
                }
                
            }
            break;
            case 5:
                lonDir       = pch[0];
            break;
            case 6:
                fix          = atoi(pch);
            break;
            case 7:
                satellites   = atoi(pch);
            break;
            case 8:
                hdop         = atof(pch);
            break;
            case 9:
                altitude     = atof(pch);
            break;
            case 10:
                altUnit      = pch[0];
            break;
            case 11:
                geoid        = atof(pch);
            break;
            case 12:
                geoUnit      = pch[0];
            break;
            
        }
        
        // put the next comma-delimited item in pch and increment our indexer
        pch = strtok (NULL, ",");
        ++i;
    }
    
}

unsigned int serialGPS_readoutCount()
{
    return readOutCount;
}

int gpsFix()
{
    return fix;
}

int gpsTime_hours()
{
    return time_h;
}

int gpsTime_minutes()
{
    return time_m;
}

long double gpsTime_seconds()
{
    return time_s;
}

long double gpsLatitude()
{
    return latitude;
}

long double gpsLongitude()
{
    return longitude;
}

char gpsLatitudeDirection()
{
    return latDir;
}

char gpsLongitudeDirection()
{
    return lonDir;
}

char* gpsTimeString()
{
    return timeStr;
}