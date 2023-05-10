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
#include "video.h"

#include "keyboard.h"
#include "opentyr.h"
#include "palette.h"
#include "video_scale.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *const scaling_mode_names[ScalingMode_MAX] = {
	"Center",
//	"Integer",
//	"Fit 8:5",
//	"Fit 4:3",
};

int fullscreen_display;
ScalingMode scaling_mode = SCALE_INTEGER;
//static SDL_Rect last_output_rect = { 0, 0, vga_width, vga_height };

uint8_t *VGAScreen, *VGAScreenSeg;
uint8_t *VGAScreen2;
uint8_t *game_screen;

//SDL_Window *main_window = NULL;
//static SDL_Renderer *main_window_renderer = NULL;
//SDL_PixelFormat *main_window_tex_format = NULL;
//static SDL_Texture *main_window_texture = NULL;

//static ScalerFunction scaler_function;

static void init_renderer(void);
static void deinit_renderer(void);
//static void init_texture(void);
static void deinit_texture(void);

//static int window_get_display_index(void);
//static void window_center_in_display(int display_index);
//static void calc_dst_render_rect(uint8_t *src_surface, SDL_Rect *dst_rect);
//static void scale_and_flip(SDL_Surface *);
extern void *__safe_buffer[];

void init_video(void)
{
	console_clear();
	console_close();
	memset((uint16_t *)(__safe_buffer[0]), 0, 320*240*2);//SCREENWIDTH*2*32);//336*2);
    memset((uint16_t *)(__safe_buffer[1]), 0, 320*240*2);//SCREENWIDTH*2*32);//336*2);

	display_init(RESOLUTION_320x240, DEPTH_16_BPP, 2, GAMMA_NONE, ANTIALIAS_RESAMPLE);
	VGAScreenSeg = malloc(320*200);
	VGAScreen = VGAScreenSeg;// SDL_CreateRGBSurface(0, vga_width, vga_height, 8, 0, 0, 0, 0);
	VGAScreen2 = malloc(320*200);//SDL_CreateRGBSurface(0, vga_width, vga_height, 8, 0, 0, 0, 0);
	game_screen = malloc(320*200);// SDL_CreateRGBSurface(0, vga_width, vga_height, 8, 0, 0, 0, 0);
	JE_clr256(VGAScreen);

	// Create the window with a temporary initial size, hidden until we set up the
	// scaler and find the true window size
#if 0
	main_window = SDL_CreateWindow("OpenTyrian",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		vga_width, vga_height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN);

	if (main_window == NULL)
	{
		fprintf(stderr, "error: failed to create window: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
#endif
	reinit_fullscreen(fullscreen_display);
	init_renderer();
	//init_texture();
	//init_scaler(scaler);

//	SDL_ShowWindow(main_window);

//	SDL_SetRenderDrawColor(main_window_renderer, 0, 0, 0, 255);
//	SDL_RenderClear(main_window_renderer);
//	SDL_RenderPresent(main_window_renderer);
}

void deinit_video(void)
{
	deinit_texture();
	deinit_renderer();

//	SDL_DestroyWindow(main_window);

//	SDL_FreeSurface(VGAScreenSeg);
//	SDL_FreeSurface(VGAScreen2);
//	SDL_FreeSurface(game_screen);

//	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

static void init_renderer(void)
{
#if 0	
	main_window_renderer = SDL_CreateRenderer(main_window, -1, 0);

	if (main_window_renderer == NULL)
	{
		fprintf(stderr, "error: failed to create renderer: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
#endif
}

static void deinit_renderer(void)
{
	#if 0
	if (main_window_renderer != NULL)
	{
		SDL_DestroyRenderer(main_window_renderer);
		main_window_renderer = NULL;
	}
	#endif
}

#if 0
static void init_texture(void)
{
	assert(main_window_renderer != NULL);

	int bpp = 32; // TODOSDL2
	Uint32 format = bpp == 32 ? SDL_PIXELFORMAT_RGB888 : SDL_PIXELFORMAT_RGB565;
	int scaler_w = scalers[scaler].width;
	int scaler_h = scalers[scaler].height;

	main_window_tex_format = SDL_AllocFormat(format);

	main_window_texture = SDL_CreateTexture(main_window_renderer, format, SDL_TEXTUREACCESS_STREAMING, scaler_w, scaler_h);

	if (main_window_texture == NULL)
	{
		fprintf(stderr, "error: failed to create scaler texture %dx%dx%s: %s\n", scaler_w, scaler_h, SDL_GetPixelFormatName(format), SDL_GetError());
		exit(EXIT_FAILURE);
	}
}
#endif

static void deinit_texture(void)
{
	#if 0
	if (main_window_texture != NULL)
	{
		SDL_DestroyTexture(main_window_texture);
		main_window_texture = NULL;
	}

	if (main_window_tex_format != NULL)
	{
		SDL_FreeFormat(main_window_tex_format);
		main_window_tex_format = NULL;
	}
	#endif
}
#if 0
static int window_get_display_index(void)
{
return 0;//	return SDL_GetWindowDisplayIndex(main_window);
}
#endif
#if 0
static void window_center_in_display(int display_index)
{
	int win_w, win_h;
	SDL_GetWindowSize(main_window, &win_w, &win_h);

	SDL_Rect bounds;
	SDL_GetDisplayBounds(display_index, &bounds);

	SDL_SetWindowPosition(main_window, bounds.x + (bounds.w - win_w) / 2, bounds.y + (bounds.h - win_h) / 2);
}
#endif

void reinit_fullscreen(int new_display)
{

	//fullscreen_display = new_display;

	//if (fullscreen_display >= SDL_GetNumVideoDisplays())
	//{
		fullscreen_display = 0;
	//}
#if 0
	SDL_SetWindowFullscreen(main_window, SDL_FALSE);
	SDL_SetWindowSize(main_window, scalers[scaler].width, scalers[scaler].height);

	if (fullscreen_display == -1)
	{
		window_center_in_display(window_get_display_index());
	}
	else
	{
		window_center_in_display(fullscreen_display);

		if (SDL_SetWindowFullscreen(main_window, SDL_WINDOW_FULLSCREEN_DESKTOP) != 0)
		{
			reinit_fullscreen(-1);
			return;
		}
	}
#endif	
}

void video_on_win_resize(void)
{
	#if 0
	int w, h;
	int scaler_w, scaler_h;

	// Tell video to reinit if the window was manually resized by the user.
	// Also enforce a minimum size on the window.

	SDL_GetWindowSize(main_window, &w, &h);
	scaler_w = scalers[scaler].width;
	scaler_h = scalers[scaler].height;

	if (w < scaler_w || h < scaler_h)
	{
		w = w < scaler_w ? scaler_w : w;
		h = h < scaler_h ? scaler_h : h;

		SDL_SetWindowSize(main_window, w, h);
	}
	#endif
}

void toggle_fullscreen(void)
{
	#if 0
	if (fullscreen_display != -1)
		reinit_fullscreen(-1);
	else
		reinit_fullscreen(SDL_GetWindowDisplayIndex(main_window));
	#endif
}

bool init_scaler(unsigned int new_scaler)
{
	#if 0
	int w = scalers[new_scaler].width,
	h = scalers[new_scaler].height;
	int bpp = main_window_tex_format->BitsPerPixel; // TODOSDL2

	scaler = new_scaler;

	deinit_texture();
	init_texture();

	if (fullscreen_display == -1)
	{
		// Changing scalers, when not in fullscreen mode, forces the window
		// to resize to exactly match the scaler's output dimensions.
		SDL_SetWindowSize(main_window, w, h);
		window_center_in_display(window_get_display_index());
	}

	switch (bpp)
	{
	case 32:
		scaler_function = scalers[scaler].scaler32;
		break;
	case 16:
		scaler_function = scalers[scaler].scaler16;
		break;
	default:
		scaler_function = NULL;
		break;
	}

	if (scaler_function == NULL)
	{
		assert(false);
		return false;
	}
	#endif
	return true;
}

bool set_scaling_mode_by_name(const char *name)
{
	#if 0
	for (int i = 0; i < ScalingMode_MAX; ++i)
	{
		 if (strcmp(name, scaling_mode_names[i]) == 0)
		 {
			 scaling_mode = i;
			 return true;
		 }
	}
	#endif
	return false;
}

void JE_clr256(uint8_t *screen)
{
//	SDL_FillRect(screen, NULL, 0);
memset(screen,0,320*200);
}
display_context_t _dc;

display_context_t lockVideo(int wait)
{
	display_context_t dc;

	if (wait)
	{
		while (!(dc = display_lock()));
	}
	else
	{
		dc = display_lock();
	}

	return dc;
}

void unlockVideo(display_context_t dc)
{
	if (dc)
	{
		display_show(dc);
	}
}
#define USE_64BIT_WRITES 1
extern Uint32 rgb_palette[256];
extern Uint32 tworgb_palette[65536];
void JE_showVGA(void) 
{ 
	_dc = lockVideo(1);

#if USE_64BIT_WRITES
	// Nintendo 64 frame buffer, 64-bit pointer to 20th row
	uint64_t *dst64 = (uint64_t *)((uintptr_t)__safe_buffer[(_dc-1)] + (uintptr_t)12800);
	// OpenTyrian frame buffer, 32-bit pointer to 1st row
	uint32_t *src32 = (uint32_t *)VGAScreen;

	// 64000 8-bit pixels
	// / 
	// 16 pixels per loop
	// = 
	// 4000 iterations
	for (uint n=0;n<4000;n++)
	{
		// read 4 pixels from 8-bit frame buffer
		uint32_t src_four1 = *src32++;
		// read 4 more pixels from 8-bit frame buffer
		uint32_t src_four2 = *src32++;
		// read 4 more pixels from 8-bit frame buffer
		uint32_t src_four3 = *src32++;
		// read 4 more pixels from 8-bit frame buffer
		uint32_t src_four4 = *src32++;
		// color index first two pixels to get 2 16-bit pixels
		uint32_t src12 = tworgb_palette[(src_four1 >> 16)];
		// color index second two pixels to get 2 more 16-bit pixels
		uint32_t src34 = tworgb_palette[(src_four1 & 0xffff)];
		// write 4 16-bit pixels to 16-bit frame buffer
		*dst64++ = (uint64_t)(((uint64_t)src12<<32)|(uint64_t)src34);
		// color index two more pixels to get 2 16-bit pixels
		src12 = tworgb_palette[(src_four2 >> 16)];
		// color index two more pixels to get 2 16-bit pixels
		src34 = tworgb_palette[(src_four2 & 0xffff)];
		// write 4 more 16-bit pixels to 16-bit frame buffer
		*dst64++ = (uint64_t)(((uint64_t)src12<<32)|(uint64_t)src34);
		// color index two more pixels to get 2 16-bit pixels
		src12 = tworgb_palette[(src_four3 >> 16)];
		// color index two more pixels to get 2 16-bit pixels
		src34 = tworgb_palette[(src_four3 & 0xffff)];
		// write 4 more 16-bit pixels to 16-bit frame buffer
		*dst64++ = (uint64_t)(((uint64_t)src12<<32)|(uint64_t)src34);
		// color index two more pixels to get 2 16-bit pixels
		src12 = tworgb_palette[(src_four4 >> 16)];
		// color index two more pixels to get 2 16-bit pixels
		src34 = tworgb_palette[(src_four4 & 0xffff)];
		// write 4 more 16-bit pixels to 16-bit frame buffer
		*dst64++ = (uint64_t)(((uint64_t)src12<<32)|(uint64_t)src34);
	}
#else
	uint32_t *dst32 = (uint32_t *)((uintptr_t)__safe_buffer[(_dc-1)] + (uintptr_t)12800);
	uint32_t *src32 = (uint32_t *)VGAScreen;
	for (uint n=0;n<4000;n++)
	{
		// read 4 8-bit pixels at a time from VGAScreen 
		// write 2 16-bit pixels at a time to Nintendo 64 frame buffer
		// write 2 16-bit pixels at a time to Nintendo 64 frame buffer
		// unrolled 4x
		uint32_t src_four1 = *src32++;
		uint32_t src_four2 = *src32++;
		uint32_t src_four3 = *src32++;
		uint32_t src_four4 = *src32++;
		uint32_t src12 = tworgb_palette[(src_four1 >> 16)];
		uint32_t src34 = tworgb_palette[(src_four1 & 0xffff)];
		*dst32++ = src12;
		*dst32++ = src34;
		src12 = tworgb_palette[(src_four2 >> 16)];
		src34 = tworgb_palette[(src_four2 & 0xffff)];
		*dst32++ = src12;
		*dst32++ = src34;
		src12 = tworgb_palette[(src_four3 >> 16)];
		src34 = tworgb_palette[(src_four3 & 0xffff)];
		*dst32++ = src12;
		*dst32++ = src34;
		src12 = tworgb_palette[(src_four4 >> 16)];
		src34 = tworgb_palette[(src_four4 & 0xffff)];
		*dst32++ = src12;
		*dst32++ = src34;			
	}
#endif
	unlockVideo(_dc);
}

#if 0
static void calc_dst_render_rect(SDL_Surface *const src_surface, SDL_Rect *const dst_rect)
{
	// Decides how the logical output texture (after software scaling applied) will fit
	// in the window.

	int win_w, win_h;
	SDL_GetWindowSize(main_window, &win_w, &win_h);

	int maxh_width, maxw_height;

	switch (scaling_mode)
	{
	case SCALE_CENTER:
		SDL_QueryTexture(main_window_texture, NULL, NULL, &dst_rect->w, &dst_rect->h);
		break;
	case SCALE_INTEGER:
		dst_rect->w = src_surface->w;
		dst_rect->h = src_surface->h;
		while (dst_rect->w + src_surface->w <= win_w && dst_rect->h + src_surface->h <= win_h)
		{
			dst_rect->w += src_surface->w;
			dst_rect->h += src_surface->h;
		}
		break;
	case SCALE_ASPECT_8_5:
		maxh_width = win_h * (8.f / 5.f);
		maxw_height = win_w * (5.f / 8.f);

		if (maxh_width > win_w)
		{
			dst_rect->w = win_w;
			dst_rect->h = maxw_height;
		}
		else
		{
			dst_rect->w = maxh_width;
			dst_rect->h = win_h;
		}
		break;
	case SCALE_ASPECT_4_3:
		maxh_width = win_h * (4.f / 3.f);
		maxw_height = win_w * (3.f / 4.f);

		if (maxh_width > win_w)
		{
			dst_rect->w = win_w;
			dst_rect->h = maxw_height;
		}
		else
		{
			dst_rect->w = maxh_width;
			dst_rect->h = win_h;
		}
		break;
	case ScalingMode_MAX:
		assert(false);
		break;
	}

	dst_rect->x = (win_w - dst_rect->w) / 2;
	dst_rect->y = (win_h - dst_rect->h) / 2;
}

static void scale_and_flip(SDL_Surface *src_surface)
{
	assert(src_surface->format->BitsPerPixel == 8);

	// Do software scaling
	assert(scaler_function != NULL);
	scaler_function(src_surface, main_window_texture);

	SDL_Rect dst_rect;
	calc_dst_render_rect(src_surface, &dst_rect);

	// Clear the window and blit the output texture to it
	SDL_SetRenderDrawColor(main_window_renderer, 0, 0, 0, 255);
	SDL_RenderClear(main_window_renderer);
	SDL_RenderCopy(main_window_renderer, main_window_texture, NULL, &dst_rect);
	SDL_RenderPresent(main_window_renderer);

	// Save output rect to be used by mouse functions
	last_output_rect = dst_rect;
}

/** Maps a specified point in game screen coordinates to window coordinates. */
void mapScreenPointToWindow(Sint32 *const inout_x, Sint32 *const inout_y)
{
	*inout_x = (2 * *inout_x + 1) * last_output_rect.w / (2 * screenwidth) + last_output_rect.x;
	*inout_y = (2 * *inout_y + 1) * last_output_rect.h / (2 * screenheight) + last_output_rect.y;
}

/** Maps a specified point in window coordinates to game screen coordinates. */
void mapWindowPointToScreen(Sint32 *const inout_x, Sint32 *const inout_y)
{
	*inout_x = (2 * (*inout_x - last_output_rect.x) + 1) * screenwidth / (2 * last_output_rect.w);
	*inout_y = (2 * (*inout_y - last_output_rect.y) + 1) * screenheight / (2 * last_output_rect.h);
}

/** Scales a distance in window coordinates to game screen coordinates. */
void scaleWindowDistanceToScreen(Sint32 *const inout_x, Sint32 *const inout_y)
{
	*inout_x = (2 * *inout_x + 1) * screenwidth / (2 * last_output_rect.w);
	*inout_y = (2 * *inout_y + 1) * screenheight / (2 * last_output_rect.h);
}
#endif