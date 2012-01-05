/*
 * AKFAvatar library - for giving your programs a graphical Avatar
 * Copyright (c) 2007,2008,2009,2010,2011,2012 Andreas K. Foerster <info@akfoerster.de>
 *
 * needed:
 *  SDL1.2 (recommended: SDL1.2.11 or later (but not 1.3!))
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

#ifndef _akfavatar_h
#define _akfavatar_h

/* SDL redefines main on some systems */
#if defined(_WIN32) || defined(__APPLE__) ||  defined(macintosh)
#  include "SDL.h"
#endif

/* to get the systems definition of wchar_t */
#include <stddef.h>

#ifndef __cplusplus
#include <stdbool.h>
#endif

#define AKFAVATAR 1

/* maximum linelength */
#define AVT_LINELENGTH 80

/* for avt_initialize */
#define AVT_AUTOMODE -1
#define AVT_WINDOW 0
#define AVT_FULLSCREENNOSWITCH 2

#ifdef AVT_NOSWITCH
#  define AVT_FULLSCREEN AVT_FULLSCREENNOSWITCH
#else
#  define AVT_FULLSCREEN 1
#endif

/* for _avt_STATUS */
#define AVT_NORMAL 0
#define AVT_QUIT 1
#define AVT_ERROR -1

#define AVT_MAKE_BOOL(x) ((x) != 0)

/* for avt_set_text_delay and avt_set_flip_page_delay */
#define AVT_DEFAULT_TEXT_DELAY 75
#define AVT_DEFAULT_FLIP_PAGE_DELAY 2700

/* for avt_text_direction */
#define AVT_LEFT_TO_RIGHT 0
#define AVT_RIGHT_TO_LEFT 1

/* for avt_get_key(&k) */
/* note: F11 is normally used to toggle fullscreen mode! */
/* note: most keyboards don't have all those keys */
#define AVT_KEY_ENTER     0x000D
#define AVT_KEY_BACKSPACE 0x0008
#define AVT_KEY_DELETE    0x007F
#define AVT_KEY_UP        0xF000
#define AVT_KEY_DOWN      0xF001
#define AVT_KEY_RIGHT     0xF002
#define AVT_KEY_LEFT      0xF003
#define AVT_KEY_INSERT    0xF004
#define AVT_KEY_HOME      0xF005
#define AVT_KEY_END       0xF006
#define AVT_KEY_PAGEUP    0xF007
#define AVT_KEY_PAGEDOWN  0xF008
#define AVT_KEY_HELP      0xF009
#define AVT_KEY_MENU      0xF00A
#define AVT_KEY_F1        0xF0F1
#define AVT_KEY_F2        0xF0F2
#define AVT_KEY_F3        0xF0F3
#define AVT_KEY_F4        0xF0F4
#define AVT_KEY_F5        0xF0F5
#define AVT_KEY_F6        0xF0F6
#define AVT_KEY_F7        0xF0F7
#define AVT_KEY_F8        0xF0F8
#define AVT_KEY_F9        0xF0F9
#define AVT_KEY_F10       0xF0FA
#define AVT_KEY_F11       0xF0FB  /* only if single keys are reserved */
#define AVT_KEY_F12       0xF0FC
#define AVT_KEY_F13       0xF0FD
#define AVT_KEY_F14       0xF0FE
#define AVT_KEY_F15       0xF0FF

/*
 * example: avt_wait(AVT_SECONDS(2.5)) waits 2.5 seconds 
 */
#define AVT_SECONDS(x) ((x)*1000)

/* macro for marking unused symbols */
#ifdef __GNUC__
#  define AVT_UNUSED __attribute__ ((__unused__))
#else
#  define AVT_UNUSED
#endif /* __GNUC__ */

#ifdef __cplusplus
#  define AVT_BEGIN_DECLS  extern "C" {
#  define AVT_END_DECLS    }
#else
#  define AVT_BEGIN_DECLS
#  define AVT_END_DECLS
#endif /* __cplusplus */

/* for later use */
#define AVT_API  extern

/***********************************************************************/
/* type definitions */

/*
 * type for single characters
 * assuming at least 32-bit systems
 * on Windows wchar_t/wint_t is not large enough for Unicode
 */
