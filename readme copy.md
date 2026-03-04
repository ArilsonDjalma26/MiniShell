*This project has been created as part of the 42 curriculum by elfranco, aalbano.*

# Description

This project is a custom implementation of a simple shell, called **minishell**, developed as part of the 42 curriculum. The goal is to create a functional shell that can interpret and execute user commands, handle built-in commands, manage environment variables, and support features such as piping and redirections, mimicking the behavior of GNU Bash.

# Instructions

## Compilation
To compile the project, run:
```
make
```
This will generate the `minishell` executable in the project directory.

## Execution
To start the shell, run:
```
./minishell
```

# Resources
- [GNU Bash Manual](https://www.gnu.org/software/bash/manual/bash.html)
- [UNIX Shell Programming](https://www.tutorialspoint.com/unix/unix-shell.htm)
- [42 Subject PDF](https://cdn.intra.42.fr/pdf/pdf/198638/en.subject.pdf)
- [man bash](https://man7.org/linux/man-pages/man1/bash.1.html)


# Authors
- elfranco
- aalbano

# Additional Sections

## Features
- Command parsing and execution
- Built-in commands: echo, env, pwd, etc.
- Support for pipes and redirections
- Environment variable management
- Error handling and user-friendly messages

## Example Usage
```
$ ./minishell
minishell$ echo Hello World
Hello World
minishell$ pwd
/home/user/minishell
minishell$ exit
```