#include "headers.h"
// do update the state of the command in the bg_queue

void tofg_command(char *command)
{
    // printf("jojojojojoj");
    char *token;
    char *input_ptr = NULL;
    int status;
    token = strtok_r(command, " \t\n", &input_ptr);
    pid_t id = atoi(strtok_r(NULL, " \t\n", &input_ptr));

    int flag = 0; // Set to 1 if process is found in bg_queue

    // Check if the process with given PID exists in the background queue
    for (int i = 0; i < bg_queue.size; i++)
    {
        if (bg_queue.processes[i].pid == id)
        {
            flag = 1;
            break;
        }
    }

    if (flag == 1)
    {
        FG_PROC->pid=id;
        // printf("mahiasfhidhg\n");
        pid_t pgid = getpgid(id);
        struct termios settings;
        if (tcgetattr(STDIN_FILENO, &settings) == 1)
        {
            perror("tcgetattr");
            return;
        }
        if (tcsetpgrp(STDIN_FILENO, pgid) == -1)
        {
            perror("tcsetpgrp");
            return;
        }
        kill(id, SIGCONT);
        waitpid(id, &status, WUNTRACED);

        if (tcsetpgrp(STDIN_FILENO, getpgrp()) == -1)
        {
            perror("tcsetpgrp");
            return;
        }
        if (tcsetattr(STDIN_FILENO, TCSADRAIN, &settings) == -1)
        {
            perror("tcsetattr");
            return;
        }
        FG_PROC->pid=SHELL_PID;
    }
    else
    {
        printf("No such process found\n");
    }
}
void tobg_command(char *command)
{
    char *token;
    char *input_ptr = NULL;
    token = strtok_r(command, " \t\n", &input_ptr);
    pid_t id = atoi(strtok_r(NULL, " \t\n", &input_ptr));

    printf("%d", id);
    int found = 0;

    // Search for the process in the bg_queue
    for (int i = 0; i < bg_queue.size; i++)
    {
        if (bg_queue.processes[i].pid == id)
        {
            found = 1;

            // Check if the process is in the "Stopped" state
            if (strcmp(bg_queue.processes[i].state, "Stopped") == 0)
            {
                // Send SIGCONT to the process to resume it
                if (kill(id, SIGCONT) == 0)
                {
                    printf("Process [%d] %s is now running in the background.\n", id, bg_queue.processes[i].command);

                    // Update the process state to "Running"
                    strcpy(bg_queue.processes[i].state, "Running");
                }
                else
                {
                    perror("Error sending SIGCONT");
                }
            }
            else
            {
                printf("Process [%d] %s is already running.\n", id, bg_queue.processes[i].command);
            }
            break;
        }
    }

    if (!found)
    {
        printf("No such process found with PID: %d\n", id);
    }
}
