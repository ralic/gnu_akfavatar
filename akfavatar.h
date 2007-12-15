/*
 * AKFAvatar library - for giving your programs a graphical Avatar
 * Copyright (c) 2007 Andreas K. Foerster <info@akfoerster.de>
 *
 * needed: 
 *  SDL1.2 (recommended: SDL1.2.11)
 * recommended: 
 *  SDL_image1.2
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

/* $Id: akfavatar.h,v 2.25 2007-12-15 09:36:15 akf Exp $ */

#ifndef _akfavatar_h
#define _akfavatar_h

/* SDL redefines main on some systems */
#if defined(__WIN32__) ||  defined(__MACOS__) || defined(__MACOSX__)
#  include "SDL.h"
#endif

/* to get the systems definition of wchar_t */
#include <stddef.h>

/* maximum linelength */
#define LINELENGTH 80

/* for avt_initialize */
#define AUTOMODE -1
#define WINDOW 0
#define FULLSCREENNOSWITCH 2

#ifdef NOSWITCH
#  define FULLSCREEN FULLSCREENNOSWITCH
#else
#  define FULLSCREEN 1
#endif


/* for _avt_STATUS */
#define AVATARNORMAL 0
#define AVATARQUIT 1
#define AVATARERROR -1

/* for avt_set_delays */
#define DEFAULT_TEXT_DELAY 75
#define DEFAULT_FLIP_PAGE_DELAY 2700

/* for avt_text_direction */
#define LEFT_TO_RIGHT 0
#define RIGHT_TO_LEFT 1

/* some unicode characters, you might want to use */
#define DOLLAR L"\x0024"
#define EURO L"\x20AC"
#define CENT L"\x00A2"
#define POUND L"\x00A3"
#define LIRA L"\x20A4"
#define YEN L"\x00A5"

#define SMILEY L"\x263A"
#define FROWNEY L"\x2639"

/* 
 * put string in quotation marks
 */
#define quote(x) L"“" x L"”"

/*
 * put string in german quotation marks
 */
#define Zitat(x) L"„" x L"“"

/* 
 * example: avt_wait(seconds(2.5)) waits 2.5 seconds 
 */
#define seconds(x) (x*1000)