typedef int avt_char;

/*
 * general types for avatar images and audio data
 */
typedef struct SDL_Surface avt_image_t;
typedef struct avt_audio_t avt_audio_t;

/* for streams (use FILE from your programs) */
typedef void avt_stream;

AVT_BEGIN_DECLS


/***********************************************************************/
/* functions */
/* most functios return the status, unless otherwise stated */
/***********************************************************************/

/***********************************************************************/
/* initialization / finalization */

/*
 * initialize the avatar system
 *
 * mode is either AVT_WINDOW or AVT_FULLSCREEN or AVT_FULLSCREENNOSWITCH.
 * The original image is freed in this function!
 * So you can directly put calls to avt_default
 * or the avt_import_* functions here.
 * the image may be NULL if no avatar should be shown
 * title and/or shortname may also be NULL
 * avt_mb_encoding() may influence title/shortname
 */
AVT_API int avt_initialize (const char *title,
			    const char *shortname,
			    avt_image_t *image,
			    int mode);

/*
 * quit the avatar system
 * can be used with atexit
 */
AVT_API void avt_quit (void);

/*
 * call avt_wait_button (); avt_move_out (); avt_quit ();
 * can be used with atexit
 */
AVT_API void avt_button_quit (void);

/***********************************************************************/
/* getting an avatarimage */

/*
 * these functions can be used directly with
 * avt_initialize or avt_change_avatar_image
 * they call avt_make_transparent if approriete
 */

/*
 * X-Pixmaps (XPM), X Bitmaps (XBM) and uncompressed BMP are always supported
 * other image formats are supported with SDL_image
 */

/* get the default avatar image */
AVT_API avt_image_t *avt_default (void);

/* import an avatar from XPM data */
AVT_API avt_image_t *avt_import_xpm (char **xpm);

/* import an avatar from XBM data */
AVT_API avt_image_t *avt_import_xbm (const unsigned char *bits,
				     int width, int height,
				     const char *colorname);

/* RGB gimp_image */
AVT_API avt_image_t *avt_import_gimp_image (void *gimp_image);

/* import avatar from image data */
AVT_API avt_image_t *avt_import_image_data (void *img, int imgsize);

/* import avatar from file */
AVT_API avt_image_t *avt_import_image_file (const char *filename);

/* import avatar from stream */
AVT_API avt_image_t *avt_import_image_stream (avt_stream *stream);

/***********************************************************************/
/* other functions for avatarimages */

/*
 * change avatar image while running
 * if the avatar is visible, the screen gets cleared
 * the original image is freed in this function!
 * the image may be NULL if no avatar should be shown
 * on error AVT_ERROR is set and returned
 * an avatar name is cleared
 */
AVT_API int avt_change_avatar_image (avt_image_t *image);

/*
 * free avt_image_t images
 * (automatically called in avt_initialize and avt_change_avatar_image)
 */
AVT_API void avt_free_image (avt_image_t *image);

/*
 * make background transparent
 * pixel in the upper left corner is supposed to be the background color
 */
AVT_API avt_image_t *avt_make_transparent (avt_image_t *image);

/***********************************************************************/
/* actions without or outside the balloon */
/* see also "showing images without the avatar" */

/* show an empty screen with the background color */
AVT_API void avt_clear_screen (void);

/* show just the avatar without the balloon */
AVT_API void avt_show_avatar (void);

/* like avt_show_avatar, but the avatar is moved in */
AVT_API int avt_move_in (void);

/* move the avatar out => empty screen */
AVT_API int avt_move_out (void);

/*
 * make a short sound, when audio is initialized
 * else it is the same as avt_flash
 * same as with \a in avt_say
 * the sound is actually not a bell ;-)
 */
AVT_API void avt_bell (void);

/* visual flash of the screen */
AVT_API void avt_flash (void);

/* update, ie handle events and give some time to other processes */
/* use this in a longer loop in your program */
AVT_API int avt_update (void);

/*
 * wait a while
 * this is for longer times - a second or more
 * it reacts on events and evetually redraws the screen if needed
 */
AVT_API int avt_wait (int milliseconds);

