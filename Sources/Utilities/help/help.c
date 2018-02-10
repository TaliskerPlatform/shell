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
#include <dirent.h>

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

#ifndef EXIT_FAILURE
# define EXIT_FAILURE                  1
#endif

static void usage(const char *progname);

int
main(int argc, char **argv)
{
	DIR *dir;
	const char *progname;
	char *base, *t, *p;
	struct dirent de, *dp;
	int ch;
	size_t baselen;
	
	progname = argv[0];
	do
	{
		t = strchr(progname, '/');
		if(t)
		{
			/* Strip trailing slashes */
			for(p = t; *p == '/'; p++) { }
			if(!*p)
			{
				*p = 0;
				break;
			}
			/* Otherwise... */
			progname = p;
		}
	}
	while(t);
	while((ch = getopt(argc, argv, "")) != -1)
	{
		switch (ch)
		{
		case '?':
		default:
			usage(progname);
			exit(EXIT_FAILURE);
		}
	}
	argv += optind;
	argc -= optind;
	if(argc)
	{
		usage(progname);
		exit(EXIT_FAILURE);
	}
	if(chdir(UTILSDIR))
	{
		fprintf(stderr, "%s: %s: %s\n", progname, UTILSDIR, strerror(errno));
		exit(EXIT_FAILURE);
	}
	dir = opendir(".");
	if(!dir)
	{
		fprintf(stderr, "%s: %s: %s\n", progname, UTILSDIR, strerror(errno));
		exit(EXIT_FAILURE);
	}
	base = strdup(progname);
	for(t = base; *t; t++)
	{
		/* Swap spaes for dashes */
		if(*t == ' ')
		{
			*t = '-';
		}
		/* Remove the last component (i.e., 'help') */
		if(t > base && t[0] == '-' &&
			(!t[1] || (!strchr(&(t[1]), ' ') && !strchr(&t[1], '-'))))
		{
			t[1] = 0;
			break;
		}
	}
	baselen = strlen(base);
	printf("Available commands:\n");
	dp = NULL;
	while(readdir_r(dir, &de, &dp) == 0)
	{
		if(!dp)
		{
			break;
		}
		if(dp->d_name[0] == '.')
		{
			/* Ignore dotfiles */
			continue;
		}
		if(strlen(dp->d_name) <= baselen || strncmp(dp->d_name, base, baselen))
		{
			/* Ignore files not beginning with our defined base */
			continue;
		}
		if(access(dp->d_name, X_OK))
		{
			/* Ignore non-executable files */
			continue;
		}
		printf("  %s\n", &(dp->d_name[baselen]));
	}
	closedir(dir);
	return 0;
}

static void
usage(const char *progname)
{
	fprintf(stderr, "Usage: %s\n", progname);
}
