#ifndef NEONET_H
#define NEONET_H

void reset_terminal_mode(struct termios* orig_termios);
void set_conio_terminal_mode(struct termios *orig_termios);
void neonate_command(char *command);
int get_most_recent_pid();


#endif