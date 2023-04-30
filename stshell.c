//
// Created by malaklinux on 4/21/23.
//

#include "stshell.h"
//ls -l | grep aaa | wc

// Add a global variable to track if a command is running
volatile sig_atomic_t cmd_running = 0;

void handle_sigint() {
    if (!cmd_running) {
        struct passwd *pw = getpwuid(getuid());
        char hostname[MAX_CMD_LENGTH];
        hostname[MAX_CMD_LENGTH - 1] = '\0';
        gethostname(hostname, MAX_CMD_LENGTH - 1);
        char cwd[MAX_CMD_LENGTH];
        getcwd(cwd, sizeof(cwd));

        printf("\n%s@%s:%s$ ", pw->pw_name, hostname, cwd);
        fflush(stdout);
    } else {
        printf("\n");
        cmd_running = 0; // Set cmd_running to 0 after handling SIGINT
    }
}


int parse_cmd(char *cmd, char **args, char **next_cmd, int *output_type) {
    char *token = strtok(cmd, " \n\t");
    int i = 0;
    int pipe_found = 0;

    while (token != NULL) {
        if (strcmp(token, ">") == 0) {
            *output_type = 1;
            token = strtok(NULL, " \n\t");
            *next_cmd = token;
            break;
        } else if (strcmp(token, ">>") == 0) {
            *output_type = 2;
            token = strtok(NULL, " \n\t");
            *next_cmd = token;
            break;
        } else if (strcmp(token, "|") == 0) {
            pipe_found = 1;
            args[i] = NULL;
            *next_cmd = strtok(NULL, "");
            break;
        } else {
            args[i++] = token;
        }
        token = strtok(NULL, " \n\t");
    }

    args[i] = NULL;
    return pipe_found;
}

void exec_with_multiple_pipes(char **args, char **next_args, char **third_args) {
    int pipefd1[2];
    int pipefd2[2];

    if (pipe(pipefd1) == -1) {
        perror("Error creating pipe");
        exit(EXIT_FAILURE);
    }

    if (pipe(pipefd2) == -1) {
        perror("Error creating pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid1 = fork();

    if (pid1 == 0) {
        dup2(pipefd1[1], STDOUT_FILENO);
        close(pipefd1[0]);
        close(pipefd1[1]);
        execvp(args[0], args);
        perror("Error executing command");
        exit(EXIT_FAILURE);
    }

    pid_t pid2 = fork();

    if (pid2 == 0) {
        dup2(pipefd1[0], STDIN_FILENO);
        dup2(pipefd2[1], STDOUT_FILENO);
        close(pipefd1[0]);
        close(pipefd1[1]);
        close(pipefd2[0]);
        close(pipefd2[1]);
        execvp(next_args[0], next_args);
        perror("Error executing command");
        exit(EXIT_FAILURE);
    }

    close(pipefd1[0]);
    close(pipefd1[1]);

    pid_t pid3 = fork();

    if (pid3 == 0) {
        dup2(pipefd2[0], STDIN_FILENO);
        close(pipefd2[0]);
        close(pipefd2[1]);
        execvp(third_args[0], third_args);
        perror("Error executing command");
        exit(EXIT_FAILURE);
    }

    close(pipefd2[0]);
    close(pipefd2[1]);

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);
}


void exec_with_redirection(char **args, char *file, int output_type) {
    int fd;

    if (output_type == 1) {
        fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    } else {
        fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    }

    if (fd == -1) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    dup2(fd, STDOUT_FILENO);
    close(fd);
    execvp(args[0], args);
    perror("Error executing command");
    exit(EXIT_FAILURE);
}

int main() {
    char cmd[MAX_CMD_LENGTH];
    char *args[MAX_ARGS];
    char *next_cmd;
    char *next_args[MAX_ARGS];
    char *third_cmd;
    char *third_args[MAX_ARGS];
    int output_type = 0;
    pid_t pid;
    int status;

    // Set up signal handler for SIGINT (Ctrl+C)
    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Error setting up signal handler");
        exit(EXIT_FAILURE);
    }

    struct passwd *pw = getpwuid(getuid());
    char hostname[MAX_CMD_LENGTH];
    hostname[MAX_CMD_LENGTH - 1] = '\0';
    gethostname(hostname, MAX_CMD_LENGTH - 1);

    while (1) {
        if (!cmd_running) { // Only print the shell prompt if cmd_running is 0
            char cwd[MAX_CMD_LENGTH];
            getcwd(cwd, sizeof(cwd));
            printf("%s@%s:%s$ ", pw->pw_name, hostname, cwd);
            fflush(stdout);
        }

        fgets(cmd, MAX_CMD_LENGTH, stdin);

        int pipe_found = parse_cmd(cmd, args, &next_cmd, &output_type);

        if (strncmp(cmd, "exit", 4) == 0 && (cmd[4] == ' ' || cmd[4] == '\n' || cmd[4] == '\t' || cmd[4] == '\0')) {
            if (cmd_running) {
                fclose(stdin);
                kill(pid, SIGTERM);
                waitpid(pid, &status, 0);
            }
            break;
        }

        if (strncmp(cmd, "cd", 2) == 0 && (cmd[2] == ' ' || cmd[2] == '\n' || cmd[2] == '\t' || cmd[2] == '\0')) {
            char *path = args[1];
            if (!path || strcmp(path, "~") == 0) {
                path = pw->pw_dir;
            }
            if (chdir(path) != 0) {
                perror("Error changing directory");
            }
            continue;
        }

        if (pipe_found) {
            pipe_found = parse_cmd(next_cmd, next_args, &third_cmd, &output_type);
            if (pipe_found) {
                parse_cmd(third_cmd, third_args, NULL, NULL);
            }
        }
        cmd_running = 1; // Set cmd_running to 1 before fork
        pid = fork();

        if (pid == 0) { // child process
            cmd_running = 0; // Set cmd_running to 0 in the child process
            if (output_type > 0) {
                exec_with_redirection(args, next_cmd, output_type);
            } else if (pipe_found) {
                exec_with_multiple_pipes(args, next_args, third_args);
            } else {
                execvp(args[0], args);
                perror("Error executing command");
                exit(EXIT_FAILURE);
            }
        } else if (pid > 0) { // parent process
            if (!pipe_found) { // Only wait for the child process if there's no pipe
                waitpid(pid, &status, 0);
                if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
                    exit(EXIT_FAILURE);
                }
                cmd_running = 0;
            }
        } else { // error
            perror("Error forking");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}


