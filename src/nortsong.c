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
#include "nortsong.h"

#include "file.h"
#include "joystick.h"
#include "keyboard.h"
#include "loudness.h"
#include "musmast.h"
#include "opentyr.h"
#include "params.h"
#include "sndmast.h"
#include "vga256d.h"


JE_word frameCountMax;

Sint16 *soundSamples[SOUND_COUNT] = { NULL }; /* [1..soundnum + 9] */  // FKA digiFx
size_t soundSampleCount[SOUND_COUNT] = { 0 }; /* [1..soundnum + 9] */  // FKA fxSize

JE_word tyrMusicVolume, fxVolume;
const JE_word fxPlayVol = 4;
JE_word tempVolume;

// The period of the x86 programmable interval timer in milliseconds.
static const float pitPeriod = (12.0f / 14318180.0f) * 1000.0f;

static Uint16 delaySpeed = 0x4300;
static float delayPeriod = 0x4300 * ((12.0f / 14318180.0f) * 1000.0f);

static Uint32 target = 0;
static Uint32 target2 = 0;

void setDelay(int delay)  // FKA NortSong.frameCount
{
	target = n64_GetTicks() + delay * delayPeriod;
}

void setDelay2(int delay)  // FKA NortSong.frameCount2
{
	target2 = n64_GetTicks() + delay * delayPeriod;
}

Uint32 getDelayTicks(void)  // FKA NortSong.frameCount
{
	// FIXME
	Sint32 delay = target - n64_GetTicks();
	return MAX(0, delay);
}

Uint32 getDelayTicks2(void)  // FKA NortSong.frameCount2
{
	//FIXME
	Sint32 delay = target2 - n64_GetTicks();
	return MAX(0, delay);
}

void wait_delay(void)
{
	//FIXME
	Sint32 delay = target - n64_GetTicks();
	if (delay > 0)
		n64_Delay(delay);
}

void service_wait_delay(void)
{
	for (; ; )
	{
		service_SDL_events(false);
		Sint32 delay = target - n64_GetTicks();
		if (delay <= 0)
			return;

		n64_Delay(MIN(delay, SDL_POLL_INTERVAL));
	}
}

void wait_delayorinput(void)
{
	for (; ; )
	{
		service_SDL_events(false);
		poll_joysticks();

		if (newkey || mousedown || joydown)
		{
			newkey = false;
			return;
		}
// FIXME
		Sint32 delay = target - n64_GetTicks();
		if (delay <= 0)
			return;

		n64_Delay(MIN(delay, SDL_POLL_INTERVAL));
	}
}

