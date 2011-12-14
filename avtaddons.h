/*
 * different addons for AKFAvatar
 * Copyright (c) 2007, 2008, 2009, 2010, 2011 Andreas K. Foerster <info@akfoerster.de>
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

/*
 * This library is not so strictly handled as the main library.
 * This library uses the libc directly and can use system-specific
 * functions.
 * Not all parts of this library are available on all platforms!
 */

#ifndef _AVTADDONS_H
#define _AVTADDONS_H

#include "akfavatar.h"
#include <wchar.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

/* for later use */
#define AVT_ADDON  extern

#ifdef AVTADDONS_DLL
  AVT_BEGIN_DECLS
#endif

/**********************************************************************
 * Section: avtccio
 * C-specific functions for input/output
 *
 * the calling program must have used avt_initialize and
 * avt_mb_encoding before calling any of these functions.
 **********************************************************************/

AVT_ADDON int avta_printf (const char *format, ...);
AVT_ADDON int avta_putchar (int c);
AVT_ADDON int avta_puts (const char *s);
AVT_ADDON int avta_scanf (const char *format, ...);
AVT_ADDON int avta_vprintf (const char *format, va_list ap);
AVT_ADDON int avta_vscanf (const char *format, va_list ap);

/**********************************************************************
 * Section: avtcwio
 * C-specific functions for wide characters input/output
 *
 * the calling program must have used avt_initialize before calling 
 * any of these functions.
 **********************************************************************/

AVT_ADDON int avta_wprintf (const wchar_t *format, ...);
AVT_ADDON wint_t avta_putwchar (wchar_t c);
AVT_ADDON int avta_putws (const wchar_t *s);
AVT_ADDON int avta_wscanf (const wchar_t *format, ...);
AVT_ADDON int avta_vwprintf (const wchar_t *format, va_list ap);
AVT_ADDON int avta_vwscanf (const wchar_t *format, va_list ap);


/**********************************************************************
 * Section: avtmsg
 * message output for avatarsay
 **********************************************************************/

/*
 * the output takes place on stderr when this exists
 * or in message boxes on some other systems
 *
 * these functions are also used by other parts of avtaddons,
 * so they are most likely included
 *
 * "avta_warning", "avta_notice" and "avta_error" take 2 message strings
 * the second one may simply be NULL if you don't need it
 */

/*
 * set the name of the programm, default: "AKFAvatar"
 * the string must be kept available - a string literal is okay
 */
AVT_ADDON void avta_prgname (const char *name);

/* info on stdout */
AVT_ADDON void avta_info(const char *msg);

/* warning on stderr */
AVT_ADDON void avta_warning (const char *msg1, const char *msg2);

/* unimportant notice (stderr - might be ignored) */
AVT_ADDON void avta_notice (const char *msg1, const char *msg2);

/* error that quits the program (stderr) */
AVT_ADDON void avta_error (const char *msg1, const char *msg2);

/**********************************************************************
 * Section: colorchooser
 * color-chooser for AKFAvatar
 **********************************************************************/

AVT_ADDON const char *avta_color_selection (void);

/**********************************************************************
 * Section: filechooser
 * file-chooser for AKFAvatar
 **********************************************************************/

typedef bool (*avta_filter_t) (const char *filename);

/* file selection
 * starts in current working directory - changes the working directory
 * if filter is NULL it shows all files (but no hidden files)
 * returns 0 on success or -1 on error
 */
AVT_ADDON int avta_file_selection (char *filename, int filename_size,
                                   avta_filter_t filter);

/* only for backward compatibility: */
#define avta_get_file(f)  avta_file_selection(f, sizeof(f), NULL)

/**********************************************************************
 * Section: avtreadfile
 * for reading a file into a buffer
 * the buffer is allocated with malloc and must be freed by the caller
 **********************************************************************/

/*
 * read a text file
 * if file_name is NULL read from stdin
 * the string will be termitated by four zero-bytes
 * returns length of the string without the terminating zeros,
 * or -1 on error
 */
