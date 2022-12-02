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
 * @file config.h
 * @brief Header for robot-specific configuration
 * @author Ayberk Özgür
 * @date 2016-03-20
 */

#ifndef CONFIG_H
#define CONFIG_H

#ifdef  __cplusplus
extern "C" {
#endif

/*
 * Wheel configuration, must be the following (bottom view):
 *
 *       M3        M2
 *
 *   USB
 *
 *            M1
 */

#define M1_MAPPING 'A'
#define M2_MAPPING 'B'
#define M3_MAPPING 'C'
//#define REVERSE_MOTORS //Uncomment to reverse motor direction

/*
 * Camera exposure settings (define only one):
 */

#define EXPOSURE_NEW_HOUSING //White/gray housing with motors and with LEDs extended via cables
//#define EXPOSURE_OLD_HOUSING //Old small housing with LEDs mounted on the board

/*
 * Preprocessor messages
 */

#define _STR(x) # x
#define STR(x) _STR(x)
#pragma message("CONFIG: M1_MAPPING IS " STR(M1_MAPPING))
#pragma message("CONFIG: M2_MAPPING IS " STR(M2_MAPPING))
#pragma message("CONFIG: M3_MAPPING IS " STR(M3_MAPPING))
#ifdef REVERSE_MOTORS
    #pragma message("CONFIG: REVERSE_MOTORS IS DEFINED")
#endif
#if defined(EXPOSURE_NEW_HOUSING)
    #pragma message("CONFIG: EXPOSURE_NEW_HOUSING IS SELECTED")
#elif defined(EXPOSURE_OLD_HOUSING)
    #pragma message("CONFIG: EXPOSURE_OLD_HOUSING IS SELECTED")
#else
    #error "CONFIG: NO EXPOSURE SETTING SELECTED"
#endif

#ifdef  __cplusplus
}
#endif

#endif //CONFIG_H
