#include "headers.h"

//instead of hop ~/new hop ~ new is working
 void hop_command(char * command)
 {
    char * command_s = strdup(command); 
    char* token;
    char* input_ptr = NULL;

    // printf("command - %s\n",command);
    token = strtok_r(command, " \t\n", &input_ptr);
    // printf("initial token = %s \n",token);
    token = strtok_r(NULL, " \t\n", &input_ptr);
    // printf("final token = %s \n",token);
    while (token != NULL) 
    {
        if (strcmp(token, "~") == 0) 
        {
            chdir(HOME_DIR); 
        } 
        else if (strcmp(token, "-") == 0) 
        {
            if (strcmp(PREV_DIR,"**")==0)
            {
                printf("Warning : OLDPWD not set");
                chdir(HOME_DIR);
            }
            else
            {
                chdir(PREV_DIR);
            }
        } 
        else if (strcmp(token, "..") == 0)
        {
            chdir("..");
        }
         else if (strcmp(token, ".") == 0) 
        {} 
        else 
        { // a sprcific directory
            if (chdir(token) != 0) 
            {
                perror("Can not be accessed");
                return;
            }
        }
        //S_ISDIR to check
        strcpy(PREV_DIR,CURR_DIR);

        getcwd(CURR_DIR, MAX_LENGTH);
        printf("%s \n", CURR_DIR);
        token = strtok_r(NULL, " \t\n", &input_ptr);
    }
    return;
}