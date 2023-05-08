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
#include "keyboard.h"

#include "joystick.h"
//#include "mouse.h"
#include "network.h"
#include "opentyr.h"
#include "video.h"
#include "video_scale.h"

//#include "SDL.h"
#include "sdl_scancode.h"
#include <stdio.h>
extern size_t iq_len;
extern size_t iq_start;

JE_boolean ESCPressed;

JE_boolean newkey, newmouse, keydown, mousedown;
SDL_Scancode lastkey_scan;
//SDL_Keymod 
int lastkey_mod;
Uint8 lastmouse_but;
Sint32 lastmouse_x, lastmouse_y;
JE_boolean mouse_pressed[3] = {false, false, false};
Sint32 mouse_x, mouse_y;

bool windowHasFocus;

Uint8 keysactive[SDL_NUM_SCANCODES];

bool new_text;
char last_text[128];//SDL_TEXTINPUTEVENT_TEXT_SIZE];

static bool mouseRelativeEnabled;

// Relative mouse position in window coordinates.
static Sint32 mouseWindowXRelative;
static Sint32 mouseWindowYRelative;

void flush_events_buffer(void)
{
#if 0
	SDL_Event ev;

	while (SDL_PollEvent(&ev));
#endif
}

void wait_input(JE_boolean keyboard, JE_boolean mouse, JE_boolean joystick)
{
#if 1	
	service_SDL_events(false);
	while (!((keyboard && keydown))) // || (mouse && mousedown) || (joystick && joydown)))
	{
		n64_Delay(SDL_POLL_INTERVAL);
		//push_joysticks_as_keyboard();
		service_SDL_events(false);

#ifdef WITH_NETWORK
		if (isNetworkGame)
			network_check();
#endif
	}
#endif	
}

void wait_noinput(JE_boolean keyboard, JE_boolean mouse, JE_boolean joystick)
{
#if 1	
	service_SDL_events(false);
	while ((keyboard && keydown))// || (mouse && mousedown) || (joystick && joydown))
	{
		n64_Delay(SDL_POLL_INTERVAL);
//		poll_joysticks();
		service_SDL_events(false);

#ifdef WITH_NETWORK
		if (isNetworkGame)
			network_check();
#endif
	}
#endif	
}

void init_keyboard(void)
{

	//SDL_EnableKeyRepeat(500, 60); TODO Find if SDL2 has an equivalent.

	newkey = newmouse = false;
	keydown = mousedown = false;
#if 0
	SDL_ShowCursor(SDL_FALSE);

#if SDL_VERSION_ATLEAST(2, 26, 0)
	SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_SYSTEM_SCALE, "1");
#endif
#endif
}

void mouseSetRelative(bool enable)
{
#if 0
	SDL_SetRelativeMouseMode(enable && windowHasFocus);

	mouseRelativeEnabled = enable;

	mouseWindowXRelative = 0;
	mouseWindowYRelative = 0;
#endif
}

JE_word JE_mousePosition(JE_word *mouseX, JE_word *mouseY)
{
#if 0	
	service_SDL_events(false);
	*mouseX = mouse_x;
	*mouseY = mouse_y;
	return mousedown ? lastmouse_but : 0;
#endif
return 0;
}

void mouseGetRelativePosition(Sint32 *const out_x, Sint32 *const out_y)
{
#if 0	
	service_SDL_events(false);

	scaleWindowDistanceToScreen(&mouseWindowXRelative, &mouseWindowYRelative);
	*out_x = mouseWindowXRelative;
	*out_y = mouseWindowYRelative;

	mouseWindowXRelative = 0;
	mouseWindowYRelative = 0;
#endif
}
extern input_event_t input_queue[1024];

