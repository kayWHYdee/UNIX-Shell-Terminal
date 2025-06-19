#include "headers.h"
// cd is not a std argument
void bash_command(char *command)
{
    char *command_s = malloc(sizeof(char)*DIR_SIZE);
    strcpy(command_s,command);
    for (int i = 0; i < DIR_SIZE; i++)
    {
        FG_PROC->command[i] = '\0';
    }

    TIME = malloc(sizeof(struct TimeInfo));
    TIME->command = calloc(DIR_SIZE, sizeof(char));
    // Foreground process
    pid_t id = fork();
    if (id == 0)
    { // Child process
        
        // char *args[] = {"/bin/bash", "-c", command, NULL};
         char *args[100] = {NULL};
    int arg_count = 0;

    char *token = strtok(command, " ");
    while (token != NULL && arg_count < 99) {
        args[arg_count] = token;
        arg_count++;
        token = strtok(NULL, " ");
    }
        // char *args[] = {command[0], command, NULL};
        // printf
        // printf("child is complete\n");
        if (execvp(args[0], args) == -1)
        {
            perror("Command execution failed");
            exit(1);
        }
    }
    else if (id > 0)
    { // Parent process
        FG_PROC->pid = id;
        FG_PROC->ppid = getpid();
        strcpy(FG_PROC->command , command);
        // FG_PROC->command = strdup(command);
        FG_PROC->start_time = time(NULL);

        waitpid(id, &(FG_PROC->status), WUNTRACED);

        time_t end_time = time(NULL);
        int exec_time = (int)difftime(end_time, FG_PROC->start_time);

        if (exec_time > 2)
        {
            // Store the command name (first word of the command)
            char *cmd_name = strdup(command);
            TIME->command = cmd_name;
            TIME->execution_time = exec_time;
            construct_prompt(1);
        }
        FG_PROC->pid = SHELL_PID;
        free(TIME->command);
        free(TIME);
        free(command_s);
    }
    return;
}