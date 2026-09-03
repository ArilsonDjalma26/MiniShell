*This project has been created as part of the 42 curriculum by aalbano, elfranco.*

# Minishell

## Description

Minishell is a simplified Unix shell written in C, developed as part of the 42 school curriculum. The goal of this project is to implement a functional command-line interpreter that replicates core behaviors of **GNU Bash**, providing a deeper understanding of how shells work under the hood — including process creation, file descriptor management, signal handling, and environment variable expansion.

The shell reads user input via an interactive prompt, tokenizes and parses the command line, expands variables and quotes, and then executes the resulting commands — either as built-in operations or by forking child processes with `execve`.

#### Features

- **Interactive prompt** with command history (using `readline`)
- **Lexer/Tokenizer** — splits input into tokens (words, operators, quotes, environment variables)
- **Parser** — builds a linked list of commands with arguments, redirections, and pipe chains
- **Variable expansion** — expands `$VAR`, `$?` (last exit status), and handles single/double quote semantics
- **Pipes** — supports multi-command pipelines (`cmd1 | cmd2 | cmd3`)
- **Redirections** — input (`<`), output (`>`), append (`>>`), and heredoc (`<<`)
- **Signal handling** — `Ctrl+C`, `Ctrl+\`, and `Ctrl+D` behave similarly to Bash
- **Built-in commands:**
  | Command  | Description                              |
  |----------|------------------------------------------|
  | `echo`   | Print text (supports `-n` flag)          |
  | `cd`     | Change working directory                 |
  | `pwd`    | Print current working directory          |
  | `export` | Set/display environment variables        |
  | `unset`  | Remove environment variables             |
  | `env`    | Display the current environment          |
  | `exit`   | Exit the shell with optional exit code   |

#### Architecture

The project is organized into the following modules:

```
.
├── main.c                 # Entry point and main loop
├── initializers.c         # Shell and environment initialization
├── validators.c           # Argument validation
├── minishell.h            # Header with all structs, enums, and prototypes
├── lexer/                 # Tokenization of raw input
├── parser/                # Command and redirection parsing
├── expander/              # Variable and quote expansion
├── executor/              # Command execution, pipes, and redirections
│   └── built-in/          # Built-in command implementations
├── signals/               # Signal setup and handlers
└── utils/                 # Custom utility functions (ft_split, ft_strdup, etc.)
```

## Instructions

#### Prerequisites

- A Unix-like operating system (Linux or macOS)
- `cc` (C compiler, e.g. `gcc` or `clang`)
- `make`
- `readline` library installed
  - On Debian/Ubuntu: `sudo apt install libreadline-dev`
  - On macOS (Homebrew): `brew install readline`

#### Compilation

Clone the repository and compile the project:

```sh
git clone <repository-url> minishell
cd minishell
make
```

Available Makefile rules:

| Rule      | Description                              |
|-----------|------------------------------------------|
| `make`    | Compile the project                      |
| `make clean` | Remove object files                  |
| `make fclean` | Remove object files and the binary  |
| `make re` | Full recompile (fclean + all)            |

#### Execution

```sh
./minishell
```

You will be greeted with an interactive prompt:

```
minishell$
```

#### Usage Examples

```sh
minishell$ echo "Hello, World!"
Hello, World!

minishell$ ls -la | grep minishell
-rwxr-xr-x 1 user user 42000 Mar  6 12:00 minishell

minishell$ export MY_VAR="42 Porto"
minishell$ echo $MY_VAR
42 Porto

minishell$ cat < input.txt > output.txt

minishell$ echo "Exit status: $?"
Exit status: 0

minishell$ exit
```

## Resources

#### References

- [GNU Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html) — the authoritative documentation for Bash behavior and syntax
- [bash(1) — Linux man page](https://man7.org/linux/man-pages/man1/bash.1.html) — detailed manual page for Bash
- [The Open Group Base Specifications — Shell Command Language](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html) — POSIX shell specification
- [Writing a Shell in C (Stephen Brennan)](https://brennan.io/2015/01/16/write-a-shell-in-c/) — introductory tutorial on building a Unix shell from scratch
- [Pipes, Forks, & Dups: Understanding Command Execution and I/O](https://www.rozmichelle.com/pipes-forks-dups/) — visual guide to pipes and file descriptors
- [readline(3) — GNU Readline Library](https://man7.org/linux/man-pages/man3/readline.3.html) — documentation for the readline library used for the interactive prompt

#### AI Usage

AI tools (GitHub Copilot) were used during the development of this project in the following capacity:

- **Debugging assistance** — identifying memory leaks, incorrect file descriptor handling, and edge cases in signal management.
- **Code review and refactoring** — suggesting improvements for code structure, naming conventions, and modularization.
- **Documentation** — assistance in writing and formatting this README file.