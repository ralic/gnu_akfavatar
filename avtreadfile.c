/*
 * reading data from a file
 * Copyright (c) 2009,2010,2013 Andreas K. Foerster <akf@akfoerster.de>
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

#include "akfavatar.h"
#include "avtaddons.h"
#include <stdio.h>
#include <iso646.h>

static int
read_stream (FILE * f, char **buffer, bool terminate)
{
  char *buf;
  int size, capacity;
  ssize_t nread;

  *buffer = buf = NULL;
  nread = 0;

  /* size must be 0 here! */
  size = capacity = 0;

  do
    {
      /* do we need more capacity? */
      if (size >= capacity)
	{
	  char *nbuf;

	  capacity += 10240;
	  if (terminate)
	    nbuf = (char *) realloc (buf, capacity + 4);
	  else
	    nbuf = (char *) realloc (buf, capacity);

	  if (nbuf)
	    buf = nbuf;
	  else
	    break;
	}

      nread = fread (buf + size, 1, capacity - size, f);
      if (nread > 0)
	size += nread;
    }
  while (nread > 0);

  if (size <= 0)		/* empty file? */
    {
      free (buf);
      buf = NULL;
    }
  else				/* make buffer smaller if possible and eventually add terminator */
    {
      char *nbuf;

      if (terminate)
	{
	  /* I terminate with 4 zeros, in case UTF-32 is used */
	  memset (buf + size, '\0', 4);
	  nbuf = (char *) realloc (buf, size + 4);
	}
      else
	nbuf = (char *) realloc (buf, size);

      if (nbuf)
	buf = nbuf;
    }

  if (not buf)
    size = -1;

  *buffer = buf;
  return size;			/* size without terminator */
}

extern int
avt_read_textfile (const char *file_name, char **buffer)
{
  FILE *f;
  char *buf;
  int size;

  if (not buffer)
    return -1;

  buf = NULL;
  size = -1;

  if (file_name == NULL)
    f = stdin;
  else
    f = fopen (file_name, "rt");

  if (f)
    {
      size = read_stream (f, &buf, true);

      if (f != stdin)
	(void) fclose (f);
    }

  *buffer = buf;
  return size;
}

extern int
avt_read_datafile (const char *file_name, void **buffer)
{
  FILE *f;
  char *buf;
  int size;

  if (not buffer or not file_name)
    return -1;

  buf = NULL;
  size = -1;

  f = fopen (file_name, "rb");

  if (f)
    {
      size = read_stream (f, &buf, false);
      (void) fclose (f);
    }

  *buffer = (void *) buf;
  return size;
}

extern int
avt_read_command (const char *command, char **buffer)
{
  FILE *f;
  char *buf;
  int size;

  if (not buffer or not command)
    return -1;

  buf = NULL;
  size = -1;

  f = popen (command, "r");

  if (f)
    {
      size = read_stream (f, &buf, true);
      (void) pclose (f);
    }

  *buffer = buf;
  return size;
}

extern int
avt_pager_file (const char *file_name, int startline)
{
  char *txt;
  int len;

  txt = NULL;
  len = avt_read_datafile (file_name, (void **) &txt);

  if (len > 0)
    avt_pager_char (txt, len, startline);

  if (txt)
    free (txt);

  if (len > 0)
    return 0;			/* okay */
  else
    return -1;			/* error */
}

extern int
avt_pager_command (const char *command, int startline)
{
  char *txt;
  int len;

  txt = NULL;
  len = avt_read_command (command, &txt);

  if (len > 0)
    avt_pager_char (txt, len, startline);

  if (txt)
    free (txt);

  if (len > 0)
    return 0;			/* okay */
  else
    return -1;			/* error */
}
