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

const char *shell_progname = SHELLNAME;

/* Tidy up the shell's @argv[0], modifying it if necessary, and setting
 * shell_progname as a side-effect to match the (possibly-updated) @argv[0]/
 */

int
shell_progname_parse(int *argc, char **argv)
{
	char *t, *p;
	
	if(*argc < 1)
	{
		return -1;
	}
	shell_progname = argv[0];
	do
	{
		t = strchr(shell_progname, '/');
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
			shell_progname = p;
		}
	}
	while(t);
	argv[0] = (char *) shell_progname;
	return 0;
}
