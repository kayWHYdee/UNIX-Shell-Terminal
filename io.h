#ifndef IO_H
#define IO_H
void redirection_handler(char *command);
void edgecasehandler(char *command);
int is_custom_command(char *command);
extern int input_fd ;    // File descriptor for input redirection
extern int output_fd;
#endif 