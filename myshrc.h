#ifndef MYSHRC_H
#define MYSHRC_H    

#define MAX_ALIASES 100
// #define MAX_FUNCTIONS 100

typedef struct {
    char name[50];
    char command[MAX_LENGTH];
} Alias;

// typedef struct {
//     char name[50];
//     char body[MAX_LENGTH];
// } Function;

extern Alias aliases[MAX_ALIASES];
extern int alias_count;

// extern Function functions[MAX_FUNCTIONS];
// extern int function_count;

// Function prototypes
void remove_comment(char *line);
void parse_alias(char *line);
// void parser_function(FILE *file, char *line);
void parse_myshrc(const char *filename);
#endif