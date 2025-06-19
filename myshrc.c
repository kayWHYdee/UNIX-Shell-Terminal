
#include "headers.h"


void remove_comment(char *line) 
{
    char *comment = strchr(line, '#');
    if (comment) *comment = '\0';
}
void parse_alias(char *line) 
{
    char *equals = strchr(line, '=');
    if (equals) {
        *equals = '\0';
        char *name = line;
        char *command = equals + 1;
        
        name = trim(name);
        command = trim(command);
        
        if (strncmp(name, "alias ", 6) == 0) {
            name = trim(name + 6);
        }
        
        strncpy(aliases[alias_count].name, name, sizeof(aliases[alias_count].name) - 1);
        aliases[alias_count].name[sizeof(aliases[alias_count].name) - 1] = '\0';
        strncpy(aliases[alias_count].command, command, sizeof(aliases[alias_count].command) - 1);
        aliases[alias_count].command[sizeof(aliases[alias_count].command) - 1] = '\0';
        
        alias_count++;
    }
}

void parse_myshrc(const char *filename) 
{
    // printf("parse %s\n", filename);

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }
    // printf("open successfully\n");

    char line[MAX_LENGTH];
    
    while (fgets(line, sizeof(line), file)) {
        char *trimmed_line = trim(line);
        remove_comment(trimmed_line);
        
        if (strlen(trimmed_line) == 0) continue;
        
        if (strchr(trimmed_line, '=')) {
            if (alias_count >= MAX_ALIASES) {
                printf("Warning: Maximum number of aliases reached. Ignoring: %s\n", trimmed_line);
            } else {
                parse_alias(trimmed_line);
            }
        } 
        
    }
    
    fclose(file);
    
    // for (int i = 0; i < alias_count; i++) {
    //     if (i < MAX_ALIASES) {
    //         printf("%s = %s\n", aliases[i].name, aliases[i].command);
    //     } else {
    //         printf("Warning: Alias index out of bounds\n");
    //         break;
    //     }
    // }    
}