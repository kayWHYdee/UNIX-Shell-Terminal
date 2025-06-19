#include "headers.h"

// HALF FROM GPT OTHER FROM CLAUDE DO CLICK PHOTOS

int is_custom_command(char *command)
{
    int result = (strncmp(command, "hop",3) == 0) || (strncmp(command, "seek",4) == 0) || (strncmp(command, "reveal",6) == 0 || strncmp(command,"log",3)==0|| strncmp(command,"proclore",8)) ;
    // printf("%d the result\n",result);
    return result;
}

void redirection_handler(char *command)
{
    // printf("%s\n",command);
    input_fd=-1;
    output_fd=-1;
    char *input_file = NULL;
    char *output_file = NULL;
    int append_mode = 0; 
    int custom_command_flag = 0; 
    char *token;
    char *saveptr;
    int original_stdin = dup(STDIN_FILENO);
    int original_stdout = dup(STDOUT_FILENO);
    char *command_s = malloc(MAX_LENGTH*sizeof(char));
    strcpy(command_s,command);
    char *inp_command = NULL;
    char *out_command = NULL;

    // Check for input redirection '<'
    if (strchr(command, '<') != NULL) 
    {
        token = strtok_r(command, "<", &saveptr);
        if (token != NULL)
        {
            inp_command = strdup(token); // First part is the input command
            input_file = strtok_r(NULL, ">", &saveptr); // Second part is the input file
            if (input_file != NULL)
            {
                input_file = strtok(input_file, " \t\n"); // Clean whitespace
            }
        }

        // Check for output redirection '>' or '>>'
        if (saveptr != NULL)
        {
            out_command = strtok_r(NULL, " \t\n", &saveptr);  // Check for output redirection
            if (out_command != NULL)
            {
                if (out_command[0] == '>')
                {
                    output_file = strtok_r(NULL, " \t\n", &saveptr);  // Check for output redirection
                    append_mode = 1; // Append mode
                    output_file = trim(output_file);
                    // output_file = strdup(out_command + 1); // Skip the second '>'
                }
                else
                {
                    output_file = strdup(out_command);
                }
            }
        }
    } else {
        // Handle case with only output redirection
        token = strtok_r(command, ">", &saveptr);
        if (token != NULL)
        {
            inp_command = trim(strdup(token)); 
            // printf("%spppp\n",inp_command);
            output_file = strtok_r(NULL, " \t\n", &saveptr); // Output fil
            // printf("%sfdgsd\n", output_file);
            if (output_file[0] == '>')
            {
                // printf("%sfdgsd\n", output_file);
                output_file = strtok_r(NULL, ">", &saveptr); // Output file
                output_file = trim(output_file);
                // printf("%sfdgsd\n", output_file);
                append_mode = 1; // Append mode
            }
            else
            {
                output_file = trim((output_file));
            }
        }
    }

    if (output_file != NULL)
    {
        int flags = O_WRONLY | O_CREAT;
        flags |= append_mode ? O_APPEND : O_TRUNC;
        output_fd = open(output_file, flags, 0644);
        if (output_fd == -1)
        {
            perror("Error opening output file");
            return;
        }
        dup2(output_fd, STDOUT_FILENO); // Redirect output
    }
    
    // Handle input redirection
    if (input_file != NULL && inp_command != NULL)
    {
        if (is_custom_command(inp_command))
        {
            custom_command_flag = 1;

            // Open the input file and read its contents
            input_fd = open(input_file, O_RDONLY);
            if (input_fd == -1)
            {
                perror("Error opening input file");
                return;
            }

            // Read the file content
            char file_contents[MAX_LENGTH];
            int bytes_read = read(input_fd, file_contents, MAX_LENGTH - 1);
            if (bytes_read == -1)
            {
                perror("Error reading input file");
                close(input_fd);
                return;
            }
            file_contents[bytes_read] = '\0'; // Null-terminate the string
            close(input_fd);

            // Create a new command string combining the original command and file contents
            char new_command[MAX_LENGTH];
            snprintf(new_command, sizeof(new_command), "%s %s", inp_command, file_contents);
            // printf("%s\n",new_command);
            parse_command(new_command);
        }
    }
    else if (inp_command != NULL && is_custom_command(inp_command))
    {
        custom_command_flag = 1;
        // printf("comm going %s",inp_command);
        parse_command(inp_command); // Handle custom command
    }

    if (custom_command_flag == 0)
    {
        // Execute the entire command with redirection symbols using execvp
        parse_command(inp_command);
    }

    // Restore file descriptors if they were changed
    if (input_fd != -1)
    {
        close(input_fd);
        input_fd = -1;
        dup2(original_stdin, STDIN_FILENO);
    }
    if (output_fd != -1)
    {
        close(output_fd);
        output_fd = -1;
        dup2(original_stdout, STDOUT_FILENO);
    }
    close(original_stdin);
    close(original_stdout);
    free(command_s);
    if (inp_command) free(inp_command);
    // if (output_file) free(output_file);
}