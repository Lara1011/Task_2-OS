//
// Created by malaklinux on 4/21/23.
//

#ifndef TASK_2_OS_STSHELL_H
#define TASK_2_OS_STSHELL_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <wait.h>
#include <stdbool.h>

#define MAX_ARGS 1000
#define MAX_INPUT_LENGTH 8192
bool CTRL_C;
int num_pipes;
int pipe_fds[2][2];
char *args[MAX_ARGS];
char input_file[MAX_INPUT_LENGTH];
char output_file[MAX_INPUT_LENGTH];
int output_mode; // 0 for overwrite, 1 for append

#endif //TASK_2_OS_STSHELL_H



