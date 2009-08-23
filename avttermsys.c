/*
 * avttermsys - system specific functions for terminal emulation
 * Copyright (c) 2007, 2008, 2009 Andreas K. Foerster <info@akfoerster.de>
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

#include "avttermsys.h"
#include "version.h"
#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#ifdef USE_OPENPTY
#  include <pty.h>
#endif

/* terminal type */
/*
 * this is not dependent on the system on which it runs,
 * but the terminal database should have an entry for this
 */
#define TERM "linux"
#define BWTERM "linux-m"

/* hack to get a macro stringified :-( */
#define XSTR(x) STR(x)
#define STR(x) #x


/* set terminal size */
extern void
avta_term_size (int fd AVT_UNUSED, int height AVT_UNUSED,
		int width AVT_UNUSED)
{
#ifdef TIOCSWINSZ
  struct winsize size;

  size.ws_row = (height > 0) ? height : 0;
  size.ws_col = (width > 0) ? width : 0;
  size.ws_xpixel = size.ws_ypixel = 0;
  ioctl (fd, TIOCSWINSZ, &size);
#endif
}

static char *
get_user_shell (void)
{
  char *shell;

  shell = getenv ("SHELL");

  /* when the variable is not set, dig deeper */
  if (shell == NULL || *shell == '\0')
    {
      struct passwd *user_data;

      user_data = getpwuid (getuid ());
      if (user_data != NULL && user_data->pw_shell != NULL
	  && *user_data->pw_shell != '\0')
	shell = user_data->pw_shell;
      else
	shell = "/bin/sh";	/* default shell */
    }

  return shell;
}

extern int
avta_term_initialize (int *input_fd, int width, int height,
		      avt_bool_t monochrome, const char *working_dir,
		      char *const prg_argv[])
{
  pid_t childpid;
  int master, slave;
  char *terminalname;
  struct termios settings;
  char *shell = "/bin/sh";

  if (prg_argv == NULL)
    shell = get_user_shell ();

#ifdef USE_OPENPTY

  if (openpty (&master, &slave, NULL, NULL, NULL) < 0)
    return -1;

#else /* not USE_OPENPTY */

  /* as specified in POSIX.1-2001 */
  master = posix_openpt (O_RDWR);

  /* some older systems: */
  /* master = open("/dev/ptmx", O_RDWR); */

  if (master < 0)
    return -1;

  if (grantpt (master) < 0 || unlockpt (master) < 0)
    {
      close (master);
      return -1;
    }

  terminalname = ptsname (master);

  if (terminalname == NULL)
    {
      close (master);
      return -1;
    }

  slave = open (terminalname, O_RDWR);

  if (slave < 0)
    {
      close (master);
      return -1;
    }

#endif /* not USE_OPENPTY */

  /* terminal settings */
  if (tcgetattr (master, &settings) < 0)
    {
      close (master);
      close (slave);
      return -1;
    }

  settings.c_cc[VERASE] = 8;	/* Backspace */
  settings.c_iflag |= ICRNL;	/* input: cr -> nl */
  settings.c_lflag |= (ECHO | ECHOE | ECHOK | ICANON);

  if (tcsetattr (master, TCSANOW, &settings) < 0)
    {
      close (master);
      close (slave);
      return -1;
    }

  avta_term_size (master, height, width);

  /*-------------------------------------------------------- */
  childpid = fork ();

  if (childpid == -1)
    {
      close (master);
      close (slave);
      return -1;
    }

  /* is it the child process? */
  if (childpid == 0)
    {
      /* child closes master */
      close (master);

      /* create a new session */
      setsid ();

      /* redirect stdin, stdout, stderr to slave */
      if (dup2 (slave, STDIN_FILENO) < 0
	  || dup2 (slave, STDOUT_FILENO) < 0
	  || dup2 (slave, STDERR_FILENO) < 0)
	_exit (EXIT_FAILURE);

      close (slave);

      /* unset the controling terminal */
#ifdef TIOCSCTTY
      ioctl (STDIN_FILENO, TIOCSCTTY, 0);
#endif

      if (monochrome)
	putenv ("TERM=" BWTERM);
      else
	putenv ("TERM=" TERM);

      /* programs can identify avatarsay with this */
      putenv ("AKFAVTTERM=" XSTR (AVTVERSIONNR));

      if (working_dir)
	(void) chdir (working_dir);

      if (prg_argv == NULL)	/* execute shell */
	execl (shell, shell, (char *) NULL);
      else			/* execute the command */
	execvp (prg_argv[0], prg_argv);


      /* in case of an error, we can not do much */
      /* stdout and stderr are broken by now */
      _exit (EXIT_FAILURE);
    }

  /* parent process */
  close (slave);
  fcntl (master, F_SETFL, O_NONBLOCK);
  *input_fd = master;

  return master;
}