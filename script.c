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

/* In future this will change, so we are reserving the behaviour
 * in the event of a pathname being given: if the first argument
 * is a string containing multiple path components then it is
 * assumed that it is the pathname of a script intended
 * to be executed.
 *
 * At present we simply return a `EPERM` (Operation not permitted)
 * failure status.
 */
int
shell_script_exec(SHELL *shell, int argc, char **argv, char **envp)
{
	struct stat sbuf;
	
	(void) envp;
	
	if(argc < 2)
	{
		return 125;
	}
	errno = 0;
	stat(argv[1], &sbuf);
	if(!errno)
	{
		errno = EPERM;
	}		
	fprintf(stderr, "%s: %s: %s\n", shell->progname, argv[1], strerror(errno));
	return 125;
}