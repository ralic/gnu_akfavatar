/*
 * AKFAvatar - library for showing an avatar who says things in a balloon
 * this part is for audio, independent of the backend
 * Copyright (c) 2007,2008,2009,2010,2011,2012,2013,2014,2015
 * Andreas K. Foerster <akf@akfoerster.de>
 *
 * required standards: C99
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

#define _ISOC99_SOURCE
#define _XOPEN_SOURCE 600

// don't make functions deprecated for this file
#define _AVT_USE_DEPRECATED

#include "akfavatar.h"
#include "avtinternals.h"


#ifndef NO_AUDIO

#include "avtdata.h"
#include <stdlib.h>		// malloc / calloc / realloc / free
#include <stdint.h>
#include <string.h>		// memcmp / memcpy / memset
#include <iso646.h>
#include <unistd.h>		// evtl. defines _POSIX_MAPPED_FILES

//#undef _POSIX_MAPPED_FILES

#include "alert.c"

#if _POSIX_MAPPED_FILES+0 > 0
#include <sys/mman.h>
#define avt_munmap(addr, length)  munmap(addr, length)
#else // no mmap
#define avt_munmap(addr, length)
#endif

// big files may be read directly from disk
#define BIG_AUDIO (2*(1<<20))	// 2MB

// maximum size for audio data
#define MAXIMUM_SIZE  0xFFFFFFFFU

// short sound for the "avt_bell" function
static avt_audio *alert_sound;
static void (*quit_audio_backend) (void);

// type for 16 bit samples (speed optimized)
typedef int_fast16_t sample16fast;

// pointer to the active mu-law or A-law decoding table
static const sample16fast *law_decoder;

// table for decoding mu-law
static const sample16fast mulaw_decode[256] = {
  -32124, -31100, -30076, -29052, -28028, -27004, -25980, -24956, -23932,
  -22908, -21884, -20860, -19836, -18812, -17788, -16764, -15996, -15484,
  -14972, -14460, -13948, -13436, -12924, -12412, -11900, -11388, -10876,
  -10364, -9852, -9340, -8828, -8316, -7932, -7676, -7420, -7164, -6908,
  -6652, -6396, -6140, -5884, -5628, -5372, -5116, -4860, -4604, -4348,
  -4092, -3900, -3772, -3644, -3516, -3388, -3260, -3132, -3004, -2876,
  -2748, -2620, -2492, -2364, -2236, -2108, -1980, -1884, -1820, -1756,
  -1692, -1628, -1564, -1500, -1436, -1372, -1308, -1244, -1180, -1116,
  -1052, -988, -924, -876, -844, -812, -780, -748, -716, -684, -652, -620,
  -588, -556, -524, -492, -460, -428, -396, -372, -356, -340, -324, -308,
  -292, -276, -260, -244, -228, -212, -196, -180, -164, -148, -132, -120,
  -112, -104, -96, -88, -80, -72, -64, -56, -48, -40, -32, -24, -16, -8, 0,
  32124, 31100, 30076, 29052, 28028, 27004, 25980, 24956, 23932, 22908,
  21884, 20860, 19836, 18812, 17788, 16764, 15996, 15484, 14972, 14460,
  13948, 13436, 12924, 12412, 11900, 11388, 10876, 10364, 9852, 9340, 8828,
  8316, 7932, 7676, 7420, 7164, 6908, 6652, 6396, 6140, 5884, 5628, 5372,
  5116, 4860, 4604, 4348, 4092, 3900, 3772, 3644, 3516, 3388, 3260, 3132,
  3004, 2876, 2748, 2620, 2492, 2364, 2236, 2108, 1980, 1884, 1820, 1756,
  1692, 1628, 1564, 1500, 1436, 1372, 1308, 1244, 1180, 1116, 1052, 988,
  924, 876, 844, 812, 780, 748, 716, 684, 652, 620, 588, 556, 524, 492, 460,
  428, 396, 372, 356, 340, 324, 308, 292, 276, 260, 244, 228, 212, 196, 180,
  164, 148, 132, 120, 112, 104, 96, 88, 80, 72, 64, 56, 48, 40, 32, 24, 16,
  8, 0
};

// table for decoding A-law
static const sample16fast alaw_decode[256] = {
  -5504, -5248, -6016, -5760, -4480, -4224, -4992, -4736, -7552, -7296, -8064,
  -7808, -6528, -6272, -7040, -6784, -2752, -2624, -3008, -2880, -2240,
  -2112, -2496, -2368, -3776, -3648, -4032, -3904, -3264, -3136, -3520,
  -3392, -22016, -20992, -24064, -23040, -17920, -16896, -19968, -18944,
  -30208, -29184, -32256, -31232, -26112, -25088, -28160, -27136, -11008,
  -10496, -12032, -11520, -8960, -8448, -9984, -9472, -15104, -14592,
  -16128, -15616, -13056, -12544, -14080, -13568, -344, -328, -376, -360,
  -280, -264, -312, -296, -472, -456, -504, -488, -408, -392, -440, -424,
  -88, -72, -120, -104, -24, -8, -56, -40, -216, -200, -248, -232, -152,
  -136, -184, -168, -1376, -1312, -1504, -1440, -1120, -1056, -1248, -1184,
  -1888, -1824, -2016, -1952, -1632, -1568, -1760, -1696, -688, -656, -752,
  -720, -560, -528, -624, -592, -944, -912, -1008, -976, -816, -784, -880,
  -848, 5504, 5248, 6016, 5760, 4480, 4224, 4992, 4736, 7552, 7296, 8064,
  7808, 6528, 6272, 7040, 6784, 2752, 2624, 3008, 2880, 2240, 2112, 2496,
  2368, 3776, 3648, 4032, 3904, 3264, 3136, 3520, 3392, 22016, 20992, 24064,
  23040, 17920, 16896, 19968, 18944, 30208, 29184, 32256, 31232, 26112,
  25088, 28160, 27136, 11008, 10496, 12032, 11520, 8960, 8448, 9984, 9472,
  15104, 14592, 16128, 15616, 13056, 12544, 14080, 13568, 344, 328, 376,
  360, 280, 264, 312, 296, 472, 456, 504, 488, 408, 392, 440, 424, 88, 72,
  120, 104, 24, 8, 56, 40, 216, 200, 248, 232, 152, 136, 184, 168, 1376,
  1312, 1504, 1440, 1120, 1056, 1248, 1184, 1888, 1824, 2016, 1952, 1632,
  1568, 1760, 1696, 688, 656, 752, 720, 560, 528, 624, 592, 944, 912, 1008,
  976, 816, 784, 880, 848
};

static void
audio_alert (void)
{
  // if alert_sound is loaded and nothing is currently playing
  if (alert_sound and not avt_audio_playing (NULL))
    avt_play_audio (alert_sound, AVT_PLAY);
}

extern void
avt_quit_audio (void)
{
  if (alert_sound)
    {
      avt_free_audio (alert_sound);
      alert_sound = NULL;
    }

  avt_bell_function (avt_flash);

  if (quit_audio_backend)
    {
      quit_audio_backend ();
      quit_audio_backend = NULL;
    }

  // no need to call it again automatically
  avt_quit_audio_function (NULL);
}

static void
method_done_memory (avt_audio * s)
{
  if (s->info.memory.sound)
    free (s->info.memory.sound);
}

static void
method_done_data (avt_audio * s)
{
  if (s->info.data.data)
    {
      avt_data_done (s->info.data.data);
      free (s->info.data.data);
    }
}

static void
method_done_mmap (avt_audio * s)
{
  if (s->info.mmap.map_length)
    avt_munmap (s->info.mmap.address, s->info.mmap.map_length);
}

// also used for mmap
static void
method_rewind_memory (avt_audio * s)
{
  s->info.memory.position = 0;
}

static void
method_rewind_data (avt_audio * s)
{
  if (s->info.data.data)
    avt_data_seek (s->info.data.data, s->info.data.start, SEEK_SET);
}

extern int
avt_add_raw_audio_data (avt_audio * snd, void *restrict data,
			size_t data_size)
{
  size_t old_size, new_size, out_size;
  bool active;

  if (_avt_STATUS != AVT_NORMAL or not snd or not data or not data_size
      or (snd->done != method_done_memory and snd->info.memory.sound))
    return avt_update ();

  // audio structure must have been created with avt_prepare_raw_audio
  if (snd->audio_type == AVT_AUDIO_UNKNOWN)
    {
      avt_set_error ("unknown audio format");
      return AVT_FAILURE;
    }

  out_size = data_size;
  old_size = snd->info.memory.length;
  new_size = old_size + out_size;

  // if it's currently playing, lock it
  active = avt_audio_playing (snd);
  if (active)
    avt_lock_audio ();

  // eventually get more memory for output buffer
  if (new_size > snd->info.memory.capacity)
    {
      void *new_sound;
      size_t new_capacity;

      // get twice the capacity
      new_capacity = 2 * snd->info.memory.capacity;

      /*
       * the capacity must never be lower than new_size
       * and it may still be 0
       */
      if (new_capacity < new_size)
	new_capacity = new_size;

      new_sound = realloc (snd->info.memory.sound, new_capacity);

      if (not new_sound)
	{
	  avt_set_error ("out of memory");
	  _avt_STATUS = AVT_ERROR;
	  return _avt_STATUS;
	}

      snd->info.memory.sound = (unsigned char *) new_sound;
      snd->info.memory.capacity = new_capacity;
    }

  memcpy (snd->info.memory.sound + old_size, data, out_size);
  snd->info.memory.length = new_size;
  snd->done = method_done_memory;

  if (active)
    avt_unlock_audio (snd);

  return avt_update ();
}

