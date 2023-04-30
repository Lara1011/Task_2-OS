//
// Created by malaklinux on 4/21/23.
//

#ifndef TASK_2_OS_STSHELL_H
#define TASK_2_OS_STSHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <pwd.h>

#define MAX_CMD_LENGTH 1024
#define MAX_ARGS 265

volatile sig_atomic_t cmd_running;

void handle_sigint();

int parse_cmd(char *cmd, char **args, char **next_cmd, int *output_type);

void exec_with_multiple_pipes(char **args, char **next_args, char **third_args);

void exec_with_redirection(char **args, char *file, int output_type);

int main();

#endif //TASK_2_OS_STSHELL_H



