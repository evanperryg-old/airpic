/**
 * @file    airpic-serialgps.h
 * @brief   Configure and use UART1 for RS-232 communication.
 * <p>
 * This part is still extremely new. Not much to see here.
 * @author  Evan Perry Grove
 * @date    November 10, 2017
 */

#ifndef AIRPIC_SERIALGPS_H
#define	AIRPIC_SERIALGPS_H

/** @brief A more user-readable version of the UART1 receiver overflow flag. */
#define serial_rcvbuffer_error_overflow (U1STAbits.OERR)

/** @brief A more user-readable version of the UART1 receiver "data available" flag. */
#define serial_rcvbuffer_data_available (U1STAbits.URXDA)

/** @brief A more user-readable version of the UART1 framing error flag. */
#define serial_receiver_error_framing (U1STAbits.FERR)

/** @brief A more user-readable version of the inverted UART1 "receiver idle" flag. */
#define serial_receiver_active (!U1STAbits.RIDLE)


#ifdef	__cplusplus
extern "C" {
#endif
    
    /**
     * The string available to the user is stored here.
     */
    extern char fullString[80];
    
    /**
     * Configure UART1 to receive data from the GPS. The GPS used sends TTL serial 
     * data as NMEA strings. Baudrate is 9600 by default. Although there are ways 
     * for the user to tell the GPS to change the baudrate, this feature is not 
     * currently implemented, as it is unnecessary for this project at the present 
     * time.
     */
    void serialGPS_config(void);
    
    /**
     * Contains the length of the NMEA string currently available to be read by 
     * the user code. NMEA strings deemed unimportant will not update the value 
     * stored here.
     * @return The length of the NMEA string currently available to the user.
     */
    unsigned int serialGPS_strLen();
    
    /**
     * The number of times the desired NMEA string has been fully 
     * received. In application, this can be used to tell when the data has been 
     * updated.
     * @return 
     */
    unsigned int serialGPS_readoutCount();
    
    /**
     * Parses the NMEA string currently stored in the fullString[] array. This 
     * is not done in the UART receive interrupt because it is pretty CPU-
     * intensive.
     */
    void serialGPS_parse();
    
    /**
     * Prevent the UART interrupt from overwriting the information stored in the 
     * fullString[] array.
     */
    void serialGPS_lockBuffer();
    

#ifdef	__cplusplus
}
#endif

#endif	/* AIRPIC_SERIAL_H */