AVT_ADDON int avta_read_textfile (const char *file_name, char **buffer);

/*
 * read a data file
 * returns the size of the buffer in bytes, or -1 on error
 */
AVT_ADDON int avta_read_datafile (const char *file_name, void **buffer);

/*
 * get the output of the command (not for interactive programs)
 * the string will be termitated by four zero-bytes
 * returns length of the string without the terminating zeros,
 * or -1 on error
 */
AVT_ADDON int avta_read_command (const char *command, char **buffer);

/*
 * run pager on file with current encoding and balloon-size
 * returns 0 on success or -1 on error
 */
AVT_ADDON int avta_pager_file (const char *file_name, int startline);

/*
 * run pager on output of command with current encoding and 
 * balloon-size
 * returns 0 on success or -1 on error
 */
AVT_ADDON int avta_pager_command (const char *command, int startline);


/**********************************************************************
 * Section: avtvorbis
 * Ogg Vorbis support for AKFAvatar based on stb_vorbis
 **********************************************************************/

AVT_ADDON avt_audio_t *avta_load_vorbis_file (char *filename);
AVT_ADDON avt_audio_t *avta_load_vorbis_data (void *data, int datasize);


/**********************************************************************
 * Section: arch
 * functions for handling ar archives
 **********************************************************************/

/*
 * return file descriptor, if it's an archive
 * or -1 on error
 */
AVT_ADDON int avta_arch_open (const char *archive);

/*
 * finds a member in the archive 
 * and leaves the fileposition at its start
 * the member name may not be longer than 15 characters 
 * returns size of the member, or 0 if not found 
 */
AVT_ADDON size_t avta_arch_find_member (int fd, const char *member);

/*
 * finds first archive member
 * and leaves the fileposition at its start
 * if member is not NULL it will get the name of the member
 * member must have at least 16 bytes
 * returns size of first member
 */
AVT_ADDON size_t avta_arch_first_member (int fd, char *member);

/*
 * read in whole member
 * the result is allocated with malloc and must be freed by the caller
 * the result gets some binary zeros added, so it can be used as string
 * returns NULL on error
 */
AVT_ADDON char *avta_arch_get_member (int fd, size_t size);

/*
 * read in whole member of a named archive
 * the result is allocated with malloc and must be freed by the caller
 * the result gets some binary zeros added, so it can be used as string
 * if size is not NULL it gets the size of the member without the terminator
 * returns NULL on error
 */
AVT_ADDON char *avta_arch_get_data (const char *archive, const char *member,
                                    size_t *size);


/**********************************************************************
 * Section: avtterm
 * terminal emulator for AKFAvatar
 * (not available for MinGW)
 **********************************************************************/

typedef int (*avta_term_apc_cmd) (wchar_t*);

/*
 * execute a subprocess, visible in the balloon
 * if prg_argv == NULL, start a shell
 * returns file-descriptor for output of the process
 * or -1 on error (also if not supported at all)
 */
AVT_ADDON int avta_term_start (const char *system_encoding, 
			       const char *working_dir,
			       char *prg_argv[]);

AVT_ADDON void avta_term_run (int fd);

AVT_ADDON void avta_term_nocolor (bool nocolor);


/* register handler for APC commands (optional) */
AVT_ADDON void avta_term_register_apc (avta_term_apc_cmd command);

/*
 * the following functions are only meant to be called
 * from the APC handler
 */

/* APC: (de)activate slowprint mode */
AVT_ADDON void avta_term_slowprint (bool on);

/* APC: send data to stdin of the running program */
AVT_ADDON void avta_term_send (const char *buf, size_t count);

/* APC: send string literal to stdin of the running program */
#define avta_term_send_literal(l)  avta_term_send("" l, sizeof(l)-1)

/*
 * APC: update size of textarea
 * call this after you have changed the size of the balloon
 */
AVT_ADDON void avta_term_update_size (void);


#ifdef AVTADDONS_DLL
  AVT_END_DECLS
#endif

#endif /* AVTADDONS_H */