#ifdef __cplusplus
/* *INDENT-OFF* */
extern "C" {
/* *INDENT-ON* */
#endif /* __cplusplus */

/* 
 * general type for avatar images
 * may change in the future!
 */
typedef void avt_image_t;

/* 
 * general type for audio data
 * may change in the future!
 */
typedef void avt_audio_t;

/*
 * type for keyhandler
 * see avt_register_keyhandler
 */
typedef void (*avt_keyhandler) (int sym, int mod, int unicode);

/* base fnctions */

/* which version */
extern const char *avt_version (void);

/* copyright information */
extern const char *avt_copyright (void);

/* license information */
extern const char *avt_license (void);

/* 0 = normal; 1 = quit-request; -1 = error */
extern int avt_get_status (void);

/* set status */
extern void avt_set_status (int status);

/* get error message */
extern char *avt_get_error (void);

/* 
 * set text direction
 * the cursor is moved to start of the line
 * in a text, you might want to call avt_newline after that
 */
extern void avt_text_direction (int direction);

/* stop, when Esc is pressed? (default: yes) */
extern void avt_stop_on_esc (int stop);

/*
 * get the default avatar image
 * use this with avt_initialize
 */
extern avt_image_t *avt_default (void);

/*
 * RGB gimp_image
 * for importing an avatar
 * also uses avt_make_transparent
 */
extern avt_image_t *avt_import_gimp_image (void *gimp_image);

/* 
 * import avatar from image data
 */
extern avt_image_t *avt_import_image_data (void *img, int imgsize);

/* 
 * import avatar from file
 */
extern avt_image_t *avt_import_image_file (const char *file);

/* 
 * free avt_image_t images
 * allocated with avt_import_gimp_image or avt_import_imagefile
 * (automatically called in avt_initialize)
 */
extern void avt_free_image (avt_image_t * image);

/*
 * make background transparent
 * pixel in the upper left corner is supposed to be the background color
 */
extern avt_image_t *avt_make_transparent (avt_image_t * image);

/*
 * define the backgroud color
 * values in the range 0x00 .. 0xFF
 * must be called before avt_initialize 
 */
extern void avt_set_background_color (int red, int green, int blue);

/*
 * change the text color
 * values in the range 0x00 .. 0xFF
 */
extern void avt_set_text_color (int red, int green, int blue);
extern void avt_set_text_background_color (int red, int green, int blue);

/*
 * delay time for text-writing
 * default: DEFAULT_TEXT_DELAY
 */
extern void avt_set_text_delay (int delay);

/*
 * delay time for page flipping
 * default: DEFAULT_FLIP_PAGE_DELAY
 */
extern void avt_set_flip_page_delay (int delay);

/* don't use this anymore, it is about to be removed */
extern void avt_set_delays (int text, int flip_page);

/* register an external keyhandler */
extern void avt_register_keyhandler (avt_keyhandler handler);

/* 
 * initialize the avatar system
 * mode is either WINDOW or FULLSCREEN or FULLSCREENNOSWITCH
 * the original image is freed in this function!
 * the image may be NULL if no avatar should be shown
 */
extern int avt_initialize (const char *title,
			   const char *icontitle,
			   avt_image_t * image, int mode);

/* 
 * quit the avatar system
 */
extern void avt_quit (void);

/* 
 * prints a L'\0' terminated string in the balloon
 * if there is no balloon, it is drawn
 * if there is no avatar, it is shown (not moved in)
 * interprets control chars
 */
extern int avt_say (const wchar_t * txt);

/* 
 * writes string with given length in the balloon
 * the string needn't be terminated and can contain binary zeros
 * if there is no balloon, it is drawn
 * if there is no avatar, it is shown (not moved in)
 * interprets control characters
 */
extern int avt_say_len (const wchar_t * txt, const int len);

/* set encoding for mb functions */
extern int avt_mb_encoding (const char *encoding);

/* 
 * decode a string into wchar_t
 * size in bytes
 * returns number of characters in dest (without the termination zero)
 * dest must be freed by caller with avt_free
 */
extern int avt_mb_decode (wchar_t ** dest, const char *src, const int size);

/* free memory allocated by this library */
extern void avt_free (void *ptr);

/*
 * like avt_say,
 * but converts from a given charset encoding
 * (see avt_mb_encoding)
 * the text is a 0 terminated C-String
 */
extern int avt_say_mb (const char *txt);

/*
 * the same with a given length
 * the string needn't be terminated then 
 * and can contain binary zeros
 */
extern int avt_say_mb_len (const char *txt, int len);

/*
 * get a character from the keyboard
 * only for printable characters, not for function keys
 * (ch is just one character, not a string)
 */
extern int avt_get_key (wchar_t * ch);

/*
 * get string (just one line)
 * the maximum length is LINELENGTH-1
 * size is the size of s in bytes (not the length)
 *
 * (I don't use size_t for better compatiblity with other languages)
 */
extern int avt_ask (wchar_t * s, const int size);

/*
 * like avt_ask,
 * but converts to a given encoding
 *
 * for UTF-8 encoding it should have a capacity of 
 * 4 * LINELENGTH Bytes
 */
extern int avt_ask_mb (char *s, const int size);

/*
 * new line
 * same as \n in avt_say
 */
extern int avt_new_line (void);

/*
 * wait a while and then clear the textfield
 * same as \f in avt_say
 */
extern int avt_flip_page (void);

/* update, ie handle events and give some time to other processes */
/* use this in a longer loop in your program */
extern int avt_update (void);

/* wait a while */
extern int avt_wait (int milliseconds);

/* wait for a keypress */
extern int avt_wait_key (const wchar_t * message);

/*
 * like avt_waitkey,
 * but converts from a given charset encoding
 * (see avt_mb_encoding)
 */
extern int avt_wait_key_mb (char *message);


/* functions for extended use */

/* 
 * set a viewport (sub-area of the textarea)
 * upper left corner is 1, 1
 */
extern void avt_viewport (int x, int y, int width, int height);

/* show an empty screen with the background color */
extern void avt_clear_screen (void);

/* show just the avatar without the balloon */
extern void avt_show_avatar (void);

/* 
 * load image and show it
 * if SDL_image isn't available then uncompressed BMP is still supported
 * if it succeeds call avt_wait or avt_waitkey 
 */
extern int avt_show_image_file (const char *file);

/*
 * show image from image data
 * if SDL_image isn't available then uncompressed BMP is still supported
 * after that call avt_wait or avt_waitkey
 */
extern int avt_show_image_data (void *img, int imgsize);

/*
 * show gimp image
 */
extern int avt_show_gimp_image (void *gimp_image);

/*
 * like avt_show_avatar, but the avatar is moved in
 */
extern int avt_move_in (void);

/*
 * move the avatar out => empty screen
 */
extern int avt_move_out (void);

/*
 * make a short sound, when audio is initialized
 * else it is the same as avt_flash
 * same as with \a in avt_say
 * the sound is actually not a bell ;-)
 */
extern void avt_bell (void);

/*
 * visual flash of the screen
 */
extern void avt_flash (void);

/* 
 * clears the viewport
 * if there is no balloon yet, it is drawn
 */
extern void avt_clear (void);

/* 
 * clear end of line
 * depending on text direction
 */
extern void avt_clear_eol (void);

/* forward one character position
 * ie. print a space
 */
extern int avt_forward (void);

/*
 * delete last caracter
 */
extern void avt_backspace (void);

/*
 * set scroll mode
 * 0 = off (page-flipping), 1 = normal
 * (further modes are planned, it is not meant as a boolean)
 */
extern void avt_set_scroll_mode (int mode);
extern int avt_get_scroll_mode (void);

/* 
 * handle coordinates
 *
 * the coordinates start with 1, 1 
 * in the upper left corner
 * and are independent from the text direction
 */

/*
 * get position in the viewport
 */
extern int avt_where_x (void);
extern int avt_where_y (void);

/* maximum positions (whole text-field) */
extern int avt_get_max_x (void);
extern int avt_get_max_y (void);

/*
 * put cusor to specified coordinates
 */
extern void avt_move_x (int x);
extern void avt_move_y (int y);

/* 
 * delete num lines, starting from line
 * the rest ist scrolled up
 */
extern void avt_delete_lines (int line, int num);

/* 
 * insert num lines, starting at line
 * the rest ist scrolled down
 */
extern void avt_insert_lines (int line, int num);

/***********************************************************************/
/* audio stuff */

/* must be called AFTER avt_initialize! */
extern int avt_initialize_audio (void);

/* should be called BEFORE avt_quit */
extern void avt_quit_audio (void);

/* 
 * loads a wave file 
 * supported: PCM, MS-ADPCM, IMA-ADPCM
 */
extern avt_audio_t *avt_load_wave_file (const char *file);

/*
 * loads wave data from memory
 * must still be freed with avt_free_audio!
 */
extern avt_audio_t *avt_load_wave_data (void *data, int datasize);

/*
 * frees memory of a loaded sound
 */
extern void avt_free_audio (avt_audio_t * snd);

/*
 * plays a sound
 */
extern int avt_play_audio (avt_audio_t * snd, int doloop);

/*
 * wait until the sound ends
 * this stops a loop, but still plays to the end of the sound
 */
extern int avt_wait_audio_end (void);

/* stops audio */
extern void avt_stop_audio (void);


#ifdef __cplusplus
/* *INDENT-OFF* */
}
#endif /* __cplusplus */
#endif /* _akfavatar_h */
