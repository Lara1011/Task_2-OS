//
// Created by malaklinux on 4/21/23.
//

#include "stshell.h"

void signal_handler(int signal_number) {
    CTRL_C = false;
    if (signal_number == SIGINT) {  //CTRL+C = 2
        CTRL_C = true;
        printf("\n");
        char *user = getlogin();
        char cwd[MAX_INPUT_LENGTH];
        getcwd(cwd, sizeof(cwd));

        printf("%s:~%s$ ", user, cwd);
        fflush(stdout);

    }
}

void execute_command() {
    CTRL_C = false;
    // Close all pipes before executing the command
    for (int i = 0; i < num_pipes; i++) {
        close(pipe_fds[i][0]);
        close(pipe_fds[i][1]);
    }

    // Redirect input/output if necessary
    if (strlen(input_file) > 0) {
        int input_fd = open(input_file, O_RDONLY);
        dup2(input_fd, STDIN_FILENO);
        close(input_fd);
    }
    if (strlen(output_file) > 0) {
        int output_fd;
        if (output_mode == 0) {
            output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        } else {
            output_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0666);
        }
        dup2(output_fd, STDOUT_FILENO);
        close(output_fd);
    }

    // Execute the command
    if (execvp(args[0], args) < 0) {
        fprintf(stderr, "Command not found\n");
        exit(1);
    }
}

void parse_command(char *command) {
    CTRL_C = false;
    char *token = strtok(command, " ");
    int i = 0;
    while (token != NULL) {
        if (strcmp(token, ">") == 0) {
            // Output redirection
            token = strtok(NULL, " ");
            strcpy(output_file, token);
            output_mode = 0;
        } else if (strcmp(token, ">>") == 0) {
            // Output redirection (append mode)
            token = strtok(NULL, " ");
            strcpy(output_file, token);
            output_mode = 1;
        } else if (strcmp(token, "<") == 0) {
            // Input redirection
            token = strtok(NULL, " ");
            strcpy(input_file, token);
        } else if (strcmp(token, "|") == 0) {
            // Pipeline
            num_pipes++;
        } else {
            args[i] = token;
            i++;
        }
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
}

void execute_pipeline(int pipeline_num) {
    CTRL_C = false;
    if (pipeline_num == num_pipes) {
        execute_command();
    } else {
        pid_t pid;
        if (pipe(pipe_fds[pipeline_num]) < 0) {
            fprintf(stderr, "Pipe error\n");
            exit(1);
        }

        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "Fork error\n");
            exit(1);
        } else if (pid == 0) {
            // Child process
            close(pipe_fds[pipeline_num][0]);
            dup2(pipe_fds[pipeline_num][1], STDOUT_FILENO);
            close(pipe_fds[pipeline_num][1]);
            execute_pipeline(pipeline_num + 1);

        } else {
            // Parent process
            close(pipe_fds[pipeline_num][1]);
            dup2(pipe_fds[pipeline_num][0], STDIN_FILENO);
            close(pipe_fds[pipeline_num][0]);
            wait(NULL);
            execute_command();
        }
    }
}


int main() {
    CTRL_C = false;
    signal(SIGINT, signal_handler);
    while (1) {
        // Reset variables
        num_pipes = 0;
        input_file[0] = '\0';
        output_file[0] = '\0';
        output_mode = 0;

        char *user = getlogin(); // get user name
        char cwd[MAX_INPUT_LENGTH]; // buffer to hold current working directory
        getcwd(cwd, sizeof(cwd)); // get current working directory
        if (!CTRL_C)
            printf("%s:~%s$ ", user, cwd); // display user name and current directory in prompt

        // Read input
        char command[MAX_INPUT_LENGTH];
        if (fgets(command, MAX_INPUT_LENGTH, stdin) == NULL) {
            printf("\n");
            exit(0);
        }

        // Remove newline character
        command[strlen(command) - 1] = '\0';

        // Parse the command
        parse_command(command);

        // Check for built-in commands
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
        } else {
            // Execute the command
            pid_t pid = fork();
            if (pid == 0) {
                // Child process
                execute_pipeline(0);
                exit(0);
            } else if (pid > 0) {
                // Parent process
                wait(NULL);
            } else {

                // Fork error
                fprintf(stderr, "Fork error\n");
                exit(1);
            }
        }

        // Check if CTRL+C was pressed
        if (CTRL_C) {
            // Reset CTRL+C flag
            CTRL_C = false;
        }
    }
}
