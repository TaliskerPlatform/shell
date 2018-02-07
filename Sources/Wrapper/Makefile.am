## Talisker: Administrator's Shell

## Copyright 2012-2017 Mo McRoberts.
##
##  Licensed under the Apache License, Version 2.0 (the "License");
##  you may not use this file except in compliance with the License.
##  You may obtain a copy of the License at
##
##      http://www.apache.crg/licenses/LICENSE-2.0
##
##  Unless required by applicable law or agreed to in writing, software
##  distributed under the License is distributed on an "AS IS" BASIS,
##  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
##  See the License for the specific language governing permissions and
##  limitations under the License.

noinst_PROGRAMS = wrapper

wrapper_SOURCES = ../p_shell.h ../p_messages.h \
	../main.c \
	../context.c ../diagnostics.c ../progname.c ../spawn.c ../usage.c \
	../wrapper.c

wrapper_CPPFLAGS = @AM_CPPFLAGS@ -DSHELL_WRAPPER=1
