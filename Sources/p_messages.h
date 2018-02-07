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

/* This is not a general-purpose header and is expected to be included
 * mutiple times.
 */
DEFDIAG(SHELL, INIT, "initialisation failed"),
DEFDIAG(SHELL, USAGE, "Usage"),
DEFDIAG(SHELL, NOTFOUND, "command not found"),
DEFDIAG(SHELL, NOEXEC, "command cannot be executed"),
DEFDIAG(SHELL, SCRIPTEXEC, "script cannot be executed"),
DEFDIAG(SHELL, CHILDWAIT, "unable to wait for child process to finish"),
DEFDIAG(SHELL, CHILDSIG, "child process terminated due to a signal"),
DEFDIAG(SHELL, CHILDSIGC, "child process terminated due to a signal (core was dumped)"),
DEFDIAG(SHELL, CHILDERR, "child process exited with an error status"),
