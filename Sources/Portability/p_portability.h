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

#ifndef P_PORTABILITY_H_
# define P_PORTABILITY_H_              1

/* We primarily target UNIX08 systems */
# define _POSIX_C_SOURCE               200809L

# if defined(__linux__)
/* On Linux, request visibility of extended, but non-portable APIs */
#  define _GNU_SOURCE                  1
# endif
# if defined(__APPLE__)
/* On Darwin (macOS, iOS, tvOS, etc.), request visibility of NP APIs */
#  define _DARWIN_C_SOURCE             1
# endif

# include <stdio.h>
# include <stdlib.h>
# include <stdarg.h>
# include <string.h>
# include <errno.h>
# include <limits.h>

# ifdef HAVE_UNISTD_H
#  include <unistd.h>
# endif

# ifdef HAVE_SPAWN_H
#  include <spawn.h>
# endif

# ifdef HAVE_SYS_WAIT_H
#  include <sys/wait.h>
# endif

# ifdef HAVE_SYS_STAT_H
#  include <sys/stat.h>
# endif

#endif /*!P_SHELL_H_*/
