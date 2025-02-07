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
#ifndef VGA256D_H
#define VGA256D_H

#include "opentyr.h"

//#include "SDL.h"
#include <libdragon.h>
#include "palette.h"

void JE_pix(uint8_t *surface, int x, int y, JE_byte c);
void JE_pix3(uint8_t *surface, int x, int y, JE_byte c);
void JE_rectangle(uint8_t *surface, int a, int b, int c, int d, int e);

void fill_rectangle_xy(uint8_t *, int x, int y, int x2, int y2, Uint8 color);

void JE_barShade(uint8_t *surface, int a, int b, int c, int d);
void JE_barBright(uint8_t *surface, int a, int b, int c, int d);
#define screenwidth 320
#define screenheight 200
#define screenpixbytes 1
#define screenpitch (screenwidth*screenpixbytes)

static inline void fill_rectangle_wh(uint8_t *surface, int x, int y, uint w, uint h, Uint8 color)
{
	for(size_t ny=y; ny<y+h;ny++) {
		memset(surface + (ny*screenpitch) + x, color, w);
	}
}

void draw_segmented_gauge(uint8_t *surface, int x, int y, Uint8 color, uint segment_width, uint segment_height, uint segment_value, uint value);

#endif /* VGA256D_H */
