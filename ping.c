#include "headers.h"
// error handling not done
void ping_command(char *command)
{
    // char *command_s = strdup(command);
    char *token;
    char *input_ptr = NULL;
    token = strtok_r(command, " \t\n", &input_ptr);
    pid_t pid = atoi(strtok_r(NULL, " \t\n", &input_ptr));
    int sig_num = atoi(strtok_r(NULL, " \t\n", &input_ptr));

    int found = 0;
    for (int j = 0; j < bg_queue.size; j++)
    {
        if (bg_queue.processes[(bg_queue.front + j) % BG_MAX].pid == pid)
        {
            found = 1;
            break;
            if (sig_num==19)
            {
                strcpy(bg_queue.processes[(bg_queue.front + j) % BG_MAX].state,"Stopped");
            }
            
        }
    }

    if (!found)
    {
        printf("No process found with pid %d\n", pid);
        return;
    }

    // Send the signal
    if (kill(pid, sig_num) == 0)
    {
        printf("Sent signal %d to process with pid %d\n", sig_num, pid);        
    }
    else
    {
        perror("Error sending signal");
    }
}