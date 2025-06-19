# Description

# Linux Shell Replica

This project is a comprehensive replication of a Unix-like shell, also known as a command-line interface or terminal. It closely emulates the behavior and appearance of traditional shells while extending their capabilities with a range of features.

## Assumptions
- When parsing commands, if multiple commands are separated by & within the same sequence (i.e., within a sequence separated by ; or newline), then either all of those commands must be executed as background processes, or none of them should. Mixed execution, where some commands are run in the background and others in the foreground within the same sequence, is not allowed.
- All commands executed through the log command will run in the foreground only.
- The reveal command will accept only one argument after the flags.
- Instead of 'hop ~/new' 'hop ~ new' works

## Prompt

## User-Implemented Commands

### Hop

The "hop" command is an enhanced version of the `cd` command in bash, offering support for various flags and both absolute and relative paths. Supported flags include:

- `~`: Navigate to the user's home directory.
- `..`: Move up one directory level.
- `.`: Stay in the current directory.
- `-`: Return to the previous directory.
- If no argument then Home directory

For ease of use, the implementation also prints the changed directory path.

### Reveal

The `reveal` command lists all the files and directories in the specified directory/directories in lexicographic order. By default, it does not display hidden files. The `reveal` command supports the following flags:

 - `-l`: Displays extra information about files and directories.
 - `-a`: Displays all files, including hidden files.
 - It also follows combination of these flags as directed.

### Seek

The `seek` command is used to search for a file or directory in the specified target directory or the current working directory if no directory is specified. It returns a list of relative paths from the target directory for all matching files (in green) and directories (in blue), separated by newline characters.

- `-d`: Only looks for directories and ignores files, even if their names match.
- `-f`: Only looks for files and ignores directories, even if their names match.
- `-e`: This flag is effective when a single file or a single directory with the specified name is found.
    -If only one file (and no directories) is found, it prints the output.
    -If only one directory (and no files) is found, it changes the current working directory to it.
    -Otherwise, the flag has no effect.
   The -e flag works in conjunction with both the -d and -f flags.


The `reveal` and `seek command supports the following symbols for specifying directories:

  - `.`: Represents the current directory.
  - `..`: Represents the parent directory.
  - `~`: Represents the user's home directory.

### Log

The `log` command provides a historical view of previous shell commands and their execution status. Users can review past commands, their outcomes, and any error messages, helping them track and troubleshoot their interactions with the shell.

- Flags and Arguments for `log`:
  - `no argument`: Prints the last 15 commands ran by user.
  - `purge`: Clears the history.
  - `execute <index>`: Execute the command at position in log as indicated

### Foreground and Background Processes

This shell supports running processes both in the foreground and background. For example:

- Running a command like `sleep 5` in the foreground makes the shell wait for its completion.
- Running a command with `&` at the end makes it a background process, allowing you to continue entering commands.

### Process and Job Management

- `proclore`: Prints process-related information for the shell program.
- `activities`: Lists all currently running jobs, particularly background jobs, in the order of their creation.
- `ping <pid> <signalNumber>`: Sends a signal to a running job based on its job number.
- `fg <pid>`: Brings a background job to the foreground.
- `bg <pid>`: Starts running a background process corresponding to the given job number.
- `CTRL+Z`: Changes the status of the currently running job to stop and pushes it to the background.
- `CTRL+D`: Exits the shell.
- `CTRL+C`: Ends the process running in the foreground


### Output and Input Redirection

The shell allows you to redirect the output of commands to files and read input data from files. Error handling is in place, including checks for non-existent input files. Examples include:

- Output redirection: `diff file1.txt file2.txt > output.txt`
- Input redirection: `sort < lines.txt`
- Input-Output redirection: `sort < lines.txt > sorted-lines.txt`

### Pipelines

You can use the `|` symbol to create pipelines, enabling the chaining of multiple commands. The shell supports any number of pipes:

- Example with two commands: `more file.txt | wc`
- Example with three commands: `cat somefile.txt | wc`

### iMan

The `iMan` command is a helpful addition to this shell, allowing users to access manual pages for various commands and utilities. It provides detailed information on command usage, options, and examples, helping users navigate the shell efficiently.

### neonate (bonus)

The `neonate` command is a powerful feature that simplifies repetitive tasks by allowing users to create and execute custom scripts within the shell. It enhances the shell's scripting capabilities and empowers users to automate complex workflows.

### myshrc
Each of these aliases maps a single word to a command, as per your requirements. They don't involve any custom functions and are not multi-word aliases.
- alias reveall='reveal -l'

## Usage

To run the shell, follow these steps:

1. Clone the repository.
2. Run `make` in the cloned directory to compile the code.
3. Execute `./a.out` to start the shell.
