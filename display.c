#include "headers.h"

void construct_prompt(int flag)
{

    char system_name[MAX_LENGTH];
    gethostname(system_name, MAX_LENGTH);
    char *prompt = malloc(MAX_LENGTH);
    if (prompt == NULL)
    {
        perror("malloc error");
        exit(EXIT_FAILURE);
    }

    if (flag == 0)
    {

        if (strncmp(CURR_DIR, HOME_DIR, strlen(HOME_DIR)) == 0)
        {
            char *dis_path = (char *)calloc(1024, sizeof(char));
            strcat(dis_path, "~");
            strcat(dis_path, CURR_DIR + strlen(HOME_DIR));
            snprintf(prompt, MAX_LENGTH, "<%s@%s:%s> ", USERNAME, system_name, dis_path);
        }
        else
        {
            snprintf(prompt, MAX_LENGTH, "<%s@%s:%s> ", USERNAME, system_name, CURR_DIR);
        }
    }
    else if (flag == 1)
    {
        if (strncmp(CURR_DIR, HOME_DIR, strlen(HOME_DIR)) == 0)
        {
            char *dis_path = (char *)calloc(1024, sizeof(char));
            strcat(dis_path, "~");
            strcat(dis_path, CURR_DIR + strlen(HOME_DIR));
            snprintf(prompt, MAX_LENGTH, "<%s@%s:%s %s> ", USERNAME, system_name, dis_path, TIME->command);
        }
        else
        {
            snprintf(prompt, MAX_LENGTH, "<%s@%s:%s %s> ", USERNAME, system_name, CURR_DIR, TIME->command);
        }
    }
    printf("%s", prompt);
    free(prompt);
    handle_input(0);
}