/*
 * delay
 * this is for short delays - fractions of a second
 * it doesn't react on events
 */
AVT_API void avt_delay (int milliseconds);

/* counter, which is increased every millisecond */
AVT_API unsigned int avt_ticks (void);

/***********************************************************************/
/* say or ask stuff with wchar_t (Unicode) */

/*
 * prints a L'\0' terminated string in the balloon
 * if there is no balloon, it is drawn
 * if there is no avatar, it is shown (not moved in)
 * interprets control chars including overstrike-text
 */
AVT_API int avt_say (const wchar_t *txt);

/*
 * writes string with given length in the balloon
 * the string needn't be terminated and can contain binary zeros
 * if there is no balloon, it is drawn
 * if there is no avatar, it is shown (not moved in)
 * interprets control characters including overstrike-text
 */
AVT_API int avt_say_len (const wchar_t *txt, int len);

/*
 * sets the balloon size so that the text fits exactly
 * prints a L'\0' terminated string in the balloon
 * if there is no balloon, it is drawn
 * if there is no avatar, it is shown (not moved in)
 * interprets control characters including overstrike-text
 */
AVT_API int avt_tell (const wchar_t *txt);

/*
 * sets the balloon size so that the text fits exactly
 * writes string with given length in the balloon
 * the string needn't be terminated and can contain binary zeros
 * if len is <= 0 then it's the same as avt_tell()
 * if there is no balloon, it is drawn
 * if there is no avatar, it is shown (not moved in)
 * interprets control characters including overstrike-text
 */
AVT_API int avt_tell_len (const wchar_t *txt, int len);

/*
 * writes a single character in the balloon
 * if there is no balloon, it is drawn
 * if there is no avatar, it is shown (not moved in)
 * interprets control characters, but not for overstrike-text
 */
AVT_API int avt_put_char (avt_char ch);

/*
 * checks whether the given charactrer is printable
 * returns false on unknown or control characters
 */
AVT_API bool avt_is_printable (avt_char ch);

/*
 * get string (just one line)
 * the maximum length is LINELENGTH-1
 * size is the size of s in bytes (not the length)
 *
 * (I don't use size_t for better compatiblity with other languages)
 */
AVT_API int avt_ask (wchar_t *s, int size);

/*
 * get a character from the keyboard
 * see AVT_KEY constants for function keys
 * (ch is a pointer to one character, not a string)
 */
AVT_API int avt_key (avt_char *ch);

/***********************************************************************/
/* say or ask stuff with multy-byte encodings */

/* set encoding for mb functions */
/* NULL or "" sets the systems default encoding (use with care!) */
AVT_API int avt_mb_encoding (const char *encoding);

/* get the encoding, NULL if none set */
/* you should make a copy of the result immediately */
AVT_API char* avt_get_mb_encoding (void);

/*
 * prints a 0 terminated string in the balloon
 * if there is no balloon, it is drawn
 * if there is no avatar, it is shown (not moved in)
 * interprets control chars including overstrike-text
 *
 * converts from a given charset encoding
 * (see avt_mb_encoding)
 */
AVT_API int avt_say_mb (const char *txt);

/*
 * the same with a given length
 * the string needn't be terminated then
 * and can contain binary zeros
 */
AVT_API int avt_say_mb_len (const char *txt, int len);

/*
 * sets the balloon size so that the text fits exactly
 * prints a 0 terminated string in the balloon
 * if there is no balloon, it is drawn
 * if there is no avatar, it is shown (not moved in)
 * interprets control chars including overstrike-text
 *
 * converts from a given charset encoding
 * (see avt_mb_encoding)
 */
AVT_API int avt_tell_mb (const char *txt);

/*
 * the same with a given length
 * the string needn't be terminated then
 * and can contain binary zeros
 */
AVT_API int avt_tell_mb_len (const char *txt, int len);

/*
 * get string (just one line)
 * converted to the given encoding
 *
 * for UTF-8 encoding s should have a capacity of 4 * LINELENGTH Bytes
 */
AVT_API int avt_ask_mb (char *s, int size);

/***********************************************************************/
/* convert text encodings */

