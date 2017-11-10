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

#ifdef	__cplusplus
extern "C" {
#endif
    // the default baudrate of our GPD is 9600bps
    
    void serial_config(void);



#ifdef	__cplusplus
}
#endif

#endif	/* AIRPIC_SERIAL_H */

