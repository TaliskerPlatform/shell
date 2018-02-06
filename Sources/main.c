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
	SHELL shell;
	
	if(shell_context_init(&shell, argc, argv, environ))
	{
		fprintf(stderr, "%s: shell initialisation failed: %s\n", argv[0], strerror(errno));
		return 125;
	}
	shell_progname_parse(&shell, &argc, argv);
	if(argc < 2)
	{
#if SHELL_WRAPPER
		shell_usage(&shell);
		return 125;
#else
		return shell_interactive(&shell);
#endif
	}
	if(strchr(argv[1], '/'))
	{
#if SHELL_WRAPPER
		shell_usage(&shell);
		return 125;
#else
		return shell_script_exec(&shell, argc, argv, environ);
#endif
	}
	return shell_wrapper_exec(&shell, argc, argv, environ);
}