/*
 * decode a string into wchar_t
 * size in bytes
 * returns number of characters in dest (without the termination zero)
 * or -1 on error
 * incomplete multibyte sequences can be completed the next turn
 * dest must be freed by caller with avt_free
 */
AVT_API int avt_mb_decode (wchar_t **dest, const char *src, int src_size);

/*
 * decode a string into wchar_t with a fixed buffer
 * sizes in bytes
 * returns number of characters in dest (without the termination zero)
 * or -1 on error
 * incomplete multibyte sequences can be completed the next turn
 * dest gets as much as fits
 * dest should have a size of (src + 1) * 4
 */
AVT_API int avt_mb_decode_buffer (wchar_t *dest, int dest_size,
                                  const char *src, int src_size);

/*
 * encode a string from wchar_t
 * len is the length
 * returns the size of dest (without the termination zero)
 * or -1 on error
 * dest must be freed by caller with avt_free
 * (the size of dest may be much more than needed)
 */
AVT_API int avt_mb_encode (char **dest, const wchar_t *src, int len);

/*
 * encode a string from wchar_t with a fixed buffer
 * len is the length
 * returns the size of dest (without the termination zero)
 * or -1 on error
 * dest gets as much as fits (terminator reserved)
 * (in UTF-8 a character can have up to 4 bytes)
 */
AVT_API int avt_mb_encode_buffer (char *dest, int dest_size,
                                  const wchar_t * src, int len);

/*
 * recode string
 * if either tocode or fromcode is NULL, it uses the current mb_encoding
 * the empty string is accepted as normal enconding name
 * the string src must not have incomplete characters at the end
 *
 * returns the size of dest (without the termination zeros)
 * or -1 on error
 * dest must be freed with avt_free
 */
AVT_API int avt_recode (const char *tocode, const char *fromcode,
                        char **dest, const char *src, int src_size);

/*
 * recode a string with a fixed buffer
 * otherwise the same as avt_recode
 *
 * returns the size of dest (without the termination zeros)
 * or -1 on error
 * dest gets as much as fits
 * 4 Bytes in the dest buffer are reserved for a terminator
 */
AVT_API int avt_recode_buffer (const char *tocode, const char *fromcode,
		char *dest, int dest_size, const char *src, int src_size);

/* free memory allocated by this library */
AVT_API void avt_free (void *ptr);

/***********************************************************************/
/* informational stuff */

/* is it initialized? */
AVT_API bool avt_initialized (void);

/* 0 = normal; 1 = quit-request; -1 = error */
AVT_API int avt_get_status (void);

/* set status */
AVT_API void avt_set_status (int status);

/* get error message */
AVT_API char *avt_get_error (void);

/* which version of the linked library is used? */
AVT_API const char *avt_version (void);

/* get copyright information */
AVT_API const char *avt_copyright (void);

/* get license information */
AVT_API const char *avt_license (void);

/* get color name of given number, or NULL on error */
AVT_API const char *avt_get_color_name (int nr);

/*
 * get the color definition
 * returns the color name or NULL on error
 */
AVT_API const char *avt_get_color (int nr, int *red, int *green, int *blue);

/*
 * get color values for a given color-name
 * returns 0 on success or -1 on error
 */
AVT_API int avt_name_to_color (const char *name,
			       int *red, int *green, int *blue);

/***********************************************************************/
/* settings */

/*
 * change the title and/or the shortname
 * use NULL for the unchanged part
 * in newer SDL-versions it depends on avt_mb_encoding()
 * if possible stick to ASCII for compatibility
 */
AVT_API void avt_set_title (const char *title, const char *shortname);

/*
 * set name for the avatar
 * set to NULL to clear the name
 */
AVT_API int avt_set_avatar_name (const wchar_t *name);
AVT_API int avt_set_avatar_name_mb (const char *name);

/* switch to fullscreen or window mode */
AVT_API void avt_switch_mode (int mode);

/* toggle fullscreen mode */
AVT_API void avt_toggle_fullscreen (void);

/* get mode */
AVT_API int avt_get_mode (void);

/*
 * set the baloon width and height in number of characters
 * 0 or less for maximum width
 * if it's actually changed, the balloon is redrawn and emptied
 * see also avt_get_max_x () and avt_get_max_y ()
 */
