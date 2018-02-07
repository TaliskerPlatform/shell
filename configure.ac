## Talisker: Administrator's Shell

## Copyright 2017-2017 Mo McRoberts.
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

#############################################################################
## Output files                                                            ##
#############################################################################

AC_CONFIG_FILES([
Makefile
Sources/Makefile
Sources/Wrapper/Makefile
])

AC_OUTPUT
