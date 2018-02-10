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

/** shell_spawn()
 *
 * Attempt to spawn the executable @name, which may be a relative or
 * absolute path (if it contains a directory separator character), or it
 * may be the name of an executable which can be found in the $PATH. The
 * supplied @argc, @argv and @envp arguments are passed to the child
 * process.
 *
 * This function waits until the child process completes before returning.
 * 
 * Our return values are expected to be passed to the parent process, and are
 * generally consistent with `docker run`:
 *
 * 127     Executable @name cannot be found
 * 126     Another error occurred invoking @name (a diagnostic has been printed) 
 * 125     Some other internal error occurred
 * 124     The child terminated due to a signal (a diagnostic has been printed)
 * 0..123  Return values from the child process
 */

int
shell_spawn(SHELL *shell, const char *name, int argc, char **argv, char **envp)
{
	pid_t pid;
	int childstat, r;
	
	pid = 0;
	argv[argc] = NULL;
	SHELL_DIAG_RESET(shell);
	r = posix_spawnp(&pid, name, NULL, NULL, argv, envp);
	if(r)
	{
		if(r == ENOENT)
		{
			return 127;
		}
		SHELL_PERR_CRIT_T(shell, NOEXEC, name);
		return 126;
	}
	for(;;)
	{
		childstat = 0;
		r = waitpid(pid, &childstat, 0);
		if(r == -1 && errno == EINTR)
		{
			continue;
		}
		if(r == -1)
		{
			SHELL_PERR_CRIT_T(shell, CHILDWAIT, argv[0]);
			return 125;
		}
		if(WIFSIGNALED(childstat))
		{
			shell->diag_signal = WTERMSIG(childstat);
#ifdef WCOREDUMP
			if(WCOREDUMP(childstat))
			{
				SHELL_NOTICE_T(shell, CHILDSIGC, argv[0]);
			}
			else
#endif
			{
				SHELL_NOTICE_T(shell, CHILDSIG, argv[0]);
			}
			return 128 + WTERMSIG(childstat);
		}
		if(WIFEXITED(childstat))
		{
			shell->diag_exitstatus = WEXITSTATUS(childstat);
			if(WEXITSTATUS(childstat))
			{
				SHELL_INFO_T(shell, CHILDERR, argv[0]);
			}
			return WEXITSTATUS(childstat);
		}
	}
}