AVT_API void avt_set_balloon_size (int height, int width);
AVT_API void avt_set_balloon_width (int width);
AVT_API void avt_set_balloon_height (int height);

/* activate the text cursor? (default: no) */
AVT_API void avt_activate_cursor (bool on);

/*
 * define the background color
 * values in the range 0x00 .. 0xFF
 * can and should be called before avt_initialize
 * if the balloon is visible, it is cleared
 */
AVT_API void avt_set_background_color (int red, int green, int blue);
AVT_API void avt_set_background_color_name (const char *name);
AVT_API void avt_get_background_color (int *red, int *green, int *blue);

/*
 * define the balloon color
 * values in the range 0x00 .. 0xFF
 * can be called before avt_initialize
 * the text-background-color is set to the balloon-color too
 * if the balloon is visible, it is cleared
 */
AVT_API void avt_set_balloon_color (int red, int green, int blue);
AVT_API void avt_set_balloon_color_name (const char *name);

/*
 * change the text color
 * values in the range 0x00 .. 0xFF
 */
AVT_API void avt_set_text_color (int red, int green, int blue);
AVT_API void avt_set_text_color_name (const char *name);
AVT_API void avt_set_text_background_color (int red, int green, int blue);
AVT_API void avt_set_text_background_color_name (const char *name);

/* set text background to balloon color */
AVT_API void avt_set_text_background_ballooncolor (void);

/*
 * set text direction
 * the cursor is moved to start of the line
 * in a text, you might want to call avt_newline after that
 */
AVT_API void avt_text_direction (int direction);

/*
 * delay time for text-writing
 * default: AVT_DEFAULT_TEXT_DELAY
 */
AVT_API void avt_set_text_delay (int delay);

/*
 * delay time for page flipping
 * default: AVT_DEFAULT_FLIP_PAGE_DELAY
 */
AVT_API void avt_set_flip_page_delay (int delay);

/* set underlined mode on or off */
AVT_API void avt_underlined (bool onoff);

/* get underlined mode */
AVT_API bool avt_get_underlined (void);

/* set bold mode on or off (not recommended) */
AVT_API void avt_bold (bool onoff);

/* get bold mode */
AVT_API bool avt_get_bold (void);

/* set inverse mode on or off */
AVT_API void avt_inverse (bool onoff);

/* get inverse mode */
AVT_API bool avt_get_inverse (void);

/* set default color and switch off bold, underlined, inverse */
/* also switches the markup mode off */
AVT_API void avt_normal_text (void);

/*
 * switch markup mode on or off
 *
 * in markup mode the character "_" toggles the underlined mode
 * and the character "*" toggles the bold mode on or off
 */
AVT_API void avt_markup (bool onoff);

/*
 * set scroll mode
 * -1 = off, 0 = page-flipping, 1 = normal
 */
AVT_API void avt_set_scroll_mode (int mode);
AVT_API int avt_get_scroll_mode (void);

/* set newline mode (default: on) */
AVT_API void avt_newline_mode (bool mode);

/* get newline mode */
AVT_API bool avt_get_newline_mode (void);

/* set auto-margin mode (default: on) */
AVT_API void avt_set_auto_margin (bool mode);
AVT_API void avt_auto_margin (bool mode);

/* get auto-margin mode */
AVT_API bool avt_get_auto_margin (void);

/*
 * origin mode
 * false: origin (1,1) is always the top of the textarea
 * true:  origin (1,1) is the top of the viewport
 */
AVT_API void avt_set_origin_mode (bool mode);
AVT_API bool avt_get_origin_mode (void);

/*
 * with this you can switch the mouse pointer on or off
 * use this after avt_register_mousehandler
 */
AVT_API void avt_set_mouse_visible (bool visible);


/***********************************************************************/
/* handle coordinates inside the balloon */

/*
 * the coordinates start with 1, 1 in the upper left corner
 * and are independent from the text direction
 */

/* get position in the viewport */
AVT_API int avt_where_x (void);
AVT_API int avt_where_y (void);

/*
 * is the cursor in the home position?
 * (also works for right-to-left writing)
 */
