## Talisker: Administrator's Shell

## Copyright 2017 Mo McRoberts.
##
##  Licensed under the Apache License, Version 2.0 (the "License");
##  you may not use this file except in compliance with the License.
##  You may obtain a copy of the License at
##
##      http://www.apache.org/licenses/LICENSE-2.0
##
##  Unless required by applicable law or agreed to in writing, software
##  distributed under the License is distributed on an "AS IS" BASIS,
##  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
##  See the License for the specific language governing permissions and
##  limitations under the License.


#############################################################################
## Initialise Autoconf and Automake with project-specific information      ##
#############################################################################

AC_INIT([talisker-shell],[develop])
AS_VAR_SET([DESC], ["Talisker Administrator's Shell"])
AC_CONFIG_AUX_DIR([BuildScripts])
AC_CONFIG_MACRO_DIR([M4])
TAL_INIT
AM_INIT_AUTOMAKE([foreign subdir-objects])
AM_MAINTAINER_MODE
AC_CONFIG_HEADER([config.h])

#############################################################################
## Check for basic system utilities and build tools (e.g., compilers)      ##
#############################################################################

AC_PROG_LN_S
AC_PROG_CC([clang c99 g++ cc])

LT_INIT

if test x"$GCC" = x"yes" ; then
	AS_VAR_APPEND([AM_CFLAGS], ['-W -Wall '])
	AS_VAR_APPEND([AM_CFLAGS], ['-fvisibility=hidden '])
	AS_VAR_APPEND([AM_CFLAGS], ['-O0 '])
	AS_VAR_APPEND([AM_CFLAGS], ['-g '])
fi

AC_SUBST([LIBS])
AC_SUBST([AM_CPPFLAGS])
AC_SUBST([AM_CFLAGS])
AC_SUBST([AM_LDFLAGS])

AS_VAR_COPY([save_prefix], [prefix])
AS_VAR_COPY([save_exec_prefix], [exec_prefix])
AS_VAR_COPY([save_libexecdir], [libexecdir])

if test x"$prefix" = x"" || test x"$prefix" = x"NONE" ; then
	prefix=${ac_default_prefix}
fi
if test x"$exec_prefix" = x"" || test x"$exec_prefix" = x"NONE" ; then
	exec_prefix='${prefix}'
fi
exec_prefix="`eval echo $exec_prefix`"
libexecdir="`eval echo $libexecdir`"
utilsdir="${libexecdir}/${PACKAGE}"
AC_SUBST([utilsdir])
AC_DEFINE_UNQUOTED([UTILSDIR], ["`eval echo $utilsdir`"], [Define to the path of bundled utilities])

AS_VAR_COPY([prefix], [save_prefix])
AS_VAR_COPY([exec_prefix], [save_exec_prefix])
AS_VAR_COPY([libexecdir], [save_libexecdir])

#############################################################################
## Project-specific compilation environment tests                          ##
#############################################################################

AC_CHECK_HEADERS([unistd.h spawn.h strings.h sys/wait.h sys/stat.h])

AC_CHECK_FUNCS([strrchr])

#############################################################################
## Output files                                                            ##
#############################################################################

AC_CONFIG_FILES([
Makefile
Sources/Makefile
Sources/Portability/Makefile
Sources/Wrapper/Makefile
Sources/Utilities/Makefile
Sources/Utilities/help/Makefile
Sources/Utilities/pwd/Makefile
])

AC_OUTPUT
