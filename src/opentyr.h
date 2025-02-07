/*
 * OpenTyrian: A modern cross-platform port of Tyrian
 * Copyright (C) 2007-2009  The OpenTyrian Development Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#ifndef OPENTYR_H
#define OPENTYR_H

//#include "SDL_types.h"

#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#define SHORT(x)	((int16_t)(((uint16_t)(x)>>8)|((uint16_t)(x)<<8))) 
#define LONG(x)     ((int32_t)((((uint32_t)(x)>>24)) | \
					((((uint32_t)(x)&0xff00)<<8)) | \
					(((uint32_t)(x)>>8)&0xff00) | \
					(((uint32_t)(x)<<24))))

#define COUNTOF(x) ((unsigned)(sizeof(x) / sizeof *(x)))  // use only on arrays!
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#ifndef M_PI
#define M_PI    3.14159265358979323846  // pi
#endif
#ifndef M_PI_2
#define M_PI_2  1.57079632679489661923  // pi/2
#endif
#ifndef M_PI_4
#define M_PI_4  0.78539816339744830962  // pi/4
#endif

typedef unsigned int uint;
typedef unsigned long ulong;

/**
 * \brief A signed 8-bit integer type.
 */
typedef int8_t Sint8;
/**
 * \brief An unsigned 8-bit integer type.
 */
typedef uint8_t Uint8;
/**
 * \brief A signed 16-bit integer type.
 */
typedef int16_t Sint16;
/**
 * \brief An unsigned 16-bit integer type.
 */
typedef uint16_t Uint16;
/**
 * \brief A signed 32-bit integer type.
 */
typedef int32_t Sint32;
/**
 * \brief An unsigned 32-bit integer type.
 */
typedef uint32_t Uint32;

/**
 * \brief A signed 64-bit integer type.
 */
typedef int64_t Sint64;
/**
 * \brief An unsigned 64-bit integer type.
 */
typedef uint64_t Uint64;

// Pascal types, yuck.
typedef Sint32 JE_longint;
typedef Sint16 JE_integer;
typedef Sint8  JE_shortint;
typedef Uint16 JE_word;
typedef Uint8  JE_byte;
typedef bool   JE_boolean;
typedef char   JE_char;
typedef float  JE_real;

#define TYRIAN_VERSION "2.1"

extern const char *opentyrian_str;
extern const char *opentyrian_version;


typedef enum {
    ctrlr_up,
	ctrlr_right,
	ctrlr_down,
	ctrlr_left,
	ctrlr_start,
	ctrlr_l,
	ctrlr_r,
	ctrlr_z,
	ctrlr_b,
	ctrlr_a,
	ctrlr_c_up,
	ctrlr_c_right,
	ctrlr_c_down,
	ctrlr_c_left
} evt_key_t;

typedef struct input_event_s {

    int down; // 0 = no, 1 = yes    
    evt_key_t key;

} input_event_t;

void reset_input_queue(void);
bool pop_input_queue(input_event_t **ev);
uint32_t n64_GetTicks();
void wait_ms(uint32_t tick);
void setupMenu(void);
#define SOUND_SAMPLE_RATE 11025
//11025
#define SAMPLE_DATA_BYTES 2

#define STEREO_MUL 2
#define NUM_SAMPLES 256
#define NUM_BYTES_IN_SAMPLE_BUFFER (NUM_SAMPLES * SAMPLE_DATA_BYTES * STEREO_MUL)
#endif /* OPENTYR_H */
