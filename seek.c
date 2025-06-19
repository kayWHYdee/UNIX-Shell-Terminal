#include "headers.h"

int fl_d = 0, fl_e = 0, fl_f = 0;
int count = 0;
char execute_path[DIR_SIZE];
void print_file_contents(const char *filepath)
{
    FILE *file = fopen(filepath, "r");
    if (file == NULL)
    {
        printf("Error: Unable to open file %s\n", filepath);
        return;
    }
    char buffer[4096];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0)
    {
        fwrite(buffer, 1, bytes_read, stdout);
    }
    printf("\n");
    fclose(file);
}

void search_directory(char *directory, char *target, int fl_d, int fl_f, int fl_e,char *directory1)
{
    DIR *dir;
    struct dirent *entry;
    char path[DIR_SIZE];
    struct stat statbuf;
    char path1[DIR_SIZE];
    dir = opendir(directory1);
    if (dir == NULL)
    {
        perror("Error opening directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || entry->d_name[0] == '.')
            continue;

        snprintf(path, sizeof(path), "%s/%s", directory, entry->d_name);
        snprintf(path1, sizeof(path1), "%s/%s", directory1, entry->d_name);
                // snprintf(path, sizeof(path), "%s/%s", directory, entry->d_name);
        if (stat(path1, &statbuf) == -1)
        {
            perror("Error getting file status");
            continue;
        }

        if (S_ISDIR(statbuf.st_mode)) // Recursively search within directories
        {
            search_directory(path, target, fl_d, fl_f, fl_e,path1);
        }

        if (strncmp(entry->d_name, target, strlen(target)) == 0)
        {
            strcpy(execute_path,path1);
            if ((S_ISDIR(statbuf.st_mode) && !fl_f) || (!S_ISDIR(statbuf.st_mode) && !fl_d))
            {
                count++;
                if (S_ISDIR(statbuf.st_mode))
                {
                    printf(BLUE "%s" RESET "\n", path);
                }
                else
                {
                    printf(GREEN "%s" RESET "\n", path);
                }
            }
        }
    }

    closedir(dir);
}

void seek_command(char *command)
{
    fl_d = 0;
    fl_e = 0;
    fl_f = 0;
    count = 0;
    char *command_s = strdup(command);
    char *target = calloc(DIR_SIZE, sizeof(char));
    char directory[DIR_SIZE] = ".";

    char *token;
    char *input_ptr = NULL;
    token = strtok_r(command, " \t\n", &input_ptr);
    token = strtok_r(NULL, " \t\n", &input_ptr);

    while (token != NULL)
    {
        if (strcmp(token, "-d") == 0)
        {
            fl_d = 1;
        }
        else if (strcmp(token, "-f") == 0)
        {
            fl_f = 1;
        }
        else if (strcmp(token, "-e") == 0)
        {
            fl_e = 1;
        }
        else if (target[0] == '\0')
        {
            strcpy(target, token);
        }
        else
        {
            strcpy(directory, token);
            break;
        }
        token = strtok_r(NULL, " \t\n", &input_ptr);
    }

    if (strcmp("-", directory) == 0)
    {
        strcpy(directory, PREV_DIR);
    }
    else if (strcmp("~", directory) == 0)
    {
        strcpy(directory, HOME_DIR);
    }
    struct stat statbuf;

    if (fl_f==1 && fl_d == 1)
    {
        printf("Invalid Flags");
        return;
    }
    
    search_directory(".", target, fl_d, fl_f, fl_e,directory);
    if (count == 0)
    {
        printf("No Match Found!");
    }
    
    if (fl_e && count == 1)
    {
    if (stat(execute_path, &statbuf) == -1)
    {
        perror("Error getting file status");
    }
    else{

        if (S_ISDIR(statbuf.st_mode))
        {
            char to_hop_command[DIR_SIZE];
            snprintf(to_hop_command, sizeof(to_hop_command), "hop %s", execute_path);
            hop_command(to_hop_command);
        }
        else
        {
            print_file_contents(execute_path);
        }
    }
    }
    free(command_s);
    free(target);
}