extern void
avt_finalize_raw_audio (avt_audio * snd)
{
  if (not snd->info.memory.sound or snd->done != method_done_memory)
    return;

  bool active = avt_audio_playing (snd);
  if (active)
    avt_lock_audio ();

  // eventually free unneeded memory
  if (snd->info.memory.capacity > snd->info.memory.length)
    {
      void *new_sound;
      size_t new_capacity;

      new_capacity = snd->info.memory.length;
      new_sound = realloc (snd->info.memory.sound, new_capacity);

      if (new_sound)
	{
	  snd->info.memory.sound = new_sound;
	  snd->info.memory.capacity = new_capacity;
	}
    }

  if (active)
    avt_unlock_audio (snd);
}

// also used for mmap
static size_t
method_get_audio_memory (avt_audio * restrict s, void *restrict data,
			 size_t size)
{
  if (s->info.memory.position + size > s->info.memory.length)
    size = s->info.memory.length - s->info.memory.position;

  memcpy (data, s->info.memory.sound + s->info.memory.position, size);
  s->info.memory.position += size;

  return size;
}


static size_t
method_get_audio_data (avt_audio * restrict s, void *restrict data,
		       size_t size)
{
  return avt_data_read (s->info.data.data, data, 1, size);
}


// get mu-law or A-law audio as 16bit from memory/mmap
static size_t
method_get_law_memory (avt_audio * restrict s, void *restrict data,
		       size_t size)
{
  size_t bytes = size / 2;

  if (s->info.memory.position + bytes > s->info.memory.length)
    bytes = s->info.memory.length - s->info.memory.position;

  uint_least8_t *restrict sound;
  int_least16_t *restrict d;

  sound = s->info.memory.sound + s->info.memory.position;
  d = data;

  size_t b = bytes;
  while (b--)
    *d++ = law_decoder[*sound++];

  s->info.memory.position += bytes;

  return bytes * 2;
}


