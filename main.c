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

#include "p_shell.h"

/* In this initial version, all we do is look for an executable in $PATH or
 * a pre-defined location named SHELLNAME-argv[1] and pass control to it,
 * modifying argv[0] to be SHELLNAME " " argv[1], then shifting the remaining
 * arguments.
 *
 * The first iteration shall add a "SHELLNAME help" command which simply lists
 * the available by commands (by scanning the search path for executables
 * matching the given pattern);
 *
 * The second interation will expand "SHELLNAME help" to support "SHELLNAME
 * help COMMAND" as an alias for "man SHELLNAME-COMMAND".
 *
 * The third iteration will add a basic interactive command-line interpreter
 * into which commands can be typed omitting the "SHELLNAME" prefix.
 *
 * The fourth iteration will add variable interpolation and manipulation.
 */
int
main(int argc, char **argv)
{
	const char *operand;
	char namebuf[128], argbuf[128];
	size_t l;
	int r;
	
	if(argc < 2)
	{
		shell_usage();
		exit(0);
	}
	operand = argv[1];
	l = strlen(SHELLNAME);
	if(l + strlen(operand) + 1 >= sizeof(namebuf))
	{
		r = 127;
	}
	else
	{
		strcpy(namebuf, SHELLNAME);
		namebuf[l] = '-';
		strcpy(&(namebuf[l + 1]), operand);
		strcpy(argbuf, namebuf);
		argbuf[l] = ' ';
		argv++;
		argc--;
		argv[0] = argbuf;
		r = shell_spawn(namebuf, argc, argv, environ);
	}
	if(r == 127)
	{
		fprintf(stderr, "No such command '%s %s'\n", SHELLNAME, operand);
	}
	return r;
}
