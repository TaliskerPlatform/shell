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

/* Available only to the wrapper */
extern const char *shell_progname;
extern char **environ;

void shell_usage(void);
int shell_progname_parse(int *argc, char **argv);
int shell_spawn(const char *name, int argc, char **argv, char **envp);
int shell_wrapper_exec(int argc, char **argv, char **envp);

/* Available to the rest of the shell */
int shell_interactive(void);
int shell_script_exec(int argc, char **argv, char **envp);

#endif /*!P_SHELL_H_*/
