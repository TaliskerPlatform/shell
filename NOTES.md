## Execution and scripting

### Execute a script (`-f`)

    talisker [OPTIONS] [-f] /PATH/TO/SCRIPTNAME [ARGS...]
    talisker [OPTIONS] -f SCRIPTNAME [ARGS...]

The `-f` option forces execution as a script, even if the operand contains no path components (the script will be searched for in the current directory if so).

The space between `-f` and the first operand may be omitted.

### Interactive mode (`-i`)

    talisker [OPTIONS] [-i[TYPE]] [COMMAND]

The `-i` option causes interactive mode to be entered. If there are any scripts or commands specified, these will be executed first.

The `TYPE` argument specifies the kind of interactive interpreter to use. These include:

    cli               Use the simple command-line-interface
    tui               Use the full-screen terminal-based user interface
    server            (Reserved for internal use by sub-shell invocation)

The default is the most capable interactive interface available on the current platform and terminal.

Note that `talisker -i` may imply creation of a sub-shell (using the `-T` option) if the standard input or output channels are not terminal devices. This can be prevented with the `-t` (treat input device as a terminal) option.

## Verbosity options

By default, the Talisker shell will display diagnostic messages which are of severity levels _Notice_, _Warning_,  _Error_ and _Critical error_. Messages at the levels _Debug_ (least severe) and _Info_ are normally suppressed.

This can be adjusted with the following options:

### Be more verbose (`-v`)

    talisker [-v] [OPTIONS] [COMMAND]

Display messages at the severity level `Info`, as well as those listed above.

### Be overly verbose (`-d`)

    talisker [-d] [OPTIONS] [COMMAND]

All messages will be shown. In other words, display messages at the severity levels `Debug` and `Info`, as well as those listed above.

### Be quiet (`-q`)

    talisker [-q] [OPTIONS] [COMMAND]

Do not show messages at the _Notice_ severity level, only those which are _Warning_,  _Error_, or _Critifcal Error_.

### Be extremely quiet  (`-qq`)

    talisker [-qq] [OPTIONS] [COMMAND]

Only show _Critical Error_ messages. All other messages are suppressed.

## Environment options

    talisker [-i] [VAR=VALUE...] [OPTIONS] [COMMAND]

If the `-i` option is given, the process environment is reset to defaults prior to performing any actions.

If any `VAR=VALUE` options are given, the environment variable `VAR` will be set to `VALUE` before performing any actions.

If both `-i` and `VAR=VALUE` options are given, the environment will be reset first, then any specified variables will be set to the provided values.

Variable interpolation will _not_ be performed when setting environment variables in this way. Be sure to quote arguments correctly to prevent the _parent_ shell from performing unwanted variable expansion.

Environment options are also passed to, and will execute within, any sub-shells that are executed -- that is, they will apply to the initial parent and to _all_ sub-shells.

## Sub-shell options

Some options cause the Talisker shell to re-execute itself within some alternative context—for example, on a remote host or with elevated privileges.

**Note: Sub-shell options are executed in the order that they are provided on the command-line.**

For example, if you specify '-r' to connect to a remote host, then '-T' to open a terminal, the shell will attempt to open the terminal _on the remote host_. 

It's possible that this will display a terminal on your local host, depending upon whether the remote terminal uses X11 and forwarding is configured, or it may fail with an error, or it may appear to do nothing at all.

### Alternative privileges ('-p')

    talisker -p[USER] [OPTIONS] [COMMAND]

Attempt to assume the privileges of `USER` (`root` if not provided), and execute a new Talisker shell within that context.

### Terminal (`-T`)

    talisker -T [OPTIONS] [COMMAND]

Execute the Talisker shell in a new terminal window. May be combined with -r or -c options.

### Remote (`-r`)

    talisker -r [USER@]HOST [COMMAND]

Execute the Talisker shell on a remote host {via SSH}. The `HOST` may match the name of a _configured remote_, which may specify further configuration for the connection (e.g., using something other than SSH; providing a default `USER` which differs from `$LOGNAME`)

### Container ('-C')

    talisker -C[IMAGE] -P[CONTAINER-ID] [COMMAND]

Execute the Talisker shell in a container {via Docker}, possibly sharing PID space with existing container `CONTAINER-ID`.

If `IMAGE` is not provided, `taliskerplatform/shell:latest` is used instead.
