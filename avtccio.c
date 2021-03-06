/*
 * C-specific functions for AKFAvatar (multibyte characters)
 * Copyright (c) 2007,2009,2010,2013
 * Andreas K. Foerster <akf@akfoerster.de>
 *
 * the calling program must have used avt_initialize before calling 
 * any of these functions.
 * These functions also require avt_char_encoding to be used before 
 * calling them.
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

#ifndef _GNU_SOURCE
#  define _GNU_SOURCE
#endif

#include "avtaddons.h"
#include <stdio.h>
#include <stdlib.h>


/* vasprintf is a GNU extention and also in BSDs */
#if defined (__USE_GNU) \
   || defined (__FreeBSD__) || defined (__DragonFly__) \
   || defined (__NetBSD__)  || defined (__OpenBSD__)

extern int
avt_vprintf (const char *format, va_list ap)
{
  char *strp;
  int n;

  n = vasprintf (&strp, format, ap);

  if (n > -1)
    {
      avt_say_char_len (strp, n);
      free (strp);
    }

  return n;
}

#else /* not __USE_GNU */

/*
 * this function assumes C99 conforming behaviour of vsnprintf
 * some old implementations just return -1 when buffer is too small
 */

extern int
avt_vprintf (const char *format, va_list ap)
{
  char *str;
  int n;

  str = (char *) malloc (BUFSIZ);
  if (str)
    n = vsnprintf (str, BUFSIZ, format, ap);
  else
    n = -1;

  /* do we need more size? */
  if (n >= BUFSIZ)
    {
      register char *new_str;
      new_str = (char *) realloc (str, n + 1);
      if (new_str)
	{
	  str = new_str;
	  n = vsnprintf (str, n, format, ap);
	}
      else
	n = -1;
    }

  if (str && n > -1)
    avt_say_char_len (str, n);

  if (str)
    free (str);

  return n;
}

#endif /* not __USE_GNU */

extern int
avt_printf (const char *format, ...)
{
  va_list ap;
  int n;

  va_start (ap, format);
  n = avt_vprintf (format, ap);
  va_end (ap);

  return n;
}

/*
 * do not use avt_put_char here,
 * because it doesn't depend on the chosen encoding!
 */
extern int
avt_putchar_char (int c)
{
  unsigned char ch;

  ch = c;
  if (c >= 0)
    avt_say_char_len ((char *) &ch, 1);

  /* return the unsigned char! */
  return (int) ch;
}

extern int
avt_puts (const char *s)
{
  avt_say_char (s);
  avt_new_line ();

  /* return a non-negative number on success */
  return 1;
}

extern int
avt_vscanf (const char *format, va_list ap)
{
  char str[4 * AVT_LINELENGTH + 1];

  avt_ask_char (str, sizeof (str));
  return vsscanf (str, format, ap);
}

extern int
avt_scanf (const char *format, ...)
{
  va_list ap;
  int n;

  va_start (ap, format);
  n = avt_vscanf (format, ap);
  va_end (ap);

  return n;
}
