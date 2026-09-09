# procview

`procview` is a Linux utility that displays process information using `/proc`.

## Requirements

- Linux
- GCC or Clang
- GNU Make

## Build and run

```sh
make
make debug
make release
make clean
```

Usage:

```sh
./bin/procview <pid>
./bin/procview $$
./bin/procview 1
```

The program reads `/proc/<pid>/status`, `/proc/<pid>/cmdline`, and
`/proc/<pid>/fd`. Information may become unavailable if the process exits while
it is being read. Access can also be restricted by the operating system. An
empty command line is displayed as `[not available]`.

Exit codes:

- `0` — success;
- `1` — error while reading process information;
- `2` — invalid command-line arguments.
