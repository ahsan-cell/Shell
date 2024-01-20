#  Simple Shell
This project is a simple shell implemented in C++ as part of an Operating Systems course. The shell provides basic functionality for executing commands, handling pipes, and supporting input/output redirection.

## Features
- Command execution
- Pipe support for multiple commands
- Input and output redirection

## Supported features
- Single commands
- Pipelines (|) for multiple commands
- Output redirection (>)
- Input redirection (<)
## Code Structure
- tokenize function:
Splits the input string into an array of tokens based on a specified delimiter.

- isEqual function:
Compares two strings for equality.

- Execute_Command function:
Handles the execution of commands, including handling pipes and redirection.

- n_piped_commands function:
Determines the number of piped commands in the input.

- redirection function:
Checks for input and output redirection.

- main function:
Implements the main loop of the shell, accepting user input and executing commands accordingly.
