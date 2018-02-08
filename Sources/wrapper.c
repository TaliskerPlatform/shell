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

/** shell_wrapper_exec()
 *
 * Attempt to invoke @argv[1] as a sub-command of @argv[0]. For example,
 * if @argv[] = { "talisker", "help", NULL }, then
 * we attempt to locate and execute "talisker-help" (any spaces present in
 * @argv[0] will also be transformed to dashes).
 *
 * In doing so, we merge @argv[0..1] and shift the remaining members left by
 * one. Therefore, the following:
 *
 *    { "talisker", "help", "set", NULL }
 *
 * becomes this, when passed to the child process:
 *
 *    { "talisker help", "set", NULL }
 *
 * If @argv[0] contains any additional path components, they will be removed.
 * As a special case, if `NDEBUG` is not defined, and @argv[0] (modulo any
 * additional path components) begins with `lt-` (the prefix for 
 * `libtool`-generated executables being executed within a source tree),
 * then it shall also be removed.
 *
 * The return value from this function is expected to be returned as an exit
 * status.
 *
 * The exit status 127 indicates the specified command was not found. Other
 * return values are as described for shell_spawn().
 */

int
shell_wrapper_exec(SHELL *shell, int argc, char **argv, char **envp)
{
	const char *basedir = UTILSDIR; 
	const char *operand;
	char namebuf[_POSIX_PATH_MAX+1], argbuf[_POSIX_PATH_MAX+1];
	size_t l, basedirlen;
	int r;
	
	if(argc < 2)
	{
		return 127;
	}
	basedirlen = strlen(basedir);
	operand = argv[1];
	l = strlen(shell->progname);
	if(l + strlen(operand) + 2 >= sizeof(argbuf))
	{
		return 127;
	}
	strcpy(argbuf, shell->progname);
	argbuf[l] = ' ';
	strcpy(&(argbuf[l + 1]), operand);
	argv++;
	argc--;
	argv[0] = argbuf;
	/* First, try UTILSDIR "/" argbuf */
	if(basedirlen + strlen(argbuf) + 2 < sizeof(namebuf))
	{
		strcpy(namebuf, basedir);
		namebuf[basedirlen] = '/';
		strcpy(&(namebuf[basedirlen + 1]), argbuf);
		for(l = 0; namebuf[l]; l++)
		{
			if(namebuf[l] == ' ')
			{
				namebuf[l] = '-';
			}
		}
		r = shell_spawn(shell, namebuf, argc, argv, envp);
		if(r != 127)
		{
			return r;
		}
	}
	else
	{
		r = 127;
	}
#if SHELL_TRY_BUNDLED_PATH
	/* Next, try argbuf with spaces transformed to dashes, in the $PATH */
	strcpy(namebuf, argbuf);
	for(l = 0; namebuf[l]; l++)
	{
		if(namebuf[l] == ' ')
		{
			namebuf[l] = '-';
		}
	}
	r = shell_spawn(shell, namebuf, argc, argv, envp);
#endif
	if(r == 127)
	{
		SHELL_CRIT_T(shell, NOTFOUND, shell->progname);
	}
	return r;
}
