#ifndef DISPLAY_H
#define DISPLAY_H

// #define MAX_LENGTH 1024

char* get_username();
void get_system_name(char* system_name, size_t length);
void get_current_directory(char* current_directory, size_t length);
void construct_prompt(int flag);

#endif