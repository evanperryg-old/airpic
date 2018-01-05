/** 
 * @file airpic-statusled.h
 * @brief  Status LED manager.
 * This library uses a common-cathode RGB LED to show system status. It is assumed 
 * that the relative brightness of each color is normalized by resistors. For the LED
 * used in testing, a good balance was found with a 200 Ohm resistor connecting to red, 
 * a 100 Ohm resistor connecting to green, and a 1000 Ohm resistor connecting to blue.
 * <p>
 * The LED is attached as follows:
 * <p><ul>
 * <li> Red is attached to RB15 (pin 26 of the PIC24FJ64GA002) through 200 ohm resistor
 * <li> Blue is attached to RB14 (pin 25 of the PIC24FJ64GA002) through 1000 ohm resistor
 * <li> Green is attached to RB13 (pin 24 of the PIC24FJ64GA002) through 100 ohm resistor
 * </ul><p>
 * By default, after the status LED is initialized by running \c statusLED_enable(),
 * the LED will show a long, blue blinking pattern.
 * <p>
 * Use of the status LED should be avoided in a final implementation. The primary 
 * purpose of this library is to provide an easy tool to help in debugging by 
 * allowing the user to more easily see what the microcontroller is up to. If the 
 * status LED isn't being used for debugging or showing vital status updates, it 
 * probably shouldn't be used, as it is a little clock cycle-hungry.
 * @author Evan Perry Grove
 * @date   November 7, 2017, 4:10 PM
 */

#ifndef AIRPIC_STATUSLED_H
#define	AIRPIC_STATUSLED_H

/** @brief Turn off status LED. */
#define STATUSLED_OFF           0x0001

/** @brief Status LED stays on, no blinking. */
#define STATUSLED_SOLID         0x0002

/** @brief Status LED stays on for a moment, then is off for a second. */
#define STATUSLED_SHORTBLINK    0x0000

/** @brief Status LED stays on for a second, then is off for a moment. */
#define STATUSLED_LONGBLINK     0x0003

/** @brief Status LED blinks very quickly. */
#define STATUSLED_FASTBLINK     0x0004

/** @brief Status LED will be red. */
#define STATUSLED_RED           0x8000

/** @brief Status LED will be green. */
#define STATUSLED_GREEN         0x2000

/** @brief Status LED will be blue. */
#define STATUSLED_BLUE          0x4000

/** @brief Status LED will be orange. */
#define STATUSLED_ORANGE        0xA000

/** @brief Status LED will be teal. */
#define STATUSLED_TEAL          0x6000

/** @brief Status LED will be magenta. */
#define STATUSLED_MAGENTA       0xC000

#ifdef	__cplusplus
extern "C" {
#endif
    
    /**
     * Configure Timer2 to be used for the status LED. Immediately after 
     * enable_statusLED() is run, it will default to blue color and short
     * blinking pattern.
     */
    void statusLED_enable(void);
    
    /**
     * 
     * @param stat A value describing the color and blink pattern of the LED. 
     * Use the STATUSLED_ macros included in airpic-status.h combined 
     * together with bitwise OR.
     */
    void statusLED_setStatus(unsigned int stat);

#ifdef	__cplusplus
}
#endif

#endif	/* AIRPIC_STATUSLED_H */

