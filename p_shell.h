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

#ifndef P_SHELL_H_
# define P_SHELL_H_                    1

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <errno.h>

# include <spawn.h>
# include <sys/wait.h>
# include <sys/stat.h>

/* The name of the command used to invoke this shell */
# ifndef SHELLNAME
#  define SHELLNAME                    "talisker"
# endif

typedef struct shell_context_struct SHELL;

struct shell_context_struct
{
	int argc;
	char **argv;
	char **envp;
	const char *progname;
};

/* Available only to the wrapper */
extern char **environ;

int shell_context_init(SHELL *context, int argc, char **argv, char **envp);
int shell_context_done(SHELL *context);

void shell_usage(SHELL *context);
int shell_progname_parse(SHELL *context, int *argc, char **argv);
int shell_spawn(SHELL *context, const char *name, int argc, char **argv, char **envp);
int shell_wrapper_exec(SHELL *context, int argc, char **argv, char **envp);

# ifndef SHELL_WRAPPER
/* Available to the rest of the shell */
int shell_interactive(SHELL *context);
int shell_script_exec(SHELL *context, int argc, char **argv, char **envp);
# endif

#endif /*!P_SHELL_H_*/
