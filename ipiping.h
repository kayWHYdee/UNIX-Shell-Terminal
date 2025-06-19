#ifndef IPIPING_H
#define IPIPING_H
// extern input_fd;
// extern output_fd;
void pipe_handler(char *command) ;
// void handle_custom_command_with_file(char *command);
void handle_custom_command_with_file(char *command, char *input_file);
#endif