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

int
shell_spawn(const char *name, int argc, char **argv, char **envp)
{
	pid_t pid;
	int childstat, r;
	
	pid = 0;
	argv[argc] = NULL;
	r = posix_spawnp(&pid, name, NULL, NULL, argv, envp);
	if(r)
	{
		if(r == ENOENT)
		{
			return 127;
		}
		fprintf(stderr, "%s: %s\n", name, strerror(r));
		return -1;
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
			fprintf(stderr, "failed to waitpid(): %s\n", strerror(errno));
			return 255;
		}
		if(WIFEXITED(childstat))
		{
			fprintf(stderr, "child exited with status %d\n", WEXITSTATUS(childstat));
			return WEXITSTATUS(childstat);
		}
		if(WIFSIGNALED(childstat))
		{
			fprintf(stderr, "child exited due to signal %d\n", WTERMSIG(childstat));
			return -WTERMSIG(childstat);
		}
	}
}
