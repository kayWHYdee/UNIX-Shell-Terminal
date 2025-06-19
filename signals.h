#ifndef SIGNALS_H
#define SIGNALS_H



void setup_signal_handlers();
void handle_sigint(int sig);
void handle_sigtstp(int sig);
void handle_ctrl_d();

#endif