#include "headers.h"
// do check the queue full conition coz we are defining a mx size for queue and if exceeds that

bg_process_queue bg_queue;
void init_bg_queue()
{
    bg_queue.front = -1;
    bg_queue.rear = -1;
    bg_queue.size = 0;
}
void execute_background_process(char *command)
{
    // printf("i am here exec back\n");
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return;
    }
    else if (pid == 0)
    {
        // Child process
        char *args[] = {"/bin/bash", "-c", command, NULL};
        execvp(args[0], args);
        perror("execvp");
        exit(1);
    }
    else
    {
        // Parent process
        int status;
        printf("%d\n", pid);
        enqueue_process(pid, command, "Running");
        // waitpid(pid, &status, WUNTRACED);
    }
}

void check_background_processes()
{
    // printf("cheking background \n");
    pid_t pid;
    if (is_bg_queue_empty())
        return;

    int size = bg_queue.size;
    for (int i = 0; i < size; i++)
    {
        bg_process temp = dequeue_process();

        int status;
        int result = waitpid(temp.pid, &status, WNOHANG);
        if (result == -1)
        {
            // perror("waitpid");
        }
        else if (result == 0)
        {
            // might have to edit the status
            enqueue_process(temp.pid, temp.command, temp.state);
        }
        else
        {
            if (WIFEXITED(status))
            {
                printf("%s exited normally (%d)\n", temp.command, temp.pid);
            }
            else
            {
                printf("%s exited abnormally (%d)\n", temp.command, temp.pid);
            }
        }
    }
}

int is_bg_queue_empty()
{
    return bg_queue.front == -1;
}

int is_bg_queue_full()
{
    return (bg_queue.rear + 1) % BG_MAX == bg_queue.front;
}

void enqueue_process(pid_t pid, const char *command, char *state)
{
    if (is_bg_queue_full())
    {
        printf("Too many background processes running at a time. \n");
        return;
    }
    if (is_bg_queue_empty())
    {
        bg_queue.front = bg_queue.rear = 0;
    }
    else
    {
        bg_queue.rear = (bg_queue.rear + 1) % BG_MAX;
    }
    bg_queue.processes[bg_queue.rear].pid = pid;
    strncpy(bg_queue.processes[bg_queue.rear].command, command, 255);
    bg_queue.processes[bg_queue.rear].command[255] = '\0';
    strncpy(bg_queue.processes[bg_queue.rear].state, state, 9);
    bg_queue.processes[bg_queue.rear].state[9] = '\0';
    bg_queue.size++;
}
bg_process dequeue_process()
{
    bg_process process = {0};
    if (is_bg_queue_empty())
    {
        return process;
    }
    process = bg_queue.processes[bg_queue.front];
    if (bg_queue.front == bg_queue.rear)
    {
        // empty
        bg_queue.front = -1;
        bg_queue.rear = -1;
        bg_queue.size = 0;
    }
    else
    {
        bg_queue.front = (bg_queue.front + 1) % BG_MAX;
        bg_queue.size--;
    }
    return process;
}
