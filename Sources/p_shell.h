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

/* We primarily target UNIX08 systems */
# define _POSIX_C_SOURCE               200809L

# if !defined(SHELL_MAX_PORTABILITY) && defined(__linux__)
/* On Linux, request visibility of extended, but non-portable APIs */
#  define _GNU_SOURCE                  1
# endif
# if !defined(SHELL_MAX_PORTABILITY) && defined(__APPLE__)
/* On Darwin (macOS, iOS, tvOS, etc.), request visibility of NP APIs */
#  define _DARWIN_C_SOURCE             1
# endif

# include <stdio.h>
# include <stdlib.h>
# include <stdarg.h>
# include <string.h>
# include <errno.h>

# include <spawn.h>

# include <sys/wait.h>
# include <sys/stat.h>

/* The fallback name of the command used to invoke this shell */
# ifndef SHELLNAME
#  define SHELLNAME                    "talisker"
# endif

typedef struct shell_context_struct SHELL;
typedef struct shell_command_struct SHELLCMD;

struct shell_context_struct
{
	int argc;
	char **argv;
	char **envp;
	const char *progname;
	
	/* Diagnostics */
	const char *diag_target;
	int diag_reason;
	int diag_signal;
	int diag_exitstatus;
};

struct shell_command_struct
{
	SHELL *shell;
	const char *cmdline;
	char *pathname;
	int argc;
	char **argv;
	char **envp;
};

typedef enum {
#define DEFDIAG(facility, mnemonic, message) \
	DIAG_ ## facility ## _ ## mnemonic
	DIAG_NONE = 0,
#include "p_messages.h"
	DIAG_MAX
} SHELLDIAG;

typedef enum {
	DIAG_EMERGENCY = 0,
	DIAG_ALERT = 1,
	DIAG_CRITICAL = 2,
	DIAG_CRIT = DIAG_CRITICAL,
	DIAG_ERROR = 3,
	DIAG_ERR = DIAG_ERROR,
	DIAG_WARNING = 4,
	DIAG_WARN = DIAG_WARNING,
	DIAG_NOTICE = 5,
	DIAG_INFO = 6,
	DIAG_DEBUG = 7
} SHELLSEVERITY;

# define SHELL_DIAG_RESET(shell) \
	(shell)->diag_target = NULL; \
	(shell)->diag_reason = 0; \
	(shell)->diag_signal = 0; \
	(shell)->diag_exitstatus = 0;

/* No reason, no target */
# define SHELL_DIAG(shell, sev, code) \
	shell_diag(shell, sev, DIAG_SHELL_ ## code)

/* No reason, target supplied */
# define SHELL_DIAG_T(shell, sev, code, target) \
	(shell)->diag_target = target; \
	shell_diag(shell, sev, DIAG_SHELL_ ## code)

/* Reason from errno, no target */
# define SHELL_PERR(shell, sev, code) \
	(shell)->diag_reason = errno; \
	shell_diag(shell, sev, DIAG_SHELL_ ## code)

/* Reason from errno, target supplied */
# define SHELL_PERR_T(shell, sev, code, target) \
	(shell)->diag_reason = errno; \
	(shell)->diag_target = target; \
	shell_diag(shell, sev, DIAG_SHELL_ ## code)

# define SHELL_PERR_CRIT(shell, code) SHELL_PERR(shell, DIAG_CRIT, code)
# define SHELL_PERR_CRIT_T(shell, code, target) SHELL_PERR_T(shell, DIAG_CRIT, code, target)
# define SHELL_CRIT(shell, code) SHELL_DIAG(shell, DIAG_CRIT, code)
# define SHELL_CRIT_T(shell, code, target) SHELL_DIAG_T(shell, DIAG_CRIT, code, target)

# define SHELL_PERR_ERR(shell, code) SHELL_PERR(shell, DIAG_ERROR, code)
# define SHELL_PERR_ERR_T(shell, code, target) SHELL_PERR_T(shell, DIAG_ERROR, code, target)
# define SHELL_ERR(shell, code) SHELL_DIAG(shell, DIAG_ERROR, code)
# define SHELL_ERR_T(shell, code, target) SHELL_DIAG_T(shell, DIAG_ERROR, code, target)

# define SHELL_PERR_WARN(shell, code) SHELL_PERR(shell, DIAG_WARNING, code)
# define SHELL_PERR_WARN_T(shell, code, target) SHELL_PERR_T(shell, DIAG_WARNING, code, target)
# define SHELL_WARN(shell, code) SHELL_DIAG(shell, DIAG_WARNING, code)
# define SHELL_WARN_T(shell, code, target) SHELL_DIAG_T(shell, DIAG_WARNING, code, target)

# define SHELL_PERR_NOTICE(shell, code) SHELL_PERR(shell, DIAG_NOTICE code)
# define SHELL_PERR_NOTICE_T(shell, code, target) SHELL_PERR_T(shell, DIAG_NOTICE, code, target)
# define SHELL_NOTICE(shell, code) SHELL_DIAG(shell, DIAG_NOTICE, code)
# define SHELL_NOTICE_T(shell, code, target) SHELL_DIAG_T(shell, DIAG_NOTICE, code, target)

/* Available in both the normal shell and the wrapper */
extern char **environ;

int shell_context_init(SHELL *context, int argc, char **argv, char **envp);
int shell_context_done(SHELL *context);

void shell_usage(SHELL *context);
int shell_progname_parse(SHELL *context, int *argc, char **argv);
int shell_spawn(SHELL *context, const char *name, int argc, char **argv, char **envp);
int shell_wrapper_exec(SHELL *context, int argc, char **argv, char **envp);

void shell_diag_vprintf(SHELL *shell, SHELLSEVERITY severity, SHELLDIAG code, const char *fmt, va_list ap);
void shell_diag_printf(SHELL *shell, SHELLSEVERITY severity, SHELLDIAG code, const char *fmt, ...);
void shell_diag(SHELL *shell, SHELLSEVERITY severity, SHELLDIAG code);

# ifndef SHELL_WRAPPER
/* Available to the rest of the shell */
int shell_interactive(SHELL *context);
int shell_script_exec(SHELL *context, int argc, char **argv, char **envp);
# endif

#endif /*!P_SHELL_H_*/
