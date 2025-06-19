#include "headers.h"
// reveal absolute path and reveal then inside a folder is not working properly!
int flag_a = 0;
int flag_l = 0;

void check_flags(char* flags) 
{
    for (int i = 0; i < strlen(flags); i++) {
        if (flags[i] == 'a') flag_a = 1;
        if (flags[i] == 'l') flag_l = 1;
    }
}
// handle the case reveal -a-l -
void reveal_command(char* command) {
    flag_a = 0;
    flag_l = 0;
    // printf("%s\n",command);
    char* saveptr;
    char* token;
    char* path = calloc(DIR_SIZE,sizeof(char));
    // printf("malloc %s\n",path);
    // char path[DIR_SIZE];
    token = strtok_r(command, " \t\n", &saveptr);
    // printf("token %s\n",token);
    if (token == NULL)
    {
        printf("Error in the command recieved at reveal\n");
        return;
    }
    token = strtok_r(NULL, " \t\n", &saveptr); // to skip the first word i.e. reveal
    int t=0;
    // printf("token 2 %s\n",token);
    while (token != NULL) {
        if (token[0] == '-' && t==0 && strcmp(token,"-")!=0) //added this coz maybe path mein - ho
        {
            check_flags(token);
        } 
        else
        {
            t=1;
            path = strcat(path,token);
        }
        token = strtok_r(NULL, " \t\n\0", &saveptr);
    }

    // after the flags there will be ONLY 1 ARGUMENT
    // printf("path bef %s\n",path);
    if (path[0] == '\0') {
        path = ".";
    }
    else if(strcmp(path,"~")==0)
    {
        path = strdup(HOME_DIR);
    }
    else if(strcmp(path,"-")==0)
    {
        path = strdup(PREV_DIR);
        // printf("The preveious directory is %s\n", PREV_DIR);
    }
    else if(strcmp(path,"..")==0)
    {
        path = strdup(CURR_DIR);
        strcat(path,"/..");
    }    
    // printf("--%s is the path that is being opened \n", path);
    DIR* dir = opendir(path);
    if (dir == NULL) {
        
            perror("opendir() error");
        return;
    }

    struct dirent* entry;
    struct stat file_stat;
    struct dirent* entries[MAX_LENGTH];
    int entry_count = 0;

    while ((entry = readdir(dir)) != NULL) 
    {
        if (!flag_a && entry->d_name[0] == '.') 
        {
            continue;
        }
        entries[entry_count++] = entry;
    }
    qsort(entries, entry_count, sizeof(struct dirent*), 
          (int (*)(const void*, const void*))compare_entries);
    
    printf("Entry count --> %d  \n",entry_count);
    if (flag_l)
    {
        // print the tota
        struct stat st;
        stat (path, &st);
        printf("total block size = %ld\n", st.st_blksize);
    }
    // printing part
    for (int i = 0; i < entry_count; i++) {
        char filepath[DIR_SIZE];
        snprintf(filepath, DIR_SIZE, "%s/%s", path, entries[i]->d_name);
        struct stat file_stat;
        if (stat(filepath, &file_stat) == -1) {
            perror("stat() error");
            continue;
        }
        if (flag_l) {

            printf("%c", (S_ISDIR(file_stat.st_mode)) ? 'd' : '-');
            printf("%c%c%c", (file_stat.st_mode & S_IRUSR) ? 'r' : '-',
                            (file_stat.st_mode & S_IWUSR) ? 'w' : '-',
                            (file_stat.st_mode & S_IXUSR) ? 'x' : '-');
            printf("%c%c%c", (file_stat.st_mode & S_IRGRP) ? 'r' : '-',
                            (file_stat.st_mode & S_IWGRP) ? 'w' : '-',
                            (file_stat.st_mode & S_IXGRP) ? 'x' : '-');
            printf("%c%c%c ", (file_stat.st_mode & S_IROTH) ? 'r' : '-',
                            (file_stat.st_mode & S_IWOTH) ? 'w' : '-',
                            (file_stat.st_mode & S_IXOTH) ? 'x' : '-');

            printf("%4ld ", file_stat.st_nlink);
            printf("%s %s ", getpwuid(file_stat.st_uid)->pw_name, getgrgid(file_stat.st_gid)->gr_name);
            printf("%9ld ", file_stat.st_size);
            printf("%.12s ", 4 + ctime(&file_stat.st_mtime));
        }
        // add conditions here condition Use specific color coding to differentiate between file names, directories 
        // and executables in the output [green for executables, white for files and blue for directories].
        if (S_ISDIR(file_stat.st_mode)) {
            printf(BLUE "%s" RESET "\n", entries[i]->d_name);
        } else if (file_stat.st_mode & S_IXUSR) {
            printf(GREEN "%s" RESET "\n", entries[i]->d_name);
        } else {
            printf(WHITE "%s" RESET "\n", entries[i]->d_name);
        }    
    }
    closedir(dir);
    // printf("returning\n");
    return;
}
int compare_entries(const struct dirent** a, const struct dirent** b) {
    return strcmp((*a)->d_name, (*b)->d_name);
}