// get mu-law or A-law audio as 16bit from data
static size_t
method_get_law_data (avt_audio * restrict s, void *restrict data, size_t size)
{
  size_t bytes = size / 2;

  uint_least8_t samples[bytes];
  size_t b;

  b = avt_data_read (s->info.data.data, &samples, sizeof (samples[0]), bytes);
  if (not b)
    return 0;

  uint_least8_t *restrict sample = samples;
  int_least16_t *restrict d = data;
  while (bytes--)
    *d++ = law_decoder[*sample++];

  return b * 2;
}


extern avt_audio *
avt_prepare_raw_audio (size_t capacity,
		       int samplingrate, int audio_type, int channels)
{
  struct avt_audio *s;

  // TODO
  if (channels < 1 or channels > 2)
    {
      avt_set_error ("only 1 or 2 channels supported");
      return NULL;
    }

  // adjustments for later optimizations
  if (AVT_LITTLE_ENDIAN == AVT_BYTE_ORDER)
    {
      switch (audio_type)
	{
	case AVT_AUDIO_S24SYS:
	  audio_type = AVT_AUDIO_S24LE;
	  break;

	case AVT_AUDIO_S32SYS:
	  audio_type = AVT_AUDIO_S32LE;
	  break;
	}
    }
  else if (AVT_BIG_ENDIAN == AVT_BYTE_ORDER)
    {
      switch (audio_type)
	{
	case AVT_AUDIO_S24SYS:
	  audio_type = AVT_AUDIO_S24BE;
	  break;

	case AVT_AUDIO_S32SYS:
	  audio_type = AVT_AUDIO_S32BE;
	  break;
	}
    }

  // get memory for struct
  s = calloc (1, sizeof (struct avt_audio));
  if (not s)
    {
      avt_set_error ("out of memory");
      return NULL;
    }

  s->audio_type = audio_type;
  s->samplingrate = samplingrate;
  s->channels = channels;
  s->rewind = method_rewind_memory;

  switch (audio_type)
    {
    case AVT_AUDIO_MULAW:
      law_decoder = mulaw_decode;
      s->get = method_get_law_memory;
      break;

    case AVT_AUDIO_ALAW:
      law_decoder = alaw_decode;
      s->get = method_get_law_memory;
      break;

    default:
      s->get = method_get_audio_memory;
    }

  // eventually reserve memory
  if (capacity > 0 and capacity < MAXIMUM_SIZE)
    {
      unsigned char *sound_data;
      sound_data = malloc (capacity);

      if (not sound_data)
	{
	  avt_set_error ("out of memory");
	  free (s);
	  return NULL;
	}

      s->done = method_done_memory;
      s->info.memory.sound = sound_data;
      s->info.memory.capacity = capacity;
    }

  return s;
}

