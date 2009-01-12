/*
 * avtfilechooser - filechooser dialog for AKFAvatar
 * Copyright (c) 2008 Andreas K. Foerster <info@akfoerster.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include "akfavatar.h"
#include "avtmsg.h"
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

/* entries or marks that are not files */
#define MARK(S) \
         avt_set_text_background_color (0xdd, 0xdd, 0xdd); \
         avt_say (S); avt_normal_text ()

#define PARENT_DIRECTORY L" .. "

/* three arrows up */
#define BACK L" \x2191 \x2191 \x2191 "

/* three arrows down */
#define CONTINUE L" \x2193 \x2193 \x2193 "

/* 3 dots */
#define LONGER L"\x2026"

/* slash */
#define DIRECTORY L"/"

/* how many pages? */
#define MAXPAGES 500

#ifdef __WIN32__
extern int ask_drive (int max_idx);
#else
#  define ask_drive(max_idx) 0
#endif

static avt_bool_t
is_directory (const char *name)
{
  struct stat buf;

  if (stat (name, &buf) > -1)
    return (avt_bool_t) S_ISDIR (buf.st_mode);
  else
    return AVT_FALSE;
}

static void
new_page (char *dirname)
{
  avt_clear ();
  avt_set_text_background_color (0xdd, 0xdd, 0xdd);
  avt_say_mb (dirname);
  avt_clear_eol ();
  avt_normal_text ();
  avt_move_xy (1, 2);
}

/* 
 * filechooser
 * lists files in working directory
 * return -1 on error or 0 on success
 */
int
get_file (char *filename)
{
  int rcode;
  DIR *dir;
  struct dirent *d;
  int max_x, max_idx;
  int idx;
  int filenr;
  char dirname[4096];
  char entry[100][256];
  int page_nr;
  off_t pages[MAXPAGES];

  avt_set_text_delay (0);
  avt_normal_text ();

  /* don't show the balloon */
  avt_show_avatar ();
  /* set maximum size */
  avt_set_balloon_size (0, 0);

  max_x = avt_get_max_x ();
  max_idx = avt_get_max_y () - 1;

  if (ask_drive (max_idx + 1))
    return -1;

  /* set maximum size */
  avt_set_balloon_size (0, 0);

start:
  /* returncode: assume failure as default */
  rcode = -1;
  filenr = -1;
  page_nr = 0;
  *filename = '\0';
  idx = 0;
  if (!getcwd (dirname, sizeof (dirname)))
    warning_msg ("getcwd", strerror (errno));

  avt_auto_margin (AVT_FALSE);
  new_page (dirname);

  dir = opendir (".");
  if (dir == NULL)
    return rcode;

  pages[page_nr] = telldir (dir);

  /* entry for parent directory */
  strcpy (entry[idx], "..");
  MARK (PARENT_DIRECTORY);
  idx++;
  avt_new_line ();

  while (!*filename)
    {
      d = readdir (dir);

      if (!d && !idx)		/* no entries at all */
	break;

      if (!d || (d && d->d_name[0] != '.'))
	{
	  /* end reached? */
	  if (!d || idx == max_idx - 1)
	    {
	      if (d)		/* continue entry */
		{
		  MARK (CONTINUE);
		  idx++;
		}

	      if (avt_choice (&filenr, 2, idx, 0, (page_nr > 0), (d != NULL)))
		break;

	      new_page (dirname);

	      if (d && filenr == idx - 1)	/* continue? */
		{
		  idx = 0;
		  page_nr++;
		  if (page_nr > MAXPAGES - 1)
		    page_nr = MAXPAGES - 1;

		  entry[idx][0] = '\0';
		  MARK (BACK);
		  idx++;
		  avt_new_line ();
		}
	      else if (filenr == 0 && page_nr > 0)	/* back */
		{
		  page_nr--;
		  seekdir (dir, pages[page_nr]);

		  idx = 0;

		  if (page_nr > 0)
		    {
		      entry[idx][0] = '\0';
		      MARK (BACK);
		    }
		  else		/* first page */
		    {
		      strcpy (entry[idx], "..");
		      MARK (PARENT_DIRECTORY);
		    }

		  idx++;
		  avt_new_line ();
		  continue;
		}
	      else		/* file chosen */
		{
		  strcpy (filename, entry[filenr]);
		  rcode = 0;
		  break;
		}
	    }

	  /* copy name into entry */
	  strncpy (entry[idx], d->d_name, sizeof (entry[idx]));
	  avt_say_mb (entry[idx]);

	  /* is it a directory? */
#ifdef _DIRENT_HAVE_D_TYPE
	  /* faster */
	  if (d->d_type == DT_DIR
	      || (d->d_type == DT_LNK && is_directory (d->d_name)))
#else
	  if (is_directory (d->d_name))
#endif /* _DIRENT_HAVE_D_TYPE */
	    {
	      /* mark as directory */
	      if (avt_where_x () > max_x)
		{
		  avt_move_x (max_x - 1);
		  MARK (LONGER);
		}
	      MARK (DIRECTORY);
	    }
	  else			/* not directory */
	    {
	      if (avt_where_x () > max_x)
		{
		  avt_move_x (max_x);
		  MARK (LONGER);
		}
	    }

	  avt_new_line ();
	  idx++;
	  if (idx == max_idx - 1 && page_nr < MAXPAGES - 1)
	    pages[page_nr + 1] = telldir (dir);
	}
    }

  if (closedir (dir) == -1)
    rcode = -1;

  if (is_directory (filename))
    {
      if (chdir (filename))
	warning_msg (filename, "cannot chdir");
      goto start;
    }

  avt_auto_margin (AVT_TRUE);
  avt_clear ();

  return rcode;
}
