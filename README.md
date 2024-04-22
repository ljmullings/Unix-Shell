# Lell Shell

Lell is a lightweight, Unix shell written in C. It's designed as a minimalistic command interpreter that runs in the terminal. While it aims to provide core functionalities expected from a shell, including executing commands, handling built-in commands like `cd` and `exit`, and maintaining a history of commands, Lell is intended for educational purposes and simpler use cases.

## Features

- Execute system commands
- Built-in support for `cd`, `exit`, and `history` commands
- Command history feature that stores the last 100 commands
- Command execution from history using an offset
- Simple command parsing without support for quoting or escape sequences

## Getting Started

### Prerequisites

- GCC compiler
- A Unix-like environment (Linux, macOS, BSD)

### Installation

1. Clone the repository to your local machine:

   ```sh
   git clone https://github.com/yourusername/lell.git
Navigate to the cloned directory:

cd lell
Compile the shell:

gcc lell.c -o lell
Run Lell:

./lell

## Usage
Once you start Lell, you will be greeted with a prompt lell> . You can start typing your commands here. For example:

- lell> ls -l

## Built-in Commands
Lell supports several built-in commands:

cd [directory]: Change the current working directory.
exit: Exit the shell.
history: Display the last 100 commands. Use history [offset] to execute a command from history and history -c to clear the command history.

### Contributing
Contributions are welcome! If you have ideas for improvements or have found a bug, please open an issue or submit a pull request.