extern void
avt_free_audio (avt_audio * snd)
{
  if (snd)
    {
      // Is this sound currently playing? Then stop it!
      if (avt_audio_playing (snd))
	avt_stop_audio ();

      // free the sound data
      if (snd->done)
	snd->done (snd);

      // free the rest
      free (snd);
    }
}

// if size is unknown use 0 or MAXIMUM_SIZE for maxsize
static avt_audio *
avt_load_audio_block (avt_data * src, size_t maxsize,
		      int samplingrate, int audio_type,
		      int channels, int playmode)
{
  avt_audio *audio;
  int n;
  uint_least32_t rest;
  uint_least8_t data[24 * 1024];

  audio = avt_prepare_raw_audio (maxsize, samplingrate, audio_type, channels);

  if (not audio)
    return NULL;

  if (maxsize)
    rest = maxsize;
  else
    rest = MAXIMUM_SIZE;

  while ((n =
	  avt_data_read (src, &data, 1, avt_min (sizeof (data), rest))) > 0)
    {
      if (avt_add_raw_audio_data (audio, data, n) != AVT_NORMAL)
	{
	  avt_free_audio (audio);
	  return NULL;
	}

      if (playmode != AVT_LOAD)
	{
	  avt_play_audio (audio, playmode);
	  playmode = AVT_LOAD;
	}

      rest -= n;
    }

  avt_finalize_raw_audio (audio);

  return audio;
}


#if _POSIX_MAPPED_FILES+0 > 0