void loadSndFile(bool xmas)
{
	int f;

	f = dir_fopen_die(data_dir(), "tyrian.snd", "rb");

	Uint16 sfxCount;
	Uint32 sfxPositions[SFX_COUNT + 1];

	// Read number of sounds.
	fread_u16_die(&sfxCount, 1, f);
	if (sfxCount != SFX_COUNT)
		goto die;

	// Read positions of sounds.
	fread_u32_die(sfxPositions, sfxCount, f);

	// Determine end of last sound.
	dfs_seek(f, 0, SEEK_END);
	sfxPositions[sfxCount] = dfs_tell(f);

	// Read samples.
	for (size_t i = 0; i < sfxCount; ++i)
	{
		soundSampleCount[i] = sfxPositions[i + 1] - sfxPositions[i];

		// Sound size cannot exceed 64 KiB.
		if (soundSampleCount[i] > UINT16_MAX)
			goto die;

		if(soundSamples[i])
		{
			free(soundSamples[i]);
		}
		soundSamples[i] = malloc(soundSampleCount[i]);

		dfs_seek(f, sfxPositions[i], SEEK_SET);
		fread_u8_die((Uint8 *)soundSamples[i], soundSampleCount[i], f);
	}

	dfs_close(f);

	f = dir_fopen_die(data_dir(), xmas ? "voicesc.snd" : "voices.snd", "rb");

	Uint16 voiceCount;
	Uint32 voicePositions[VOICE_COUNT + 1];

	// Read number of sounds.
	fread_u16_die(&voiceCount, 1, f);
	if (voiceCount != VOICE_COUNT)
		goto die;

	// Read positions of sounds.
	fread_u32_die(voicePositions, voiceCount, f);

	// Determine end of last sound.
	dfs_seek(f, 0, SEEK_END);
	voicePositions[voiceCount] = dfs_tell(f);

	for (size_t vi = 0; vi < voiceCount; ++vi)
	{
		size_t i = SFX_COUNT + vi;

		soundSampleCount[i] = voicePositions[vi + 1] - voicePositions[vi];

		// Voice sounds have some bad data at the end.
		soundSampleCount[i] = soundSampleCount[i] >= 100
			? soundSampleCount[i] - 100
			: 0;

		// Sound size cannot exceed 64 KiB.
		if (soundSampleCount[i] > UINT16_MAX)
			goto die;

		if(soundSamples[i])
		{
			free(soundSamples[i]);
		}
		soundSamples[i] = malloc(soundSampleCount[i]);

		dfs_seek(f, voicePositions[vi], SEEK_SET);
		fread_u8_die((Uint8 *)soundSamples[i], soundSampleCount[i], f);
	}

	dfs_close(f);

	// Convert samples to output sample format and rate.
	//SDL_AudioCVT cvt;
	//if (SDL_BuildAudioCVT(&cvt, AUDIO_S8, 1, 11025, AUDIO_S16SYS, 1, audioSampleRate) < 0)
	//{
	//	fprintf(stderr, "error: Failed to build audio converter: %s\n", SDL_GetError());

	//	for (int i = 0; i < SOUND_COUNT; ++i)
	//		soundSampleCount[i] = 0;

	//	return;
	//}
	Sint16 *cvt_buf;
	size_t maxSampleSize = 0;
	for (size_t i = 0; i < SOUND_COUNT; ++i)
	{
		maxSampleSize = MAX(maxSampleSize, soundSampleCount[i]);
	}

	cvt_buf = malloc(maxSampleSize * 2);

	for (size_t i = 0; i < SOUND_COUNT; ++i)
	{
		size_t cvt_len = soundSampleCount[i];
//		memcpy(cvt_buf, soundSamples[i], cvt_len);

//		if (SDL_ConvertAudio(&cvt))
//		{
//			fprintf(stderr, "error: Failed to convert audio: %s\n", SDL_GetError());
//
//			soundSampleCount[i] = 0;
//
//			continue;
//		}

// 11025
		for (size_t samp_i = 0; samp_i < cvt_len; samp_i++)
		{
			Sint16 s1 = *(Sint8 *)(((Sint8*)soundSamples[i]) + samp_i)*255;
			cvt_buf[samp_i] = s1;
		}

		free(soundSamples[i]);
		soundSamples[i] = malloc(cvt_len * 2);

		memcpy(soundSamples[i], cvt_buf, cvt_len * 2);
		soundSampleCount[i] = cvt_len;//*2 / sizeof (Sint16);
	}

	free(cvt_buf);
	return;

die:
	fprintf(stderr, "error: Unexpected data was read from a file.\n");
//	SDL_Quit();
	exit(EXIT_FAILURE);
}

void JE_playSampleNum(JE_byte samplenum)
{
	multiSamplePlay(soundSamples[samplenum-1], soundSampleCount[samplenum-1], 0, fxPlayVol);
}

void setDelaySpeed(Uint16 speed)  // FKA NortSong.speed and NortSong.setTimerInt
{
	delaySpeed = speed;
	delayPeriod = speed * pitPeriod;
}

void JE_changeVolume(JE_word *music, int music_delta, JE_word *sample, int sample_delta)
{
	int music_temp = *music + music_delta,
	    sample_temp = *sample + sample_delta;
	
	if (music_delta)
	{
		if (music_temp > 255)
		{
			music_temp = 255;
			JE_playSampleNum(S_CLINK);
		}
		else if (music_temp < 0)
		{
			music_temp = 0;
			JE_playSampleNum(S_CLINK);
		}
	}
	
	if (sample_delta)
	{
		if (sample_temp > 255)
		{
			sample_temp = 255;
			JE_playSampleNum(S_CLINK);
		}
		else if (sample_temp < 0)
		{
			sample_temp = 0;
			JE_playSampleNum(S_CLINK);
		}
	}
	
	*music = music_temp;
	*sample = sample_temp;
	
	set_volume(*music, *sample);
}
