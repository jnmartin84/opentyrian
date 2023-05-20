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
#include "vga256d.h"

#include "config.h" // For fullscreen stuff
#include "keyboard.h"
#include "opentyr.h"
#include "palette.h"
#include "video.h"


#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

void JE_pix(uint8_t *surface, int x, int y, JE_byte c)
{
	/* Bad things happen if we don't clip */
	if (x <  /*surface->pitch*/screenpitch && y <  /*surface->h*/screenheight)
	{
		Uint8 *vga = surface/*->pixels*/;
		vga[y * /*surface->pitch*/screenpitch + x] = c;
	}
}

void JE_pix3(uint8_t *surface, int x, int y, JE_byte c)
{
	/* Originally implemented as several direct accesses */
	JE_pix(surface, x, y, c);
	JE_pix(surface, x - 1, y, c);
	JE_pix(surface, x + 1, y, c);
	JE_pix(surface, x, y - 1, c);
	JE_pix(surface, x, y + 1, c);
}

void JE_rectangle(uint8_t *surface, int a, int b, int c, int d, int e) /* x1, y1, x2, y2, color */
{
	if (a < /*surface->pitch*/screenpitch && b < /*surface->h*/screenheight &&
	    c < /*surface->pitch*/screenpitch && d < /*surface->h*/screenheight)
	{
		Uint8 *vga = surface/*->pixels*/;
		int i;

		/* Top line */
		memset(&vga[b * /*surface->pitch*/screenpitch + a], e, c - a + 1);

		/* Bottom line */
		memset(&vga[d * /*surface->pitch*/screenpitch + a], e, c - a + 1);

		/* Left line */
		for (i = (b + 1) * /*surface->pitch*/screenpitch + a; i < (d * /*surface->pitch*/screenpitch + a); i += /*surface->pitch*/screenpitch)
		{
			vga[i] = e;
		}

		/* Right line */
		for (i = (b + 1) * /*surface->pitch*/screenpitch + c; i < (d * /*surface->pitch*/screenpitch + c); i += /*surface->pitch*/screenpitch)
		{
			vga[i] = e;
		}
	}
	else
	{
		printf("!!! WARNING: Rectangle clipped: %d %d %d %d %d\n", a, b, c, d, e);
	}
}
extern Palette palette;

void fill_rectangle_xy(uint8_t *surface, int x, int y, int x2, int y2, Uint8 color)
{
#if 0	
	if (((x + (y*320)) & 63) == 0) {
		surface_t surf = surface_make(surface, FMT_CI8, 320, 200, 320);
		surface_t subsurf = surface_make_sub(&surf, x, y, (x2-x),(y2-y));
		rdpq_attach(&subsurf, NULL);
		rdpq_clear(palette[color]);
		rdpq_detach(); 
	}
	else
#endif	
	 {

	for(size_t ny=y; ny<y2;ny++) {
		memset(surface + (ny*screenpitch) + x, color, x2 - x);
	}
	}
}

void JE_barShade(uint8_t *surface, int a, int b, int c, int d) /* x1, y1, x2, y2 */
{
	if (a < /*surface->pitch*/screenpitch && b < /*surface->h*/screenheight &&
	    c < /*surface->pitch*/screenpitch && d < /*surface->h*/screenheight)
	{
		Uint8 *vga = surface/*->pixels*/;
		int i, j, width;

		width = c - a + 1;

		for (i = b * /*surface->pitch*/screenpitch + a; i <= d * /*surface->pitch*/screenpitch + a; i += /*surface->pitch*/screenpitch)
		{
			for (j = 0; j < width; j++)
			{
				vga[i + j] = ((vga[i + j] & 0x0F) >> 1) | (vga[i + j] & 0xF0);
			}
		}
	}
	else
	{
		printf("!!! WARNING: Darker Rectangle clipped: %d %d %d %d\n", a,b,c,d);
	}
}

void JE_barBright(uint8_t *surface, int a, int b, int c, int d) /* x1, y1, x2, y2 */
{
	if (a < /*surface->pitch*/screenpitch && b < /*surface->h*/screenheight &&
	    c < /*surface->pitch*/screenpitch && d < /*surface->h*/screenheight)
	{
		Uint8 *vga = surface/*->pixels*/;
		int i, j, width;

		width = c-a+1;

		for (i = b * /*surface->pitch*/screenpitch + a; i <= d * /*surface->pitch*/screenpitch + a; i += /*surface->pitch*/screenpitch)
		{
			for (j = 0; j < width; j++)
			{
				JE_byte al, ah;
				al = ah = vga[i + j];

				ah &= 0xF0;
				al = (al & 0x0F) + 2;

				if (al > 0x0F)
				{
					al = 0x0F;
				}

				vga[i + j] = al + ah;
			}
		}
	}
	else
	{
		printf("!!! WARNING: Brighter Rectangle clipped: %d %d %d %d\n", a,b,c,d);
	}
}

void draw_segmented_gauge(uint8_t *surface, int x, int y, Uint8 color, uint segment_width, uint segment_height, uint segment_value, uint value)
{
	assert(segment_width > 0 && segment_height > 0);

	const uint segments = value / segment_value,
	           partial_segment = value % segment_value;

	for (uint i = 0; i < segments; ++i)
	{
		fill_rectangle_wh(surface, x, y, segment_width, segment_height, color + 12);
		x += segment_width + 1;
	}
	if (partial_segment > 0)
		fill_rectangle_wh(surface, x, y, segment_width, segment_height, color + (12 * partial_segment / segment_value));
}