static avt_audio *
avt_mmap_audio (avt_data * src, size_t maxsize,
		int samplingrate, int audio_type, int channels, int playmode)
{
  int fd = avt_data_filenumber (src);

  // not a file?
  if (fd < 0)
    return NULL;

  long pos = avt_data_tell (src);
  if (pos < 0)
    return NULL;

  long length;
  if (maxsize and maxsize < MAXIMUM_SIZE)
    length = maxsize + pos;
  else				// get length of file
    {
      if (not avt_data_seek (src, 0, SEEK_END))
	return NULL;		// stream not seekable

      length = avt_data_tell (src);
      maxsize = length - pos;
      avt_data_seek (src, pos, SEEK_SET);
    }

  void *address = mmap (NULL, length, PROT_READ, MAP_PRIVATE, fd, 0);
  if (MAP_FAILED == address)
    return NULL;

  // advise that access will be sequential
#if defined(POSIX_MADV_SEQUENTIAL)
  posix_madvise (address, length, POSIX_MADV_SEQUENTIAL);
#elif defined(MADV_SEQUENTIAL)
  // BSD
  madvise (address, length, MADV_SEQUENTIAL);
#endif

  avt_audio *audio;
  audio = avt_prepare_raw_audio (0, samplingrate, audio_type, channels);
  if (not audio)
    {
      avt_munmap (address, length);
      return NULL;
    }

  audio->info.mmap.address = address;
  audio->info.mmap.map_length = length;
  audio->done = method_done_mmap;
  audio->info.mmap.sound = ((unsigned char *) address) + pos;
  audio->info.mmap.length = maxsize;

  if (playmode != AVT_LOAD)
    avt_play_audio (audio, playmode);

  /*
   * note: It is okay to close the original file
   * after the mapping is established
   */

  return audio;
}

#else // no mapped files
#warning "No memory mapping for audio files available"
#define avt_mmap_audio(src,maxsize,rate,type,channels,mode)  (NULL)
#endif

static avt_audio *
avt_fetch_audio_data (avt_data * src, int samplingrate,
		      int audio_type, int channels, int playmode)
{
  int start = avt_data_tell (src);

  // on non seekable streams rewind would be impossible
  if (start < 0)
    return NULL;

  // make a copy of the data source
  avt_data *data = avt_data_dup (src);
  if (not data)
    return NULL;

  avt_audio *audio;
  audio = avt_prepare_raw_audio (0, samplingrate, audio_type, channels);
  if (not audio)
    {
      free (data);
      return NULL;
    }

  audio->info.data.data = data;
  audio->info.data.start = start;
  audio->rewind = method_rewind_data;
  audio->done = method_done_data;

  switch (audio_type)
    {
    case AVT_AUDIO_MULAW:
      law_decoder = mulaw_decode;
      audio->get = method_get_law_data;
      break;

    case AVT_AUDIO_ALAW:
      law_decoder = alaw_decode;
      audio->get = method_get_law_data;
      break;

    default:
      audio->get = method_get_audio_data;
    }

  if (playmode != AVT_LOAD)
    avt_play_audio (audio, playmode);

  return audio;
}

// find a suitable audio loader
static avt_audio *
avt_audio_loader (avt_data * src, size_t audio_size, int samplingrate,
		  int audio_type, int channels, int playmode)
{
  avt_audio *audio = NULL;

  if (avt_data_filenumber (src) < 0)	// already in memory?
    audio = avt_fetch_audio_data (src, samplingrate, audio_type,
				  channels, playmode);
  else if (BIG_AUDIO <= audio_size)
    {
      // try to get large audio data while playing
      audio = avt_mmap_audio (src, audio_size, samplingrate, audio_type,
			      channels, playmode);

      if (not audio)
	audio = avt_fetch_audio_data (src, samplingrate, audio_type,
				      channels, playmode);
    }

  // if nothing else worked, read it into memory
  if (not audio)
    audio = avt_load_audio_block (src, audio_size, samplingrate, audio_type,
				  channels, playmode);

  return audio;
}

// keep it working on non-seekable streams
static avt_audio *
avt_load_au (avt_data * src, size_t maxsize, int playmode)
{
  if (not src)
    return NULL;

  avt_data_big_endian (src, true);

  uint_least32_t head_size, audio_size, encoding, samplingrate, channels;
  // magic value is already read
  head_size = avt_data_read32 (src);
  audio_size = avt_data_read32 (src);
  encoding = avt_data_read32 (src);
  samplingrate = avt_data_read32 (src);
  channels = avt_data_read32 (src);

  // skip the rest of the header
  if (head_size > 24)
    avt_data_skip (src, head_size - 24);

  if (maxsize < MAXIMUM_SIZE)
    {
      maxsize -= head_size;

      if (maxsize < audio_size)
	audio_size = maxsize;
    }

  // Note: linear PCM is always assumed to be signed and big endian
  int audio_type;
  switch (encoding)
    {
    case 1:			// mu-law
      audio_type = AVT_AUDIO_MULAW;
      break;

    case 2:			// 8Bit linear PCM
      audio_type = AVT_AUDIO_S8;	// signed!
      break;

    case 3:			// 16Bit linear PCM
      audio_type = AVT_AUDIO_S16BE;
      break;

    case 4:			// 24Bit linear PCM
      audio_type = AVT_AUDIO_S24BE;
      break;

    case 5:			// 32Bit linear PCM
      audio_type = AVT_AUDIO_S32BE;
      break;

    case 27:			// A-law
      audio_type = AVT_AUDIO_ALAW;
      break;

    default:
      avt_set_error ("unsupported encoding in AU file");
      return NULL;
    }

  /*
   * other encodings:
   *
   * 6: 32Bit float
   * 7: 64Bit float
   * 10-13: 8/16/24/32Bit fixed point
   * 23-26: ADPCM variants
   */

  return avt_audio_loader (src, audio_size, samplingrate, audio_type,
			   channels, playmode);
}


