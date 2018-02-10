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

/* Scan command-line arguments and determine whether we should execute
 * a sub-shell before doing anything else.
 */
int
shell_args_scan(SHELL *shell)
{
	int ch;
	
	optreset = 1;
	optind = 1;
	/* Ignore errors for this pass */
	opterr = 0;
	while((ch = getopt(shell->argc, shell->argv, "vdqitlc:f:r:TCs")) != -1)
	{
		switch(ch)
		{
			case 'v':
				if(shell->loglevel > DIAG_INFO)
				{
					shell->loglevel = DIAG_INFO;
				}
				break;
			case 'd':
				if(shell->loglevel > DIAG_DEBUG)
				{
					shell->loglevel = DIAG_DEBUG;
				}
				break;
			case 'q':
				if(shell->loglevel > DIAG_WARN)
				{
					shell->loglevel = DIAG_WARN;
				}
				else if(shell->loglevel == DIAG_WARN)
				{
					shell->loglevel = DIAG_CRIT;
				}
				break;
			case 'i':
				shell->interactive = 1;
				break;
			case 't':
				shell->force_tty = 1;
				break;
			case 'l':
				shell->login_shell = 1;
				break;
			case 'c':
				if(shell->exec_mode)
				{
					fprintf(stderr, "cannot specify both -c and -f together\n");
					return -1;
				}
				shell->exec_command = optarg;
				shell->exec_mode = 1;
				break;
			case 'f':
				if(shell->exec_mode)
				{
					fprintf(stderr, "cannot specify both -c and -f together\n");
					return -1;
				}
				shell->exec_command  = NULL;
				shell->exec_mode = 2;
				break;
			case 'r':
			case 'T':
			case 'C':
				/* Sub-shell modes */
				shell->subshell_mode = ch;
				shell->subshell_arg = optarg;
				break;
			case '?':
				continue;
		}
	}
	return 0;
}

int
shell_args_process(SHELL *shell)
{
	int ch;
	
	/* If we're not invoking a sub-shell, we can do proper argument processing */
	
	optreset = 1;
	optind = 1;
	opterr = 1;
	while((ch = getopt(shell->argc, shell->argv, "vdqitlc:f:r:TC")) != -1)
	{
		switch(ch)
		{
			case '?':
				return -1;
		}
	}
	shell->argv += optind;
	shell->argc -= optind;
	return 0;
}