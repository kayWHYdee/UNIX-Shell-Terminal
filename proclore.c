#include "headers.h"

void proc_command(char *command)
{
    char proc_path[1024];
    char buffer[1024];
    char *token, *saveptr;
    int pid;
    token = strtok_r(command, " \t\n",&saveptr);
    token = strtok_r(NULL, " \t\n",&saveptr);
    // If no argument is provided, use the current process ID
    if (token == NULL) {
        pid = SHELL_PID;
    } else {
        pid = atoi(token);
    }

    snprintf(proc_path, sizeof(proc_path), "/proc/%d/stat", pid);
    FILE *stat_file = fopen(proc_path, "r");
    if (stat_file == NULL) {
        perror("Error opening stat file");
        return;
    }
    
    if (fgets(buffer, sizeof(buffer), stat_file) == NULL) {
        perror("Error reading stat file");
        fclose(stat_file);
        return;
    }
    fclose(stat_file);
    saveptr = NULL;
    // Tokenize the stat file content
    strtok_r(buffer, " ", &saveptr);  // pid
    strtok_r(NULL, " ", &saveptr);    // comm
    char *status = strtok_r(NULL, " ", &saveptr);
    strtok_r(NULL, " ", &saveptr);    // ppid
    char *pgrp = strtok_r(NULL, " ", &saveptr);

    // Get virtual memory size
    unsigned long vm_size = 0;
    for (int i = 0; i < 20; i++) {
        token = strtok_r(NULL, " ", &saveptr);
    }
    vm_size = strtoul(token, NULL, 10);

    // Get executable path
    snprintf(proc_path, sizeof(proc_path), "/proc/%d/exe", pid);
    char exec_path[DIR_SIZE];
    ssize_t len = readlink(proc_path, exec_path, sizeof(exec_path) - 1);
    if (len != -1) {
        exec_path[len] = '\0';
    } else {
        strcpy(exec_path, "Unknown");
    }

    // Determine if it's a foreground process
    char fg_char = ' ';
    int tpgid;
    snprintf(proc_path, sizeof(proc_path), "/proc/%d/stat", pid);
    stat_file = fopen(proc_path, "r");
    if (stat_file != NULL) {
        if (fscanf(stat_file, "%*d %*s %*c %*d %*d %*d %*d %d", &tpgid) == 1) {
            if (tpgid == atoi(pgrp)) {
                fg_char = '+';
            }
        }
        fclose(stat_file);
    }
    // Print the information
    printf("pid : %d\n", pid);
    printf("process status : %s%c\n", status, fg_char);
    printf("Process Group : %s\n", pgrp);
    printf("Virtual memory : %lu\n", vm_size);
    printf("executable path : %s\n", exec_path);
}