#ifndef BASHCOMM_H
#define BASHCOMM_H
typedef struct ForegroundProcess {
    pid_t pid;           // Process ID of the foreground process
    pid_t ppid;          // Parent process ID (usually the shell's PID)
    char* command;       // The command being executed
    time_t start_time;   // Start time of the process
    int status;          // Exit status of the process
}ForegroundProcess;

struct TimeInfo {
    char* command;       // Name of the command
    int execution_time;  // Execution time in seconds
};

void bash_command(char * command);

#endif
