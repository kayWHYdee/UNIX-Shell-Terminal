#ifndef BG_H
#define BG_H

#define BG_MAX 100

typedef struct bg_process {
    pid_t pid;
    char command[256];
    char state[10];
}bg_process;

typedef struct bg_process_queue {
    struct bg_process processes[BG_MAX];
    int front;
    int rear;
    int size;
}bg_process_queue;

extern struct bg_process_queue bg_queue;
void execute_background_process(char* command);
void init_bg_queue();
int is_bg_queue_empty();
int is_bg_queue_full();
void enqueue_process(pid_t pid, const char* command,char *state);
struct bg_process dequeue_process();
void check_background_processes();

#endif