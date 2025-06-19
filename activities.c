#include "headers.h"

// changing the implementation to proclore

void get_process_state(pid_t pid, char* state) 
{
    char stat_file[64];
    sprintf(stat_file, "/proc/%d/stat", pid);

    FILE* file = fopen(stat_file, "r");
    if (!file) {
        strcpy(state, "Unknown");
        return;
    }

    int pid_val;
    char comm[256], state_char;

    // Read the content of /proc/[pid]/stat
    fscanf(file, "%d %s %c", &pid_val, comm, &state_char);
    fclose(file);

    if (state_char == 'T') {
        strcpy(state, "Stopped");
    } else {
        // printf("%c\n",state_char);
        strcpy(state, "Running");
    }
}

int compare_bg_processes(const void* a, const void* b) 
{
    return strcmp(((const struct bg_process*)a)->command, ((const struct bg_process*)b)->command);
}

void activities_command() {
    // printf("coming here\n");
    if (is_bg_queue_empty()) {
        printf("No other processes is running.\n");
        return;
    }

    bg_process temp_queue[BG_MAX];
    for (int i = 0; i < bg_queue.size; i++) 
    {
        temp_queue[i] = bg_queue.processes[(bg_queue.front + i) % BG_MAX];
    }
    qsort(temp_queue,bg_queue.size,sizeof(bg_process),compare_bg_processes);
    print_activities(temp_queue);
}

void print_activities(bg_process temp_queue[])
{
    for (int i = 0; i < bg_queue.size; i++) 
    {
        char state[10];
        get_process_state(temp_queue[i].pid, state);  // Get the process state

        printf("[%d] : %s - %s\n", temp_queue[i].pid, temp_queue[i].command, state);
        
        /*
        not working coz there is some problem with wnhohand and in proc gfile it is coming as T+ for some reason, 
        int status;
        int result = waitpid(temp_queue[i].pid, &status, WNOHANG);
        char state[10] = "Stopped";
        if (result == 0)
        {
            strcpy(state , "Running");
        }
        printf("%d : %s - %s\n", temp_queue[i].pid, temp_queue[i].command, state);
        bg_process temp = bg_queue.processes[(bg_queue.front + i) % BG_MAX];
        printf("[%d] : %s - %s\n", temp.pid, temp.command, temp.state);*/
        
    }
}