#include "headers.h"
// extern
extern ForegroundProcess *FG_PROC;
void setup_signal_handlers()
{
    // Handle Ctrl-C (SIGINT)
    signal(SIGINT, handle_sigint);

    // Handle Ctrl-Z (SIGTSTP)
    signal(SIGTSTP, handle_sigtstp);
    return;
}

void handle_sigint(int sig)
{

    pid_t fg_pid;
    if (FG_PROC->pid == SHELL_PID)
    {
        fg_pid = 0;
    }
    else
        fg_pid = FG_PROC->pid;

    if (fg_pid > 0)
    {
        kill(fg_pid, SIGINT);
        printf("Process %d interrupted by Ctrl-C\n", fg_pid);
        fg_pid = -1; // Reset foreground process tracking
    }
    else
    {
        printf("No foreground process to interrupt.\n");
    }
    return;
}

void handle_sigtstp(int sig)
{
    pid_t fg_pid;
    if (FG_PROC->pid == SHELL_PID)
    {
        fg_pid = 0;
    }
    else
    {
        fg_pid = FG_PROC->pid;
    }

    if (fg_pid > 0)
    {
        int a = kill(fg_pid, SIGSTOP);
        if (a < 0)
        {
            perror("kill didn't work");
        }
        else
        {
            printf("Process %d stopped by Ctrl-Z\n", fg_pid);
            
            enqueue_process(fg_pid, FG_PROC->command, "Stopped");
            // printf("erorororororor\n");
            FG_PROC->pid = SHELL_PID;
        }
    }
    else
    {
        printf("No foreground process to stop.\n");
    }
}

// Ctrl-D handler
void handle_ctrl_d()
{

    while (!is_bg_queue_empty())
    {
        bg_process bg = dequeue_process();
        kill(bg.pid, SIGKILL); 
    }
    printf("Namaste!\n");
    exit(0);
}
