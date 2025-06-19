#include "headers.h"
void handle_input()
{
    char input[MAX_LENGTH];
    if (fgets(input, MAX_LENGTH, stdin) == NULL)
    {
        if (feof(stdin))
        {                    // Ctrl-D is detected as EOF
            handle_ctrl_d();
        }
        perror("fgets error");
        return;
    }
    if (strcmp(input, "\n") == 0)
    {
        return;
    }
    tokenize(input, 0);
    // printf("this is the input %s\n",input);
}

void tokenize(char *input, int gg)
{
    if (gg == 0)
    {
        update_log(input);
    }
    char *input_s = strdup(input);
    char *token;
    char *inputptr = NULL;
    char *delim = ";";
    token = strtok_r(input_s, delim, &inputptr);
    // check for < > >> ;; if found send it to redirection_handler, 
    while (token != NULL)
    {
        int j = 0;
        for (int i = 0; i < strlen(token); i++)
        {
            if (token[i] == '&')
            {
                char *bash_comm = calloc(DIR_SIZE, sizeof(char));
                strncpy(bash_comm, token + j, (i - j));
                j = i + 1;
                bash_comm = trim(bash_comm);
                execute_background_process(bash_comm);
            }
        }

        if (j == 0)
        {
            if (strchr(token, '<') || strchr(token, '>'))
            {
                redirection_handler(token);
            }
            else if (strchr(token, '|'))
            {
                pipe_handler(token);
            }
            
            else
            {
                token = trim(token);
                parse_command(token);
            }
        }
        token = strtok_r(NULL, delim, &inputptr);
    }
    free(input_s);
}

void parse_command(char *command)
{
    // hop .. ; h
    char *args[MAX_LENGTH];
    int i = 0;
    char *command_s = strdup(command);
    char *saveptr;
    args[i] = strtok_r(command, " \t\n", &saveptr);
    while (args[i] != NULL)
    {
        i++;
        args[i] = strtok_r(NULL, " \t\n", &saveptr);
    }

    if (strcmp(args[0], "hop") == 0)
    {
        hop_command(command_s);
    }
    else if (strcmp(args[0], "reveal") == 0)
    {
        reveal_command(command_s);
    }
    else if (strcmp(args[0], "log") == 0)
    {
        // // printf("arg 1 %s jaba",args[1]);
        if (args[1] == NULL)
        {
            print_log();
        }
        else if (strcmp(args[1], "execute") == 0 && args[2] != NULL)
        {
            int index = atoi(args[2]);
            log_execute(index);
        }
        else if (strcmp(args[1], "purge") == 0)
        {
            log_purge();
        }
        else
            perror("Invalid log command");
    }
    else if (strcmp(args[0], "seek") == 0)
    {

        seek_command(command_s);
    }
    else if (strcmp(args[0], "proclore") == 0)
    {

        proc_command(command_s);
    }
    else if (strcmp(args[0], "activities") == 0)
    {
        activities_command();
    }
    else if (strcmp(args[0], "ping") == 0)
    {

        ping_command(command_s);
    }
    else if (strcmp(args[0],"bg")==0)
    {
        tobg_command(command_s);
    }else if (strcmp(args[0],"fg")==0)
    {
        tofg_command(command_s);
    }
    else if (strcmp(args[0],"iMan")==0)
    {
        iman_command(command_s);
    }
    else if (strcmp(args[0],"neonate")==0)
    {
        neonate_command(command_s);
    }
    else
    {
        for (int i = 0; i < alias_count; i++)
        {
            // printf("%s %s\n",aliases[i].name,args[0]);
            if (strcmp(aliases[i].name,args[0])==0)
            {
                parse_command(aliases[i].command);
                return;
            }
            
        }
        bash_command(command_s);
    }
    free(command_s);
    return;
}