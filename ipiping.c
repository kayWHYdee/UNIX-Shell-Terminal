#include "headers.h"
void handle_custom_command_with_file(char *command, char *input_file)
{
    char input_content[MAX_LENGTH] = "";
    ssize_t bytes_read;

    // If input_file is NULL, read from stdin (which is the previous pipe's output)
    if (input_file == NULL)
    {
        bytes_read = read(STDIN_FILENO, input_content, MAX_LENGTH - 1);
        if (bytes_read == -1)
        {
            perror("Error reading from pipe");
            return;
        }
        input_content[bytes_read] = '\0'; // Null-terminate the string
    }
    else
    {
        // Open the input file and read its contents
        int fd = open(input_file, O_RDONLY);
        if (fd == -1)
        {
            perror("Error opening input file");
            return;
        }

        bytes_read = read(fd, input_content, MAX_LENGTH - 1);
        if (bytes_read == -1)
        {
            perror("Error reading input file");
            close(fd);
            return;
        }
        input_content[bytes_read] = '\0'; // Null-terminate the string
        close(fd);
    }

    // Create a new command string combining the original command and input content
    char new_command[MAX_LENGTH * 2]; // Double the size to accommodate both command and input
    snprintf(new_command, sizeof(new_command), "%s %s", command, input_content);

    parse_command(new_command);
}
void pipe_handler(char *command)
{
    char *commands[256]; // Array to store each command in the pipe
    int num_commands = 0;
    char *token;
    char *saveptr;

    token = strtok_r(command, "|", &saveptr);
    while (token != NULL && num_commands < 256)
    {
        commands[num_commands++] = trim(strdup(token)); // Store the trimmed command
        token = strtok_r(NULL, "|", &saveptr);
    }

    if (num_commands < 2)
    {
        fprintf(stderr, "Invalid use of pipe\n");
        return;
    }

    // int prev_pipe[2] = {0, 1};  // Initialize with stdin and stdout
    // int curr_pipe[2];
    // pid_t pid;

    // for (int i = 0; i < num_commands; i++)
    // {
    //     if (i < num_commands - 1) {
    //         if (pipe(curr_pipe) == -1)
    //         {
    //             perror("Pipe creation failed");
    //             return;
    //         }
    //     }

    //     pid = fork();
    //     if (pid == -1) {
    //         perror("Fork failed");
    //         return;
    //     }

    //     if (pid == 0) {
    //         // Child process
    //         if (i > 0)
    //         {
    //             // Not the first command, read from previous pipe
    //             dup2(prev_pipe[0], STDIN_FILENO);
    //             close(prev_pipe[0]);
    //             close(prev_pipe[1]);
    //         }

    //         if (i < num_commands - 1) {
    //             // Not the last command, write to current pipe
    //             dup2(curr_pipe[1], STDOUT_FILENO);
    //             close(curr_pipe[0]);
    //             close(curr_pipe[1]);
    //         }

    //         if (is_custom_command(commands[i]))
    //         {
    //             handle_custom_command_with_file(commands[i], NULL);
    //         }
    //         else
    //         {
    //             parse_command(commands[i]);
    //         }

    //         exit(EXIT_SUCCESS);
    //     } else {
    //         // Parent process
    //         if (i > 0) {
    //             close(prev_pipe[0]);
    //             close(prev_pipe[1]);
    //         }
    //         if (i < num_commands - 1) {
    //             prev_pipe[0] = curr_pipe[0];
    //             prev_pipe[1] = curr_pipe[1];
    //         }
    //     }
    // }
    int fds[2][2];  // To manage two pipes in alternating fashion
    int flag = 0;   // To manage cases where command is empty or invalid

    for (int i = 0; i < num_commands; i++) {  // For each command
        // Create the next pipe if it's not the last command
        if (i < num_commands - 1) {
            if (pipe(fds[i % 2]) == -1) {
                printf("\033[1;31mERROR: Pipe call not accessed\n");
                printf("\033[0m");
                return;
            }
        }

        int input_fd = STDIN_FILENO;   
        int output_fd = STDOUT_FILENO; // Default output to stdout
        char* final_pi = trim(commands[i]);  
        // printf("%sch\n", final_pi);

        // Set up piping between commands
        if (i > 0) {
            input_fd = fds[!(i % 2)][0];  // Read from previous pipe
            if (flag == 1) {
                close(fds[!(i % 2)][1]);  // Close write end of previous pipe if needed
            }
        }
        if (i < num_commands - 1) {
            output_fd = fds[i % 2][1];    // Write to current pipe
        }

        // Skip invalid commands
        if (final_pi == NULL) {
            flag = 1;
            continue;
        }

        // Save current stdin and stdout
        int restore_input = dup(STDIN_FILENO); 
        int restore_output = dup(STDOUT_FILENO);

        // Redirect input/output for current command
        dup2(input_fd, STDIN_FILENO);    // Redirect input to stdin
        dup2(output_fd, STDOUT_FILENO);  // Redirect output to stdout

        parse_command(final_pi);  // Execute the command

        // Restore the original stdin/stdout
        dup2(restore_output, STDOUT_FILENO);
        dup2(restore_input, STDIN_FILENO);

        // Close any custom input/output descriptors
        if (input_fd != STDIN_FILENO) {
            close(input_fd);
        }
        if (output_fd != STDOUT_FILENO) {
            close(output_fd);
        }

        // Close duplicated fds to avoid file descriptor leakage
        close(restore_input);
        close(restore_output);

        // Free the processed command string
        // free(final_pi);
    }

    // Parent waits for all child processes to finish
    for (int i = 0; i < num_commands; i++)
    {
        wait(NULL);
    }

    // // Free allocated memory
    // for (int i = 0; i < num_commands; i++) {
    //     free(commands[i]);
    // }
}