AVT_API bool avt_home_position (void);

/* maximum positions (whole text-field) */
AVT_API int avt_get_max_x (void);
AVT_API int avt_get_max_y (void);

/* put cusor to specified coordinates */
AVT_API void avt_move_x (int x);
AVT_API void avt_move_y (int y);
AVT_API void avt_move_xy (int x, int y);

/* save and restore current cursor position */
AVT_API void avt_save_position (void);
AVT_API void avt_restore_position (void);

/* set a viewport (sub-area of the textarea) */
AVT_API void avt_viewport (int x, int y, int width, int height);

/***********************************************************************/
/* activities inside the balloon */

/* new line - same as \n in avt_say */
AVT_API int avt_new_line (void);

/* wait a while and then clear the textfield - same as \f in avt_say */
AVT_API int avt_flip_page (void);

/*
 * clears the viewport
 * if there is no balloon yet, it is drawn
 */
AVT_API void avt_clear (void);

/*
 * clears from cursor position down the viewport
 * if there is no balloon yet, it is drawn
 */
AVT_API void avt_clear_down (void);

/*
 * clears from cursor position up the viewport
 * if there is no balloon yet, it is drawn
 */
AVT_API void avt_clear_up (void);

/*
 * clear end of line
 * depending on text direction
 */
AVT_API void avt_clear_eol (void);

/*
 * clear beginning of line
 * depending on text direction
 */
AVT_API void avt_clear_bol (void);

/* clear line */
AVT_API void avt_clear_line (void);

/*
 * forward one character position
 * ie. print a space
 */
AVT_API int avt_forward (void);

/* delete last caracter */
AVT_API void avt_backspace (void);

/* insert spaces at current position (move rest of line) */
AVT_API void avt_insert_spaces (int num);

/* delete num characters at current position (move rest of line) */
AVT_API void avt_delete_characters (int num);

/*
 * erase num characters from current position
 * don't move the cursor or the rest of the line
 */
AVT_API void avt_erase_characters (int num);

/* go to next or last tab stop */
AVT_API void avt_next_tab (void);
AVT_API void avt_last_tab (void);

/* reset tab stops to every eigth column */
AVT_API void avt_reset_tab_stops (void);

/* clear all tab stops */
AVT_API void avt_clear_tab_stops (void);

/* set or clear a tab in position x */
AVT_API void avt_set_tab (int x, bool onoff);

/*
 * delete num lines, starting from line
 * the rest ist scrolled up
 */
AVT_API void avt_delete_lines (int line, int num);

/*
 * insert num lines, starting at line
 * the rest ist scrolled down
 */
AVT_API void avt_insert_lines (int line, int num);

/*
 * lock or unlock updates of the balloon-content
 * can be used for speedups
 * when set to false, the textarea gets updated
 * when set to true, the text_delay is set to 0
 * use with care!
 */
AVT_API void avt_lock_updates (bool lock);

/***********************************************************************/
/* showing images without the avatar */
/* you should call avt_wait or avt_wait_button or avt_get_key thereafter */

/*
 * X-Pixmaps (XPM), X Bitmaps (XBM) and uncompressed BMP are always supported
 * other image formats are supported with SDL_image
 */

AVT_API int avt_image_max_width (void);
AVT_API int avt_image_max_height (void);

/*
 * load image file and show it
 * on error it returns AVT_ERROR without changing the status
 */
AVT_API int avt_show_image_file (const char *file);

/*
 * load image from stream and show it
 * on error it returns AVT_ERROR without changing the status
 */
AVT_API int avt_show_image_stream (avt_stream *stream);

/*
 * show image from image data
 * on error it returns AVT_ERROR without changing the status
 */
AVT_API int avt_show_image_data (void *img, int imgsize);

/*
 * show image from XPM data
 * on error it returns AVT_ERROR without changing the status
 */
AVT_API int avt_show_image_xpm (char **xpm);

/*
 * show image from XBM data with a given color
 * the background is transparent
 * on error it returns AVT_ERROR without changing the status
 */
AVT_API int avt_show_image_xbm (const unsigned char *bits,
				int width, int height,
				const char *colorname);

