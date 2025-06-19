#ifndef INPUT_H
#define INPUT_H

// #include "headers.h"  
// void handle_input();
// void parse_command(char* command);
// void tokenize(char * input, int gg);
// void execute_custom_command_with_redirection(char *command, char *input_file, char *output_file, int append);
// void execute_piped_commands(char *command);
// void handle_redirection(char **args, char *input_file, char *output_file, int append_mode);
// int is_custom_command(char *args[]);
// char* join_args(char** args);
void handle_input();
void tokenize(char *input, int gg);
void parse_command(char *command);
void execute_without_io(char *command);
void execute_with_io(char *command);
void handle_redirection(char **args, char *input_file, char *output_file, int append_mode);
void handle_pipe(char *command);
char *join_args(char **args);
#endif  
