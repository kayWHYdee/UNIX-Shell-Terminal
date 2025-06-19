#include "headers.h"

// Initialize the log by reading the last 15 lines from the log file
void init_log() {
    FILE *log_file = fopen(LOG_FILE, "r");
    g_log.start = 0;
    g_log.count = 0;

    if (log_file) {
        char line[DIR_SIZE];
        char *last_15[LOG_SIZE];  // Store last 15 commands temporarily
        int total_lines = 0;

        // Read through the entire file to count the number of commands
        while (fgets(line, sizeof(line), log_file)) {
            line[strcspn(line, "\n")] = 0;  // Remove trailing newline

            // Keep only the last 15 lines
            if (total_lines < LOG_SIZE) {
                last_15[total_lines] = strdup(line);  // Store command in array
            } else {
                free(last_15[total_lines % LOG_SIZE]);  // Free older commands
                last_15[total_lines % LOG_SIZE] = strdup(line);
            }
            total_lines++;
        }

        // Copy the last 15 commands into the log queue
        int start_index = (total_lines > LOG_SIZE) ? total_lines - LOG_SIZE : 0;
        int count = (total_lines > LOG_SIZE) ? LOG_SIZE : total_lines;

        for (int i = 0; i < count; i++) {
            strncpy(g_log.commands[i], last_15[(start_index + i) % LOG_SIZE], DIR_SIZE - 1);
            g_log.commands[i][DIR_SIZE - 1] = '\0';
        }
        g_log.count = count;

        // Free the dynamically allocated memory
        for (int i = 0; i < (total_lines > LOG_SIZE ? LOG_SIZE : total_lines); i++) {
            free(last_15[i]);
        }
        fclose(log_file);
    }
}

// Update the log queue and append the command to the log file
void update_log(char* command) {
    if (command[0] == '\0') return;

    // Don't log the same command consecutively
    if (g_log.count > 0) {
        int last_index = (g_log.start + g_log.count - 1) % LOG_SIZE;
        if (strcmp(trim(g_log.commands[last_index]), trim(command)) == 0) return;
        // printf("comparing between %s and %s\n",g_log.commands[last_index],command);
    }
    if (strstr(command, "log") != NULL) return;

    // Write the command to the log queue
    int index = (g_log.start + g_log.count) % LOG_SIZE;
    strncpy(g_log.commands[index], command, DIR_SIZE - 1);
    g_log.commands[index][DIR_SIZE - 1] = '\0';

    if (g_log.count < LOG_SIZE) {
        g_log.count++;
    } else {
        g_log.start = (g_log.start + 1) % LOG_SIZE;
    }

    // Append the command to the log file
    // printf("why?\n");
    FILE *log_file = fopen(LOG_FILE, "a");
    if (log_file) 
    {
        fprintf(log_file, "%s\n", command);
        fclose(log_file);
    }
}

void print_log() {
    for (int i = 0; i < g_log.count; i++) 
    {
        int index = (g_log.start + i) % LOG_SIZE;
        printf("%d) %s\n", i + 1, g_log.commands[index]);
    }
}

// Purge the log and clear the log file
void log_purge() 
{

    // Clear the log file
    FILE *log_file = fopen(LOG_FILE, "w");
    if (log_file) {
        fclose(log_file);
    }
    init_log();  
}

// Execute a command from the log
void log_execute(int index) {
    if (index < 1 || index > g_log.count) {
        printf("Invalid index!\n");
        return;
    }
    int cmd_index = (g_log.start + index - 1) % LOG_SIZE;
    printf("Executing: %s\n", g_log.commands[cmd_index]);
    tokenize(g_log.commands[cmd_index], 1);
}
