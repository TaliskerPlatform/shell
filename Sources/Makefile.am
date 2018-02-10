## Talisker: Administrator's Shell

## Copyright 2017 Mo McRoberts.
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

SUBDIRS = Portability . Wrapper Utilities

bin_PROGRAMS = talisker

talisker_SOURCES = p_shell.h p_messages.h \
	main.c \
	args.c builtins.c conio.c context.c diagnostics.c glob.c interactive.c \
	parse.c progname.c prompt.c script.c spawn.c subshell.c usage.c vars.c \
	wrapper.c
