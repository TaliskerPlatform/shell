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

#include "../p_shell.h"

/* Talisker Administrator's Shell wrapper executable -- see ../wrapper.c
 * for the finer points on how this works. Note that for the wrapper
 * executable, the functionality in that file is essentially all that happens:
 *
 * We do is look for an executable in a pre-defined location (UTILSDIR) or the
 * $PATH, named ${argv[0]}-${argv[1]} and pass control to it, merging 
 * ${argv[0]} and ${argv[1]}, then shifting the remaining arguments.
 *
 * Although the executables we search for are named "${argv[0]}-${argv[1]}",
 * the string supplied to the child process as ${argv[0]} will contain spaces
 * (e.g., "talisker help") rather than dashes (i.e., "talisker-help", the
 * executable name).
 */
int
main(int argc, char **argv)
{
	SHELL shell;
	int r;
	
	if(shell_context_init(&shell, argc, argv, environ))
	{
		SHELL_PERR_CRIT(&shell, INIT);
		return 125;
	}
	/* We must parse the progname before we do any argument manipulation */
	shell_progname_parse(&shell);
	if(shell.argc < 2)
	{
		shell_usage(&shell);
		return 125;
	}
	if(strchr(argv[1], '/'))
	{
		shell_usage(&shell);
		return 125;
	}
	r = shell_wrapper_exec(&shell, argc, argv, environ);
	if(r > 128)
	{
		/* Child terminated due to a signal */
		r = 124;
	}
	return r;
}
