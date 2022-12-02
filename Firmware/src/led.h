/*
 * Copyright (C) 2015 EPFL
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */

/**
 * @file led.h
 * @brief Header for user interface LED activities
 * @author Ayberk Özgür
 * @date 2015-04-26
 */

#ifndef LED_H
#define LED_H

#include "driver/spi/drv_spi.h"   // SYS function prototypes

#include "timer.h"

#include "qml-plugin/include/CelluloBluetoothSharedDefs.h"

#define LED_IDLE_BRIGHTNESS 0x600
#define LED_TOUCH_RESPONSE_EXTRA_BRIGHTNESS 0x800
#define LED_IDLE_MODE_BRIGHTNESS_DIVISOR 2
#define LED_WAITING_TURN_SPEED 13
#define LED_TRANSITION_INERTIA_DEFAULT 10 //Higher this value, the more time it takes for transitioning
#define LED_ALERT_FLASH_MILLIS 300
#define LED_BLINK_FLASH_DEFAULT_VALUE 15
#define LED_BLINK_FLASH_MILLIS_MULTIPLIER 20
#define LED_BREATHE_BRIGHTNESS_DIVISOR 4
#define LED_BREATHE_LOW_MILLIS 2000
#define LED_BREATHE_HIGH_MILLIS 400
#define LED_TRANSITION_INERTIA_PULSE 25

#define LOW_NIBBLE_MASK 0x000F
#define LOW_BYTE_MASK 0x00FF

#define LED_XLAT_PORT_CHANNEL PORT_CHANNEL_C
#define LED_XLAT_PORT_BIT_POS PORTS_BIT_POS_14

typedef LED_RESPONSE_MODE_ENUM_SHARED LEDResponseMode;
typedef VISUAL_EFFECT_ENUM_SHARED VisualEffect;

extern bool effectTransition;       ///< The current effect is still transitioning
extern VisualEffect visualEffect;  ///< The current effect

/**
 * @brief Describes a sequence of 6 RGB leds, each with 12-bit brightness
 *
 * Since buffer elements are sent from 0 to 6, they end up reversed in the
 * shift register of the LED driver. Therefore they are in reverse order here.
 */
typedef union {
    struct {

        //Int 0
        unsigned int led5green              : 12;
        unsigned int led5blue               : 12;
        unsigned int unused                 : 8;

        //Int 1
        unsigned int led4green_high_byte    : 8;
        unsigned int led4blue               : 12;
        unsigned int led5red                : 12;

        //Int 2
        unsigned int led3green_high_nibble  : 4;
        unsigned int led3blue               : 12;
        unsigned int led4red                : 12;
        unsigned int led4green_low_nibble   : 4;

        //Int 3
        unsigned int led2blue               : 12;
        unsigned int led3red                : 12;
        unsigned int led3green_low_byte     : 8;

        //Int 4
        unsigned int led1blue_high_byte     : 8;
        unsigned int led2red                : 12;
        unsigned int led2green              : 12;

        //Int 5
        unsigned int led0blue_high_nibble   : 4;
        unsigned int led1red                : 12;
        unsigned int led1green              : 12;
        unsigned int led1blue_low_nibble    : 4;

        //Int 6
        unsigned int led0red                : 12;
        unsigned int led0green              : 12;
        unsigned int led0blue_low_byte      : 8;
    } leds;

    unsigned int buffer[7];
} LEDObject;

extern LEDObject leds;                          ///< Object that contains the data to be sent to the LED driver
extern unsigned short currentLEDColors[6][3];   ///< Current colors of the LEDs: [led number in 0, 1, 2, 3, 4 or 5][red, green or blue in 0, 1 or 2]

/**
 * Initializes user interface LED functionality
 */
void APP_LED_Initialize();

/**
 * Performs regular user interface LED tasks
 */
void APP_LED_Tasks();

/**
 * @brief Suspends LED operation for sleep
 */
void suspendLEDsSleep();

/**
 * @brief Suspends LED operation for idle mode
 */
void suspendLEDsIdle();

/**
 * @brief Resumes LED operation (after waking up from sleep)
 */
void resumeLEDsSleep();

/**
 * @brief Resumes LED operation after idle mode
 */
void resumeLEDsIdle();

/**
 * @brief Plays a low battery animation with LEDs and suspends LED functionality off again
 */
void lowBatteryAnimation();

/**
 * @brief Sets the new LED response mode
 *
 * @param newMode The new LED response mode
 */
void setLEDResponseMode(LEDResponseMode newMode);

/**
 * @brief Sets the new visual effect
 *
 * @param newEffect The new visual effect
 * @param red 0 to 255
 * @param green 0 to 255
 * @param blue 0 to 255
 * @param value A value relevant to the desired effect
 */
void setVisualEffect(VisualEffect newEffect, unsigned char red, unsigned char green, unsigned char blue, int value);

/**
 * @brief Makes LED respond to key touch, if currently relevant and allowed
 *
 * @param key 0, 1, 2, 3, 4 or 5
 * @param touched Whether touched or released
 */
void keyTouched(unsigned int key, bool touched);

/**
 * @brief Sets the LED brightness, does not get updated immediately
 *
 * @param led 0, 1, 2, 3, 4 or 5
 * @param red Red brightness, 0 to 4095
 * @param green Green brightness, 0 to 4095
 * @param blue Blue brightness, 0 to 4095
 */
void setLED(unsigned int led, unsigned short red, unsigned short green, unsigned short blue);

/**
 * @brief Requests update for all LEDs in the next cycle
 */
void requestLEDUpdate();

#endif //LED_H
