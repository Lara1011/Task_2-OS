//
// Created by malaklinux on 4/21/23.
//

#include "stshell.h"

pid_t child_pid;

void read_input(char *buffer) {
    char *user = getlogin(); // get user name
    char cwd[MAX_INPUT_LENGTH]; // buffer to hold current working directory
    getcwd(cwd, sizeof(cwd)); // get current working directory
    printf("%s:~%s$ ", user, cwd); // display user name and current directory in prompt
    fgets(buffer, MAX_INPUT_LENGTH, stdin); // read user input
    strtok(buffer, "\n"); // remove newline character from buffer
}

void parse_input(char *input, char **args) {
    char *token;
    int i = 0;

    token = strtok(input, " ");
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
}

void sigint_handler() {
    if (child_pid > 0) {
        kill(child_pid, SIGINT);
        printf("\n");
    }
}

void execute_command(char **args) {
    pid_t pid1, pid2;
    int status;

    pid1 = fork();
    if (pid1 == 0) {
        // child process
        execvp(args[0], args);
        // if execvp returns, there was an error
        printf("Error: %s command not found\n", args[0]);
        exit(EXIT_FAILURE);
    } else if (pid1 < 0) {
        // fork failed
        printf("Error: fork failed\n");
        exit(EXIT_FAILURE);
    } else {
        // parent process
        child_pid = pid1; // store the child process ID
        do {
            waitpid(pid1, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        child_pid = 0; // reset the child process ID
    }
}

int main() {
    char input[MAX_INPUT_LENGTH];
    char *args[MAX_INPUT_LENGTH];
    child_pid = 0; // initialize child_pid to 0
    signal(SIGINT, sigint_handler); // install the signal handler

    while (1) {
        read_input(input);
        parse_input(input, args);

        if (strcmp(args[0], "exit") == 0) {
            exit(0);
        } else if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL) {
                // no directory specified, navigate to home directory
                char *home_dir = getenv("HOME");
                if (chdir(home_dir) != 0) {
                    printf("Error: failed to navigate to home directory\n");
                }
            } else {
                // directory specified, change to that directory
                if (chdir(args[1]) != 0) {
                    printf("Error: directory not found\n");
                }
            }
        } else if (strcmp(args[0], "pwd") == 0) {
            char cwd[MAX_INPUT_LENGTH];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("%s\n", cwd);
            } else {
                printf("Error: failed to get current working directory\n");
            }
        } else if (strcmp(args[0], "ls") == 0) {
            pid_t pid;
            int status;

            pid = fork();
            if (pid == 0) {
                // child process
                execvp("/bin/ls", args);
                // if execvp returns, there was an error
                printf("Error: %s command not found\n", args[0]);
                exit(EXIT_FAILURE);
            } else if (pid < 0) {
                // fork failed
                printf("Error: fork failed\n");
                exit(EXIT_FAILURE);
            } else {
                // parent process
                do {
                    waitpid(pid, &status, WUNTRACED);
                } while (!WIFEXITED(status) && !WIFSIGNALED(status));
            }
        } else {
            // execute command with fork + exec + wait
            execute_command(args);
        }
    }
    return 0;
}