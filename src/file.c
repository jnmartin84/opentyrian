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
#include "file.h"

#include "opentyr.h"
#include "varz.h"

//#include "SDL.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *custom_data_dir = NULL;

// finds the Tyrian data directory
const char *data_dir(void)
{
#if 0	
	const char *const dirs[] =
	{
		custom_data_dir,
		TYRIAN_DIR,
		"data",
		".",
	};

	static const char *dir = NULL;

	if (dir != NULL)
		return dir;

	for (uint i = 0; i < COUNTOF(dirs); ++i)
	{
		if (dirs[i] == NULL)
			continue;

		int f = dir_fopen(dirs[i], "tyrian1.lvl", "rb");
		if (f > -1)
		{
			dfs_close(f);

			dir = dirs[i];
			break;
		}
	}

	if (dir == NULL) // data not found
		dir = "";
#endif
	return "";//dir;
}

// prepend directory and fopen
int dir_fopen(const char *dir, const char *file, const char *mode)
{
	int f = dfs_open(file);
	return f;
}

// warn when dir_fopen fails
int dir_fopen_warn(const char *dir, const char *file, const char *mode)
{
	int f = dir_fopen(dir, file, mode);
//	if (f < 0)
//		printf("warning: failed to open '%s': %s\n", file, strerror(errno));
	return f;
}

// die when dir_fopen fails
int dir_fopen_die(const char *dir, const char *file, const char *mode)
{
	int f = dir_fopen(dir, file, mode);
	if (f < 0)
	{
		while(1)
		{		
			printf("error: failed to open '%s': %s\n", file, strerror(errno));
			printf("error: One or more of the required Tyrian " TYRIAN_VERSION " data files could not be found.\n"
		           "       Please read the README file.\n");
		}
		JE_tyrianHalt(1);
	}

	return f;
}

// check if file can be opened for reading
bool dir_file_exists(const char *dir, const char *file)
{
	int f = dir_fopen(dir, file, "rb");
	if (f > -1)
		dfs_close(f);
	return (f > -1);
}

// returns end-of-file position
long ftell_eof(int f)
{
	long pos = dfs_tell(f);

	dfs_seek(f, 0, SEEK_END);
	long size = dfs_tell(f);

	dfs_seek(f, pos, SEEK_SET);

	return size;
}

void fread_die(void *buffer, size_t size, size_t count, int stream)
{
	size_t result = dfs_read(buffer, size, count, stream);
	if (result != (size*count))
	{
		while(1)
		{
			printf("error: An unexpected problem occurred while reading from a file.\n");
		}
		exit(EXIT_FAILURE);
	}
}

void fwrite_die(const void *buffer, size_t size, size_t count, int stream)
{
/*	size_t result = fwrite(buffer, size, count, stream);
	if (result != count)
	{
		fprintf(stderr, "error: An unexpected problem occurred while writing to a file.\n");
		SDL_Quit();
		exit(EXIT_FAILURE);
	}*/
}
