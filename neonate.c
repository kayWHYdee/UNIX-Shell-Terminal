#include "headers.h"

void reset_terminal_mode(struct termios *orig_termios)
{
    tcsetattr(0, TCSANOW, orig_termios);
}

void set_conio_terminal_mode(struct termios *orig_termios)
{
    struct termios new_termios;
    tcgetattr(0, orig_termios);
    memcpy(&new_termios, orig_termios, sizeof(new_termios));
    new_termios.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
    tcsetattr(0, TCSANOW, &new_termios);
}

int get_most_recent_pid()
{
    DIR *proc = opendir("/proc");
    struct dirent *entry;
    int max_pid = -1;

    if (proc == NULL)
    {
        perror("ERROR: Could not open /proc directory");
        return -1;
    }

    while ((entry = readdir(proc)) != NULL)
    {
        if (isdigit(*entry->d_name))
        {
            int pid = atoi(entry->d_name);
            if (pid > max_pid)
            {
                max_pid = pid;
            }
        }
    }
    closedir(proc);
    return max_pid;
}

void neonate_command(char *command)
{
    char *token;
    char *input_ptr = NULL;
    token = strtok_r(command, " \t\n", &input_ptr);
    char *flag = strtok_r(NULL, " \t\n", &input_ptr);
    if (strcmp(flag,"-n")!=0)
    {
        printf("invalid command!\n");
        return;
    }
    
    double ftime_arg = strtod(strtok_r(NULL, " \t\n", &input_ptr),NULL);
    int time_arg = (int) ftime_arg;
    if (time_arg < 0 ||(floor(ftime_arg)!=ftime_arg) )
    {
        printf("Invalid time\n");
        return;
    }
    
    printf("time : %d",time_arg);
    struct termios orig_termios;
    set_conio_terminal_mode(&orig_termios);

    printf("Press 'x' to exit\n");

    while (1)
    {
        // Check for key press without blocking
        fd_set readfds;
        struct timeval tv;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        tv.tv_sec = 0;
        tv.tv_usec = 0;

        int max_pid = get_most_recent_pid();
        if (max_pid != -1)
        {
            printf("%d\n", max_pid);
        }

        if (select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv) == 1)
        {
            char ch;
            read(STDIN_FILENO, &ch, 1);
            if (ch == 'x')
            {
                break; // Exit if 'x' is pressed
            }
        }

        sleep(time_arg); // Sleep for the specified time interval
    }

    reset_terminal_mode(&orig_termios);
}