/*
 * show raw image
 * only 3 or 4 Bytes per pixel supported (RGB or RGBA)
 */
AVT_API int avt_show_raw_image (void *image_data, int width, int height,
				int bytes_per_pixel);

/***********************************************************************/
/* high-level functions */

/* wait for a keypress while displaying a button */
AVT_API int avt_wait_button (void);

/*
 * show positive or negative buttons
 * keys for positive: + 1 Enter
 * keys for negative: - 0 Backspace
 *
 * returns the result as boolean
 * on error or quit request false is returned and the status is set
 * you should check the status with avt_get_status()
 */
AVT_API bool avt_decide (void);

/*
 * navigation bar
 *
 * buttons is a string with the following characters
 * 'l': left
 * 'r': right (play)
 * 'd': down
 * 'u': up
 * 'x': cancel
 * 'f': (fast)forward
 * 'b': (fast)backward
 * 'p': pause
 * 's': stop
 * 'e': eject
 * '*': circle (record)
 * '+': plus (add)
 * '-': minus (remove)
 * '?': help
 * ' ': spacer (no button)
 *
 * Pressing a key with one of those characters selects it.
 * For the directions you can also use the arrow keys,
 * The [Pause] key returns 'p'.
 * The [Help] key or [F1] return '?'.
 *
 * the function returns the letter for the selected option
 * or AVT_ERROR or AVT_QUIT
 *
 * If audio output ends while this function is active, it automatically
 * pushes either 's' (stop) or 'f' (forward).  If both are given, then
 * the rightmost.
 *
 * example:
 *   r = avt_navigate ("lxr");
 *   if (r < 32) exit (0);
 *   select (r) ...
 */
AVT_API int avt_navigate (const char *buttons);

/*
 * avt_choice - use for menus
 * result:        result code, first item is 1
 * start_line:    line, where choice begins
 * items:         number of items/lines
 * key:           first key, like '1' or 'a', 0 for no keys
 * back, forward: whether first/last entry is a back/forward function
 *
 * returns AVT_ERROR and sets _avt_STATUS when it cannot get enough memory
 */
AVT_API int
avt_choice (int *result, int start_line, int items, int key,
	    bool back, bool forward);

/*
 * show longer text with a text-viewer application
 * if len is 0, assume 0-terminated string
 * startline is only used, when it is greater than 1
 */
AVT_API int avt_pager (const wchar_t *txt, int len, int startline);
AVT_API int avt_pager_mb (const char *txt, int len, int startline);

/* show final credits */
AVT_API int avt_credits (const wchar_t *text, bool centered);
AVT_API int avt_credits_mb (const char *text, bool centered);

/***********************************************************************/
/* plumbing */

/*
 * reserve single keys (Esc, F11)
 * use this with avt_register_keyhandler
 */
AVT_API void avt_reserve_single_keys (bool onoff);

/*
 * type for keyhandler
 * see avt_register_keyhandler
 */
typedef void (*avt_keyhandler) (int sym, int mod, int unicode);

/* register an external keyhandler */
AVT_API void avt_register_keyhandler (avt_keyhandler handler);

/*
 * type for mousehandler
 * see avt_register_mousehandler
 */
typedef void (*avt_mousehandler) (int button, bool pressed,
				  int x, int y);

/* register an external mousehandler
 *
 * it is only called, when a mouse-button is pressed or released
 * The coordinates are the character positions if it's inside of
 * the balloon or -1, -1 otherwise.
 */
AVT_API void avt_register_mousehandler (avt_mousehandler handler);

/*
 * returns pointer to character definition of given codepoint
 * either defined as unsigned char or unsigned short,
 * depending on the fonts width
 */
AVT_API void *get_font_char (int ch);

/*
 * get height and width of a character
 * the font is a fixed width font
 */
AVT_API void avt_get_font_size (int *width, int *height);

/***********************************************************************/
/* audio stuff */

/* must be called AFTER avt_initialize! */
AVT_API int avt_initialize_audio (void);

/*
 * quit audio system
 * (automatically called by avt_quit())
 */
AVT_API void avt_quit_audio (void);

