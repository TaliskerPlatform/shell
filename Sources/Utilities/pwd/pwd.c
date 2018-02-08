/* Talisker
 * Administrator's Shell
 */

/* Copyright 2017 Mo McRoberts.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif
#ifdef HAVE_SYS_STAT_H
# include <sys/stat.h>
#endif

#ifndef EXIT_FAILURE
# define EXIT_FAILURE                  1
#endif

typedef enum
{
	PWD_PHYSICAL,
	PWD_LOGICAL
} PWDMODE;

static void usage(const char *progname);
static const char *getcwd_physical(void);
static const char *getcwd_logical(void);


int
main(int argc, char **argv)
{
	PWDMODE mode = PWD_LOGICAL;
	int ch;
	const char *progname, *cwd;
	
	progname = argv[0];
	while((ch = getopt(argc, argv, "LP")) != -1)
	{
		switch (ch)
		{
		case 'L':
			mode = PWD_LOGICAL;
			break;
		case 'P':
			mode = PWD_PHYSICAL;
			break;
		case '?':
		default:
			usage(progname);
			exit(EXIT_FAILURE);
		}
	}
	if(mode == PWD_LOGICAL)
	{
		cwd = getcwd_logical();
		if(!cwd)
		{
			/* Fall back to getting the physical CWD */
			cwd = getcwd_physical();
		}
	}
	else
	{
		cwd = getcwd_physical();
	}
	if(!cwd)
	{
		fprintf(stderr, "%s: %s\n", progname, strerror(errno));
		exit(EXIT_FAILURE);
	}
	puts(cwd);
	return 0;
}

static void
usage(const char *progname)
{
	fprintf(stderr, "Usage: %s [-L | -P]\n", progname);
}

static const char *
getcwd_logical(void)
{
	const char *cwd, *p;
	struct stat statcwd, stathere;
	
	/* From SUSv4:
	 * "If the PWD environment variable contains an absolute pathname of the
	 *  current directory and the pathname does not contain any components that
	 *  are dot or dot-dot, pwd shall write this pathname to standard output"
	 */
	cwd = getenv("PWD");
	if(!cwd || cwd[0] != '/')
	{
		return NULL;
	}
	for(p = cwd; *p; p++)
	{
		if(p[0] == '/')
		{
			if(p[1] == '.' && (!p[2] || p[2] == '/'))
			{
				return NULL;
			}
			if(p[1] == '.' && p[2] == '.' && (!p[3] || p[3] == '/'))
			{
				return NULL;
			}
		}
	}
	if(stat(cwd, &statcwd))
	{
		return NULL;
	}
	if(stat(".", &stathere))
	{
		return NULL;
	}
	if(statcwd.st_dev != stathere.st_dev ||
		statcwd.st_ino != stathere.st_ino)
	{
		return NULL;
	}
	return cwd;
}

static const char *
getcwd_physical(void)
{
	return getcwd(NULL, 0);
}
