/*
 * AKFAvatar Terminal emulation for Lua 5.1
 * ATTENTION: this is work in progress, ie. not finished yet
 * Copyright (c) 2010 Andreas K. Foerster <info@akfoerster.de>
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

#include "akfavatar.h"
#include "avtaddons.h"

#ifndef NO_LANGINFO
#  include <langinfo.h>
#endif

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

static int
quit (lua_State * L)
{
  if (avt_get_status () <= AVT_ERROR)
    {
      return luaL_error (L, "%s", avt_get_error ());
    }
  else				/* stop requested */
    {
      /* no actual error, so no error message */
      /* this is handled by the calling program */
      lua_pushnil (L);
      return lua_error (L);
    }
}

/*
 * ececute program in terminal
 * accepts a program and its parameters as separate arguments
 * if no arguments are given, it starts the default shlell
 */
static int
lterm_execute (lua_State * L)
{
  int fd;
  int n, i;
  char encoding[256];
  const char *argv[256];

#ifdef NO_LANGINFO
  /* get encoding from AKFAvatar settings */
  strncpy (encoding, avt_get_mb_encoding (), sizeof (encoding));
#else
  /* get encoding from system settings */
  /* conforming to SUSv2, POSIX.1-2001 */
  strncpy (encoding, nl_langinfo (CODESET), sizeof (encoding));
#endif

  encoding[sizeof (encoding) - 1] = '\0';	/* enforce termination */

  n = lua_gettop (L);		/* number of options */

  if (n >= 1)
    {
      if (n > 255)
	n = 255;

      for (i = 0; i < n; i++)
	argv[i] = luaL_checkstring (L, i + 1);
      argv[n] = NULL;

      fd = avta_term_start (encoding, NULL, (char **) argv);
    }
  else				/* start shell */
    fd = avta_term_start (encoding, NULL, NULL);

  avt_clear ();

  if (fd != -1)
    avta_term_run (fd);

  if (avt_get_status () != AVT_NORMAL)
    quit (L);

  return 0;
}

static const struct luaL_reg termlib[] = {
  {"execute", lterm_execute},
  {NULL, NULL}
};

int
luaopen_term (lua_State * L)
{
  luaL_register (L, "term", termlib);
  return 1;
}