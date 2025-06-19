#include "headers.h"
char *HOME_DIR = NULL;
char *PREV_DIR = NULL;
char *CURR_DIR = NULL;
char *USERNAME = NULL;
char *LOG_FILE = NULL;
LogQueue g_log;
pid_t SHELL_PID = 0;
pid_t fg_pid = 0;
ForegroundProcess *FG_PROC;
int input_fd = 0;
int output_fd = 0;
Alias aliases[MAX_ALIASES];
int alias_count = 0;

struct TimeInfo *TIME;
char *trim(char *str)
{
    char *end;
    while (isspace((unsigned char)*str))
        str++;

    if (*str == 0)
        return str;

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;

    *(end + 1) = 0;
    return str;
}
int main()
{

    FG_PROC = malloc(sizeof(struct ForegroundProcess));
    FG_PROC->command = calloc(DIR_SIZE, sizeof(char));
    input_fd = -1; 
    output_fd = -1;
    init_bg_queue();
    SHELL_PID = getpid();
    FG_PROC->pid = SHELL_PID;
    USERNAME = getenv("USER");
    HOME_DIR = malloc(sizeof(char) * DIR_SIZE);
    getcwd(HOME_DIR, DIR_SIZE);
    LOG_FILE = malloc(sizeof(char) * DIR_SIZE);
    snprintf(LOG_FILE, DIR_SIZE, "%s/%s", HOME_DIR, "logcommands.txt");
    PREV_DIR = malloc(sizeof(char) * DIR_SIZE);
    CURR_DIR = malloc(sizeof(char) * DIR_SIZE);
    strcpy(CURR_DIR, HOME_DIR);
    strcpy(PREV_DIR, "**");
    init_log(); // initialisze the log queue
    setup_signal_handlers();
    parse_myshrc("myshrc.txt");

    while (1)
    {
        check_background_processes();
        construct_prompt(0);
    }
    return 0;
}