// round up to next even value
#define even(x)  ((x) + ((x) & 1))

// The AU format is much simpler and cleaner than WAV
// keep it working on non-seekable streams
static avt_audio *
avt_load_wave (avt_data * src, size_t maxsize, int playmode)
{
  int audio_type;
  char identifier[4];
  bool wrong_chunk;
  uint_least32_t chunk_size;
  uint_least32_t samplingrate;
  uint_least16_t encoding, channels, bits_per_sample;

  if (not src)
    return NULL;

  avt_data_big_endian (src, false);

  /*
   * magic value RIFF is already read
   * this chunk contains the rest,
   * so chunk_size should be the file size - 8
   */
  chunk_size = avt_data_read32 (src);

  if (avt_data_read (src, &identifier, sizeof (identifier), 1) != 1
      or memcmp ("WAVE", identifier, sizeof (identifier)) != 0)
    return NULL;		// not a Wave file

  // search format chunk
  do
    {
      if (avt_data_read (src, &identifier, sizeof (identifier), 1) != 1)
	return NULL;		// no format chunk found
      chunk_size = avt_data_read32 (src);
      wrong_chunk = (memcmp ("fmt ", identifier, sizeof (identifier)) != 0);
      if (wrong_chunk)
	avt_data_skip (src, even (chunk_size));
    }
  while (wrong_chunk);

  encoding = avt_data_read16 (src);
  channels = avt_data_read16 (src);
  samplingrate = avt_data_read32 (src);
  avt_data_read32 (src);	// bytes_per_second
  avt_data_read16 (src);	// block_align
  bits_per_sample = avt_data_read16 (src);	// just for PCM

  if (chunk_size > 16)
    avt_data_skip (src, even (chunk_size) - 16);

  switch (encoding)
    {
    case 1:			// PCM
      // smaller numbers are already right-padded
      if (bits_per_sample <= 8)
	audio_type = AVT_AUDIO_U8;	// unsigned!
      else if (bits_per_sample <= 16)
	audio_type = AVT_AUDIO_S16LE;	// signed
      else if (bits_per_sample <= 24)
	audio_type = AVT_AUDIO_S24LE;	// signed
      else if (bits_per_sample <= 32)
	audio_type = AVT_AUDIO_S32LE;	// signed
      else
	return NULL;
      break;

    case 6:			// A-law
      audio_type = AVT_AUDIO_ALAW;
      break;

    case 7:			// mu-law
      audio_type = AVT_AUDIO_MULAW;
      break;

    default:			// unsupported encoding
      return NULL;
    }

  // search data chunk - must be after format chunk
  do
    {
      if (avt_data_read (src, &identifier, sizeof (identifier), 1) != 1)
	return NULL;		// no data chunk found
      chunk_size = avt_data_read32 (src);
      wrong_chunk = (memcmp ("data", identifier, sizeof (identifier)) != 0);
      if (wrong_chunk)
	avt_data_skip (src, even (chunk_size));
    }
  while (wrong_chunk);

  if (chunk_size < maxsize)
    maxsize = chunk_size;

  return avt_audio_loader (src, maxsize, samplingrate, audio_type, channels,
			   playmode);
}

