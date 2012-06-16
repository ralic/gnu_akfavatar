/*
 * AKFAvatar - library for showing an avatar who says things in a balloon
 * this part contains dummy audio functions
 * Copyright (c) 2007,2008,2009,2010,2011,2012 Andreas K. Foerster <info@akfoerster.de>
 *
 * required standards: C99 or C++
 *
 * other software
 * required: SDL1.2
 *
 * This file is part of AKFAvatar
 *
 * AKFAvatar is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * AKFAvatar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

/* don't make functions deprecated for this file */
#define _AVT_USE_DEPRECATED

#include "akfavatar.h"
#include "avtinternals.h"
#include "SDL.h"		/* for SDL_SetError */

static void
no_audio (void)
{
  SDL_SetError ("not compiled with audio support");
}

extern int
avt_initialize_audio (void)
{
  no_audio ();
  /* do not set _avt_STATUS here */
  return AVT_FAILURE;
}

extern int
avt_start_audio (void)
{
  no_audio ();
  /* do not set _avt_STATUS here */
  return AVT_FAILURE;
}

extern void
avt_quit_audio (void)
{
  no_audio ();
}

extern void
avt_stop_audio (void)
{
  no_audio ();
}

extern avt_audio *
avt_load_audio_file (const char *filename, int playmode)
{
  no_audio ();
  return NULL;
}

extern avt_audio *
avt_load_audio_part (avt_stream * stream, size_t maxsize, int playmode)
{
  no_audio ();
  return NULL;
}

extern avt_audio *
avt_load_audio_stream (avt_stream * stream, int playmode)
{
  no_audio ();
  return NULL;
}

extern avt_audio *
avt_load_audio_data (void *data, size_t datasize, int playmode)
{
  no_audio ();
  return NULL;
}

extern avt_audio *
avt_load_raw_audio_data (void *data, size_t data_size,
			 int samplingrate, int audio_type, int channels)
{
  no_audio ();
  return NULL;
}

extern int
avt_add_raw_audio_data (avt_audio * snd, void *data, size_t data_size)
{
  no_audio ();
  return AVT_FAILURE;
}

extern int
avt_set_raw_audio_capacity (avt_audio * snd, size_t data_size)
{
  no_audio ();
  return AVT_FAILURE;
}

extern void
avt_finalize_raw_audio (avt_audio * snd)
{
}

extern void
avt_free_audio (avt_audio * snd)
{
  no_audio ();
}

extern int
avt_wait_audio_end (void)
{
  no_audio ();
  return _avt_STATUS;
}

extern int
avt_play_audio (avt_audio * snd, int playmode)
{
  no_audio ();
  return _avt_STATUS;
}

extern void
avt_pause_audio (bool pause)
{
  no_audio ();
}

extern bool
avt_audio_playing (avt_audio * snd)
{
  no_audio ();
  return false;
}