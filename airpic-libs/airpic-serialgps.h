/**
 * @file    airpic-serialgps.h
 * @brief   Configure and use UART1 for RS-232 communication with GPS.
 * <p>
 * Take care of basic communication with a MK3339-based GPS module.
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

/** @brief Contains the length of the fullString buffer, generally should not be changed by the user. */
#define NMEASTR_BUFFLEN 100


#ifdef	__cplusplus
extern "C" {
#endif
    
    /**
     * The string available to the user is stored here.
     */
    extern char fullString[NMEASTR_BUFFLEN];
    
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
     * Parses the NMEA string currently stored in the fullString[] array. This 
     * is not done automatically in the UART receive interrupt because it is 
     * pretty CPU-intensive, and if every update is not used, it would be 
     * extremely inefficient to parse every string.
     */
    void serialGPS_parse();
    
    /**
     * The number of times the desired NMEA string has been fully 
     * received. This can be used to determine when the serialGPS_parse() 
     * function needs to be run. <i>(Hint: With the MK3339 GPS in default 
     * settings, this is approximately the number of seconds since the 
     * serialGPS_config() function was run)</i>
     * @return The number of times the GPS data has been updated.
     */
    unsigned int serialGPS_readoutCount();
    
    /**
     * The GPS Fix will always be one of three values, meaning the following:
     * <p><ul>
     * <li> 0: Fix not available (bad)
     * <li> 1: GPS fix (good)
     * <li> 2: Differential GPS fix (only happens if differential GPS is enabled)
     * </ul><p>
     * @return The last Fix value returned by the GPS.
     */
    int gpsFix();
    
    /**
     * The hour component of the UTC time, in 24-hour format.
     * @return Hour of the current UTC time, 0-23.
     */
    int gpsTime_hours();
    
    /**
     * The minutes component of the UTC time.
     * @return Minutes of the current UTC time.
     */
    int gpsTime_minutes();
    
    /**
     * The seconds component of the UTC time, with precision to 3 decimal places.
     * @return Seconds component of the current UTC time, 5 significant figures.
     */
    long double gpsTime_seconds();
    
    /**
     * The current GPS latitude, in degrees.
     * @return The GPS latitude in degrees.
     */
    long double gpsLatitude();
    
    /**
     * The current GPS longitude, in degrees.
     * @return The GPS longitude in degrees.
     */
    long double gpsLongitude();
    
    /**
     * The current altitude of the GPS above/below sea level, measured in meters.
     * @return The MSL altitude of the GPS, in meters.
     */
    long double gpsAltitude();
    
    /**
     * A character representing the hemisphere of the Latitude value. Always 
     * will return either 'N' or 'S'
     * @return Hemisphere of the latitude, either 'N' or 'S'.
     */
    char gpsLatitudeDirection();
    
    /**
     * A character representing the hemisphere of the Longitude value. Always 
     * will return either 'E' or 'W'
     * @return Hemisphere of the longitude, either 'E' or 'W'.
     */
    char gpsLongitudeDirection();
    
    /**
     * Get the UTC time, formatted as an 8-character ASCII string.
     * @return The UTC time, HH:MM:SS.
     */
    char* gpsTimeString();
    

#ifdef	__cplusplus
}
#endif

#endif	/* AIRPIC_SERIAL_H */