void service_SDL_events(JE_boolean clear_new)
{
//	SDL_Event ev;

	if (clear_new)
	{
		newkey = false;
		newmouse = false;
		new_text = false;
	}
#if 0
	struct controller_data keys_pressed;
	struct controller_data keys_held;
	struct controller_data keys_released;

	controller_scan();

	keys_pressed = get_keys_down();
	keys_held = get_keys_held();
	keys_released = get_keys_up();

	struct SI_condat pressed = keys_pressed.c[0];
	struct SI_condat held = keys_held.c[0];

//keydown = false;
	if (pressed.A || held.A) {
		keysactive[SDL_SCANCODE_SPACE] = 1;

		newkey = true;
		lastkey_scan = SDL_SCANCODE_SPACE;
		lastkey_mod = 0;
		keydown = true;

//		//mouseInactive = true;
		return;
	}
	if (pressed.B || held.B) {
		keysactive[SDL_SCANCODE_RETURN] = 1;

		newkey = true;
		lastkey_scan = SDL_SCANCODE_RETURN;
		lastkey_mod = 0;
		keydown = true;

//		//mouseInactive = true;
		return;
	}
//	if (pressed.C_up) {
//		input.lastChar = KEY_c;
//		input.code = true;
//	}
	if (pressed.up || held.up) {
		keysactive[SDL_SCANCODE_UP] = 1;

		newkey = true;
		lastkey_scan = SDL_SCANCODE_UP;
		lastkey_mod = 0;
		keydown = true;

		//mouseInactive = true;
		return;
	}
	if (pressed.down || held.down) {
		keysactive[SDL_SCANCODE_DOWN] = 1;

		newkey = true;
		lastkey_scan = SDL_SCANCODE_DOWN;
		lastkey_mod = 0;
		keydown = true;

		//mouseInactive = true;
		return;
	}
	if (pressed.left || held.left) {
		keysactive[SDL_SCANCODE_LEFT] = 1;

		newkey = true;
		lastkey_scan = SDL_SCANCODE_LEFT;
		lastkey_mod = 0;
		keydown = true;

		//mouseInactive = true;
		return;
	}
	if (pressed.right || held.right) {
		keysactive[SDL_SCANCODE_RIGHT] = 1;

		newkey = true;
		lastkey_scan = SDL_SCANCODE_RIGHT;
		lastkey_mod = 0;
		keydown = true;

		//mouseInactive = true;
		return;
	}
	if (pressed.start || held.start) {
		keysactive[SDL_SCANCODE_ESCAPE] = 1;

		newkey = true;
		lastkey_scan = SDL_SCANCODE_ESCAPE;
		lastkey_mod = 0;
		keydown = true;

		//mouseInactive = true;
		return;
	}

	struct SI_condat released = keys_released.c[0];

	if (released.A) {
		keysactive[SDL_SCANCODE_SPACE] = 0;
		keydown = false;
		//mouseInactive = true;
		return;
	}
	if (released.B) {
		keysactive[SDL_SCANCODE_RETURN] = 0;
		keydown = false;
		//mouseInactive = true;
		return;
	}
	if (released.up) {
		keysactive[SDL_SCANCODE_UP] = 0;
		keydown = false;
		//mouseInactive = true;
		return;
	}
	if (released.down) {
		keysactive[SDL_SCANCODE_DOWN] = 0;
		keydown = false;
		//mouseInactive = true;
		return;
	}
	if (released.left) {
		keysactive[SDL_SCANCODE_LEFT] = 0;
		keydown = false;
		//mouseInactive = true;
		return;
	}
	if (released.right) {
		keysactive[SDL_SCANCODE_RIGHT] = 0;
		keydown = false;
		//mouseInactive = true;
		return;
	}
	if (released.start) {
		keysactive[SDL_SCANCODE_ESCAPE] = 0;
		keydown = false;
		//mouseInactive = true;
		return;
	}
#endif
//	for(size_t i=iq_start;i<iq_len;i++) {
//		printf("%d %d\n", input_queue[i].down, input_queue[i].key);
//	}
	input_event_t ev;
	while (pop_input_queue(&ev))
	{
		// released
		if (ev.down == 0)
		{  
			if (ev.key == ctrlr_up) {
				keysactive[SDL_SCANCODE_UP] = 0;
				keydown = false;
				return;
			}
			if (ev.key == ctrlr_down) {
				keysactive[SDL_SCANCODE_DOWN] = 0;
				keydown = false;
				return;
			}
			if (ev.key == ctrlr_left) {
				keysactive[SDL_SCANCODE_LEFT] = 0;
				keydown = false;
				return;
			}
			if (ev.key == ctrlr_right) {
				keysactive[SDL_SCANCODE_RIGHT] = 0;
				keydown = false;
				return;
			}
			if (ev.key == ctrlr_l) {
				keysactive[SDL_SCANCODE_LCTRL] = 0;
				keydown = false;
				return;
			}
			if (ev.key == ctrlr_r) {
				keysactive[SDL_SCANCODE_LALT] = 0;
				keydown = false;
				return;
			}
			if (ev.key == ctrlr_a) {
				keysactive[SDL_SCANCODE_SPACE] = 0;
				keydown = false;
				return;
			}
			if (ev.key == ctrlr_b) {
				keysactive[SDL_SCANCODE_RETURN] = 0;
				keydown = false;
				return;
			}
			if (ev.key == ctrlr_start) {
				keysactive[SDL_SCANCODE_PAUSE] = 0;
				keydown = false;
				return;
			}	
			return;
		}
		// pressed
		else if (ev.down) 
		{
			if (ev.key == ctrlr_up) {
				keysactive[SDL_SCANCODE_UP] = 1;
				newkey = true;
				lastkey_scan = SDL_SCANCODE_UP;
				lastkey_mod = 0;
				keydown = true;
				return;
			}
			if (ev.key == ctrlr_down) {
				keysactive[SDL_SCANCODE_DOWN] = 1;
				newkey = true;
				lastkey_scan = SDL_SCANCODE_DOWN;
				lastkey_mod = 0;
				keydown = true;
				return;
			}
			if (ev.key == ctrlr_left) {
				keysactive[SDL_SCANCODE_LEFT] = 1;
				newkey = true;
				lastkey_scan = SDL_SCANCODE_LEFT;
				lastkey_mod = 0;
				keydown = true;
				return;
			}
			if (ev.key == ctrlr_right) {
				keysactive[SDL_SCANCODE_RIGHT] = 1;
				newkey = true;
				lastkey_scan = SDL_SCANCODE_RIGHT;
				lastkey_mod = 0;
				keydown = true;
				return;
			}
			if (ev.key == ctrlr_l) {
				keysactive[SDL_SCANCODE_LCTRL] = 1;
				newkey = true;
				lastkey_scan = SDL_SCANCODE_LCTRL;
				lastkey_mod = 0;
				keydown = true;
				return;
			}
			if (ev.key == ctrlr_r) {
				keysactive[SDL_SCANCODE_LALT] = 1;
				newkey = true;
				lastkey_scan = SDL_SCANCODE_LALT;
				lastkey_mod = 0;
				keydown = true;
				return;
			}
			if (ev.key == ctrlr_a) {
				keysactive[SDL_SCANCODE_SPACE] = 1;
				newkey = true;
				lastkey_scan = SDL_SCANCODE_SPACE;
				lastkey_mod = 0;
				keydown = true;
				return;
			}
			if (ev.key == ctrlr_b) {
				keysactive[SDL_SCANCODE_RETURN] = 1;
				newkey = true;
				lastkey_scan = SDL_SCANCODE_RETURN;
				lastkey_mod = 0;
				keydown = true;
				return;
			}
			if (ev.key == ctrlr_start) {
				keysactive[SDL_SCANCODE_PAUSE] = 1;
				newkey = true;
				lastkey_scan = SDL_SCANCODE_PAUSE;
				lastkey_mod = 0;
				keydown = true;
				return;
			}	
			return;
		}
	}
	/*
				case SDL_SCANCODE_UP:
				{
					JE_playSampleNum(S_CURSOR);

					const size_t pickerItemsCount = selectedMenuItem->getPickerItemsCount();

					pickerSelectedIndex = pickerSelectedIndex == 0
						? pickerItemsCount - 1
						: pickerSelectedIndex - 1;
					break;
				}
				case SDL_SCANCODE_DOWN:
				{
					JE_playSampleNum(S_CURSOR);

					const size_t pickerItemsCount = selectedMenuItem->getPickerItemsCount();

					pickerSelectedIndex = pickerSelectedIndex == pickerItemsCount - 1
						? 0
						: pickerSelectedIndex + 1;
					break;
				}
				case SDL_SCANCODE_SPACE:
				case SDL_SCANCODE_RETURN:
				{
					action = true;
					break;
				}
				case SDL_SCANCODE_ESCAPE:
*/
#if 0
	while (SDL_PollEvent(&ev))
	{
		switch (ev.type)
		{
			case SDL_WINDOWEVENT:
				switch (ev.window.event)
				{
				case SDL_WINDOWEVENT_FOCUS_LOST:
					windowHasFocus = false;

					mouseSetRelative(mouseRelativeEnabled);
					break;

				case SDL_WINDOWEVENT_FOCUS_GAINED:
					windowHasFocus = true;

					mouseSetRelative(mouseRelativeEnabled);
					break;

				case SDL_WINDOWEVENT_RESIZED:
					video_on_win_resize();
					break;
				}
				break;

			case SDL_KEYDOWN:
				/* <alt><enter> toggle fullscreen */
				if (ev.key.keysym.mod & KMOD_ALT && ev.key.keysym.scancode == SDL_SCANCODE_RETURN)
				{
					toggle_fullscreen();
					break;
				}

				keysactive[ev.key.keysym.scancode] = 1;

				newkey = true;
				lastkey_scan = ev.key.keysym.scancode;
				lastkey_mod = ev.key.keysym.mod;
				keydown = true;

				//mouseInactive = true;
				return;

			case SDL_KEYUP:
				keysactive[ev.key.keysym.scancode] = 0;
				keydown = false;
				return;

			case SDL_MOUSEMOTION:
				mouse_x = ev.motion.x;
				mouse_y = ev.motion.y;
				mapWindowPointToScreen(&mouse_x, &mouse_y);

				if (mouseRelativeEnabled && windowHasFocus)
				{
					mouseWindowXRelative += ev.motion.xrel;
					mouseWindowYRelative += ev.motion.yrel;
				}

				// Show system mouse pointer if outside screen.
				SDL_ShowCursor(mouse_x < 0 || mouse_x >= vga_width ||
				               mouse_y < 0 || mouse_y >= vga_height ? SDL_TRUE : SDL_FALSE);

				if (ev.motion.xrel != 0 || ev.motion.yrel != 0)
					//mouseInactive = false;
				break;

			case SDL_MOUSEBUTTONDOWN:
				//mouseInactive = false;

				// fall through
			case SDL_MOUSEBUTTONUP:
				mapWindowPointToScreen(&ev.button.x, &ev.button.y);
				if (ev.type == SDL_MOUSEBUTTONDOWN)
				{
					newmouse = true;
					lastmouse_but = ev.button.button;
					lastmouse_x = ev.button.x;
					lastmouse_y = ev.button.y;
					mousedown = true;
				}
				else
				{
					mousedown = false;
				}
				switch (ev.button.button)
				{
					case SDL_BUTTON_LEFT:
						mouse_pressed[0] = mousedown; break;
					case SDL_BUTTON_RIGHT:
						mouse_pressed[1] = mousedown; break;
					case SDL_BUTTON_MIDDLE:
						mouse_pressed[2] = mousedown; break;
				}
				break;

			case SDL_TEXTINPUT:
				SDL_strlcpy(last_text, ev.text.text, COUNTOF(last_text));
				new_text = true;
				break;

			case SDL_TEXTEDITING:
				break;

			case SDL_QUIT:
				/* TODO: Call the cleanup code here. */
				exit(0);
				break;
		}
	}
#endif	
}

void JE_clearKeyboard(void)
{
	// /!\ Doesn't seems important. I think. D:
}