// keep it working on non-seekable streams
static avt_audio *
avt_load_audio_general (avt_data * src, size_t maxsize, int playmode)
{
  if (not src)
    return NULL;

  if (not maxsize)
    maxsize = MAXIMUM_SIZE;

  char head[4];
  if (avt_data_read (src, head, sizeof (head), 1) != 1)
    {
      avt_set_error ("cannot read head of audio data");
      return NULL;
    }

  struct avt_audio *s;
  if (memcmp (head, ".snd", 4) == 0)
    s = avt_load_au (src, maxsize, playmode);
  else if (memcmp (head, "RIFF", 4) == 0)
    s = avt_load_wave (src, maxsize, playmode);
  else
    {
      s = NULL;
      avt_set_error ("audio data neither in AU nor WAVE format");
    }

  return s;
}

extern int
avt_start_audio_common (void (*quit_backend) (void))
{
  if (not alert_sound)
    alert_sound = avt_load_audio_data (&avt_alert_data,
				       avt_alert_data_size, AVT_LOAD);

  if (alert_sound)
    avt_bell_function (audio_alert);

  quit_audio_backend = quit_backend;
  avt_quit_audio_function (avt_quit_audio);

  return _avt_STATUS;
}

static inline bool
no_data_needed (avt_audio * s)
{
  return (s->done != method_done_data);
}

extern avt_audio *
avt_load_audio_file (const char *file, int playmode)
{
  avt_audio *r;
  avt_data d;

  r = NULL;

  avt_data_init (&d);
  if (avt_data_open_file (&d, file))
    r = avt_load_audio_general (&d, MAXIMUM_SIZE, playmode);

  if (not r or no_data_needed (r))
    d.done (&d);

  return r;
}

extern avt_audio *
avt_load_audio_part (avt_stream * stream, size_t maxsize, int playmode)
{
  avt_audio *r;
  avt_data d;

  r = NULL;

  avt_data_init (&d);
  if (avt_data_open_stream (&d, (FILE *) stream, false))
    r = avt_load_audio_general (&d, maxsize, playmode);

  if (not r or no_data_needed (r))
    d.done (&d);

  return r;
}

extern avt_audio *
avt_load_audio_stream (avt_stream * stream, int playmode)
{
  avt_audio *r;
  avt_data d;

  r = NULL;

  avt_data_init (&d);
  if (avt_data_open_stream (&d, (FILE *) stream, false))
    r = avt_load_audio_general (&d, MAXIMUM_SIZE, playmode);

  if (not r or no_data_needed (r))
    d.done (&d);

  return r;
}

extern avt_audio *
avt_load_audio_data (const void *data, size_t datasize, int playmode)
{
  avt_audio *r;
  avt_data d;

  r = NULL;

  avt_data_init (&d);
  if (avt_data_open_memory (&d, data, datasize))
    r = avt_load_audio_general (&d, datasize, playmode);

  if (not r or no_data_needed (r))
    d.done (&d);

  return r;
}

#else // NO_AUDIO

extern avt_audio *
avt_prepare_raw_audio (size_t capacity,
		       int samplingrate, int audio_type, int channels)
{
  (void) capacity;
  (void) samplingrate;
  (void) audio_type;
  (void) channels;

  return NULL;
}

extern int
avt_add_raw_audio_data (avt_audio * snd, void *data, size_t data_size)
{
  (void) snd;
  (void) data;
  (void) data_size;

  return AVT_FAILURE;
}

extern void
avt_finalize_raw_audio (avt_audio * snd)
{
  (void) snd;
}

extern void
avt_free_audio (avt_audio * snd)
{
  (void) snd;
}

extern void
avt_quit_audio (void)
{
}

extern avt_audio *
avt_load_audio_file (const char *file, int playmode)
{
  (void) file;
  (void) playmode;

  return NULL;
}

extern avt_audio *
avt_load_audio_part (avt_stream * stream, size_t maxsize, int playmode)
{
  (void) playmode;
  (void) maxsize;
  (void) stream;

  return NULL;
}

extern avt_audio *
avt_load_audio_stream (avt_stream * stream, int playmode)
{
  (void) playmode;
  (void) stream;

  return NULL;
}

extern avt_audio *
avt_load_audio_data (const void *data, size_t datasize, int playmode)
{
  (void) data;
  (void) datasize;
  (void) playmode;

  return NULL;
}


#endif // NO_AUDIO