/*
 * supported audio formats:
 * AU:  linear PCM with up to 32Bit, mu-law, A-law
 * WAV: linear PCM with up to 16Bit, MS-ADPCM, IMA-ADPCM
 * Both: mono or stereo
 *
 * the current implementation can only play sounds with 
 * up to 16Bit precision, but AU-files with more Bits can
 * be read.
 */

/*
 * loads an audio file in AU or Wave format
 * not for headerless formats
 */
AVT_API avt_audio_t *avt_load_audio_file (const char *filename);

/*
 * loads audio in AU or Wave format from a stream
 * if maxsize is >0 it reads no more than this size
 * maxsize is ignored for Wave data
 * not for headerless formats
 */
AVT_API avt_audio_t *avt_load_audio_part (avt_stream *stream, int maxsize);

/*
 * loads audio in AU or Wave format from a stream
 * not for headerless formats
 */
AVT_API avt_audio_t *avt_load_audio_stream (avt_stream *stream);

/*
 * loads audio in AU or Wave format from memory
 * must still be freed with avt_free_audio!
 */
AVT_API avt_audio_t *avt_load_audio_data (void *data, int datasize);

/* values for audio_type */
#define AVT_AUDIO_UNKNOWN   0  /* doesn't play */
#define AVT_AUDIO_U8        1  /* unsigned 8 Bit */
#define AVT_AUDIO_S8        2  /* signed 8 Bit */
#define AVT_AUDIO_U16LE     3  /* unsigned 16 Bit little endian */
#define AVT_AUDIO_U16BE     4  /* unsigned 16 Bit big endian */
#define AVT_AUDIO_U16SYS    5  /* unsigned 16 Bit system's endianess */
#define AVT_AUDIO_S16LE     6  /* signed 16 Bit little endian */
#define AVT_AUDIO_S16BE     7  /* signed 16 Bit big endian */
#define AVT_AUDIO_S16SYS    8  /* signed 16 Bit system's endianess */
#define AVT_AUDIO_MULAW   100  /* 8 Bit mu-law (u-law) */
#define AVT_AUDIO_ALAW    101  /* 8 Bit A-Law */

/* for channels */
#define AVT_AUDIO_MONO      1
#define AVT_AUDIO_STEREO    2

/*
 * loads raw audio data from memory
 * the data buffer is copied and can be freed immediately
 *
 * audio_type is one of the AVT_AUDIO_* constants
 * channels is AVT_MONO or AVT_STEREO
 *
 * you can use avt_add_raw_audio_data to add data later
 * then you can start with a data_size of zero here
 *
 * must be freed with avt_free_audio! (even if empty)
 */
AVT_API avt_audio_t *avt_load_raw_audio_data (void *data, int data_size,
			int samplingrate, int audio_type, int channels);

/*
 * add raw audio data to an audio type
 * the audio type must have been created with avt_load_raw_audio_data
 * data should be a larger buffer
 */
AVT_API int avt_add_raw_audio_data (avt_audio_t *snd, void *data, int data_size);

/*
 * frees memory of a loaded sound
 */
AVT_API void avt_free_audio (avt_audio_t *snd);

/*
 * plays a sound
 * on error it returns AVT_ERROR without changing the status
 */
AVT_API int avt_play_audio (avt_audio_t *snd, bool doloop);

/*
 * wait until the sound ends
 * this stops a loop, but still plays to the end of the sound
 */
AVT_API int avt_wait_audio_end (void);

/* stops audio immediately */
AVT_API void avt_stop_audio (void);

/* pause/resume audio */
AVT_API void avt_pause_audio (bool pause);

/*
 * Is this sound currently playing?
 * Use NULL for any sound
 */
AVT_API bool avt_audio_playing (avt_audio_t *snd);


/***********************************************************************/
/* deprecated functions - only for backward comatibility */
/* don't use them for new programs! */

/* macro for marking deprecated functions in this header */
#if defined (__GNUC__) && !defined (_AVT_NO_DEPRECATED)
#  define AVT_DEPRECATED  __attribute__ ((__deprecated__))
#else
#  define AVT_DEPRECATED
#endif /* __GNUC__ */

/* currently none */

AVT_END_DECLS

#endif /* _akfavatar_h */
