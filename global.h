#ifndef GLOBAL_H
#define GLOBAL_H

#define MAX_LENGTH 4096
#define DIR_SIZE 4096
#define LOG_SIZE 15
extern char *HOME_DIR;
extern char *PREV_DIR;
extern char *CURR_DIR;
extern char* USERNAME;
extern pid_t SHELL_PID;
char *trim(char *str);

extern int input_fd;
extern int output_fd;
extern struct ForegroundProcess* FG_PROC;
extern struct TimeInfo* TIME;
void init_bg_queue() ;
typedef struct {
    char commands[LOG_SIZE][DIR_SIZE]; 
    int start;                     
    int count;
} LogQueue;
extern LogQueue g_log;
#define BLUE "\033[0;34m"
#define GREEN "\033[0;32m"
#define WHITE "\033[0;37m"
#define RESET "\033[0m"
#endif