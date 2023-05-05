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
#ifndef FILE_H
#define FILE_H
#include <libdragon.h>
//#include "SDL_endian.h"
#include "opentyr.h"
#include <stdbool.h>
#include <stdio.h>

extern const char *custom_data_dir;

const char *data_dir(void);

int dir_fopen(const char *dir, const char *file, const char *mode);
int dir_fopen_warn(const char *dir, const char *file, const char *mode);
int dir_fopen_die(const char *dir, const char *file, const char *mode);

bool dir_file_exists(const char *dir, const char *file);

long ftell_eof(int f);

void fread_die(void *buffer, size_t size, size_t count, int stream);

// 8-bit fread that dies if read fails
static inline void fread_bool_die(bool *buffer, int stream)
{
	Uint8 temp;
	fread_die(&temp, sizeof(Uint8), 1, stream);
	*buffer = temp != 0;
}

// 8-bit fread
static inline size_t fread_u8(Uint8 *buffer, size_t count, int stream)
{
//	return fread(buffer, sizeof(Uint8), count, stream);
	return dfs_read(buffer, sizeof(Uint8), count, stream);
}

// 8-bit fread that dies if read fails
static inline void fread_u8_die(Uint8 *buffer, size_t count, int stream)
{
	fread_die(buffer, sizeof(Uint8), count, stream);
}

// 8-bit fread that dies if read fails
static inline void fread_s8_die(Sint8 *buffer, size_t count, int stream)
{
	fread_die(buffer, sizeof(Sint8), count, stream);
}

// 16-bit endian-swapping fread that dies if read fails
static inline void fread_u16_die(Uint16 *buffer, size_t count, int stream)
{
	fread_die(buffer, sizeof(Uint16), count, stream);

	for (size_t i = 0; i < count; ++i)
		buffer[i] = SHORT(buffer[i]);
}

// 16-bit endian-swapping fread that dies if read fails
static inline void fread_s16_die(Sint16 *buffer, size_t count, int stream)
{
	fread_die(buffer, sizeof(Sint16), count, stream);

	for (size_t i = 0; i < count; ++i)
		buffer[i] = SHORT(buffer[i]);
}

// 32-bit endian-swapping fread that dies if read fails
static inline void fread_u32_die(Uint32 *buffer, size_t count, int stream)
{
	fread_die(buffer, sizeof(Uint32), count, stream);

	for (size_t i = 0; i < count; ++i)
		buffer[i] = LONG(buffer[i]);
}

// 32-bit endian-swapping fread that dies if read fails
static inline void fread_s32_die(Sint32 *buffer, size_t count, int stream)
{
	fread_die(buffer, sizeof(Sint32), count, stream);

	for (size_t i = 0; i < count; ++i)
		buffer[i] = LONG(buffer[i]);
}

void fwrite_die(const void *buffer, size_t size, size_t count, int stream);

// 8-bit fwrite that dies if write fails
static inline void fwrite_bool_die(const bool *buffer, int stream)
{
	Uint8 temp = *buffer ? 1 : 0;
	fwrite_die(&temp, sizeof(Uint8), 1, stream);
}

// 8-bit fwrite
static inline size_t fwrite_u8(const Uint8 *buffer, size_t count, int stream)
{
	return -1;//return fwrite(buffer, sizeof(Uint8), count, stream);
}

// 8-bit fwrite that dies if write fails
static inline void fwrite_u8_die(const Uint8 *buffer, size_t count, int stream)
{
	fwrite_die(buffer, sizeof(Uint8), count, stream);
}

// 8-bit fwrite that dies if write fails
static inline void fwrite_s8_die(const Sint8 *buffer, size_t count, int stream)
{
	fwrite_die(buffer, sizeof(Sint8), count, stream);
}

// 16-bit endian-swapping fwrite that dies if write fails
static inline void fwrite_u16_die(const Uint16 *buffer, int stream)
{
	Uint16 temp = SHORT(*buffer);
	buffer = &temp;

	fwrite_die(buffer, sizeof(Uint16), 1, stream);
}

// 16-bit endian-swapping fwrite that dies if write fails
static inline void fwrite_s16_die(const Sint16 *buffer, int stream)
{
	Sint16 temp = SHORT(*buffer);
	buffer = &temp;

	fwrite_die(buffer, sizeof(Sint16), 1, stream);
}

// 32-bit endian-swapping fwrite that dies if write fails
static inline void fwrite_u32_die(const Uint32 *buffer, int stream)
{
	Uint32 temp = LONG(*buffer);
	buffer = &temp;

	fwrite_die(buffer, sizeof(Uint32), 1, stream);
}

// 32-bit endian-swapping fwrite that dies if write fails
static inline void fwrite_s32_die(const Sint32 *buffer, int stream)
{
	Sint32 temp = LONG(*buffer);
	buffer = &temp;

	fwrite_die(buffer, sizeof(Sint32), 1, stream);
}

#endif // FILE_H
