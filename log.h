#ifndef LOG_H
#define LOG_H


extern char *LOG_FILE;

// Function declarations
void init_log();
void update_log( char* command);
void print_log();
void log_purge();
void log_execute(int index);



#endif
