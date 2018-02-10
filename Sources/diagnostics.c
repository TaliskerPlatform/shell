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

/* Diagnostic messages printed by the Talisker Shell follow a particular
 * format, similar to that used by Compaq OpenVMS and Cisco IOS:
 *
 * %FACILITY-SEVERITY-MNEMONIC: MESSAGE
 *
 * Some messages will include a reason code, which is appended to the message:
 *
 * %FACILITY-SEVERITY-MNEMONIC: MESSAGE (reason code #CODE: REASON)
 *
 * Occasionally, a description of a reason code will not be available on your
 * platform:
 *
 * %FACILITY-SEVERITY-MNEMONIC: MESSAGE (reason code #CODE)
 *
 * The FACILITY is the name of the application producing the diagnostic, in
 * this case SHELL.
 *
 * SEVERITY levels are denoted by a single letter abbreviation:
 *
 *     D = Debug             (least severe)
 *     I = Info
 *     N = Notice
 *     W = Warning
 *     E = Error
 *     C = Critical error    (most severe)
 *
 * The same message may be emitted at different severity levels, depending upon
 * the context: for example, a missing file may be an _Error_ in some
 * situations, but a _Critical error_ in others.
 *
 * The MNEMONICs are the same across all platforms; the reason codes are
 * platform-specific: reason code 47 on Linux may not correspond to reason code
 * 47 on macOS, for example.
 *
 */

struct shell_diag_struct
{
	SHELLDIAG code;
	const char *facility;
	const char *mnemonic;
	const char *message;
};

const struct shell_diag_struct shell_messages[] = {
#undef DEFDIAG
#define DEFDIAG(facility, mnemonic, message) \
	{ DIAG_ ## facility ## _ ## mnemonic, # facility, # mnemonic, message }
# include "p_messages.h"
	{ DIAG_NONE, NULL, NULL, NULL }
};

static const struct shell_diag_struct *shell_diag_locate_(SHELLDIAG diag);
static void shell_diag_printf_internal_(SHELL *shell, SHELLSEVERITY severity, const struct shell_diag_struct *diag, SHELLDIAG code, const char *fmt, ...);
static void shell_diag_vprintf_internal_(SHELL *shell, SHELLSEVERITY severity, const struct shell_diag_struct *diag, SHELLDIAG code, const char *fmt, va_list ap);

/* Print a diagnostic message followed by a custom string */
void
shell_diag_vprintf(SHELL *shell, SHELLSEVERITY severity, SHELLDIAG code, const char *fmt, va_list ap)
{
	const struct shell_diag_struct *diag;
	
	if(shell && severity > shell->loglevel)
	{
		return;
	}
	diag = shell_diag_locate_(code);
	shell_diag_vprintf_internal_(shell, severity, diag, code, fmt, ap);
}

void
shell_diag_printf(SHELL *shell, SHELLSEVERITY severity, SHELLDIAG code, const char *fmt, ...)
{
	va_list ap;
	const struct shell_diag_struct *diag;
	
	if(shell && severity > shell->loglevel)
	{
		return;
	}
	diag = shell_diag_locate_(code);
	va_start(ap, fmt);
	shell_diag_vprintf_internal_(shell, severity, diag, code, fmt, ap);
	va_end(ap);
}

/* Display a diagnostic message */
void
shell_diag(SHELL *shell, SHELLSEVERITY severity, SHELLDIAG code)
{
	const struct shell_diag_struct *diag;
	const char *reasonstr;
	
	if(shell && severity > shell->loglevel)
	{
		return;
	}
	diag = shell_diag_locate_(code);
	if(diag && diag->message)
	{
		shell_diag_printf_internal_(shell, severity, diag, code, "%s", diag->message);
	}
	else
	{
		shell_diag_printf_internal_(shell, severity, diag, code, "Unknown diagnostic #%04d", code);
	}
	if(shell && shell->diag_reason)
	{
		reasonstr = strerror(shell->diag_reason);
		fprintf(stderr, " (reason code #%d", shell->diag_reason);
		if(reasonstr)
		{
			fprintf(stderr, ": %s", reasonstr);
		}
		fputc(')', stderr);
	}
	if(shell && shell->diag_signal)
	{
		fprintf(stderr, " (%s)", strsignal(shell->diag_signal));
	}
	if(shell && shell->diag_exitstatus)
	{
		fprintf(stderr, " (exited with status %d)", shell->diag_exitstatus);
	}
	fputc('\n', stderr);
}

static void
shell_diag_printf_internal_(SHELL *shell, SHELLSEVERITY severity, const struct shell_diag_struct *diag, SHELLDIAG code, const char *fmt, ...)
{
	va_list ap;
	
	if(shell && severity > shell->loglevel)
	{
		return;
	}
	va_start(ap, fmt);
	shell_diag_vprintf_internal_(shell, severity, diag, code, fmt, ap);
	va_end(ap);
}

static void
shell_diag_vprintf_internal_(SHELL *shell, SHELLSEVERITY severity, const struct shell_diag_struct *diag, SHELLDIAG code, const char *fmt, va_list ap)
{
	char severitylevel[] = { '!', '*', 'C', 'E', 'W', 'N', 'I', 'D'};
	char ch;
		
	if(shell && severity > shell->loglevel)
	{
		return;
	}
	diag = shell_diag_locate_(code);
	if((size_t) severity < sizeof(severitylevel))
	{
		ch = severitylevel[severity];
	}
	else
	{
		ch = '?';
	}
	if(diag)
	{
		fprintf(stderr, "%%%s-%c-%s: ", diag->facility, ch, diag->mnemonic);
	}
	else
	{
		fprintf(stderr, "%%UNSPEC-%c-%04d: ", ch, code);
	}
	if(shell && shell->diag_target)
	{
		fprintf(stderr, "%s: ", shell->diag_target);
	}
	vfprintf(stderr, fmt, ap);
}

static const struct shell_diag_struct *
shell_diag_locate_(SHELLDIAG code)
{
	size_t c;
	
	for(c = 0; shell_messages[c].code != DIAG_NONE; c++)
	{
		if(shell_messages[c].code == code)
		{
			return &shell_messages[c];
		}
	}
	return NULL;
}