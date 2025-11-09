#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    // TODO: Complete and document
    if (argc != 2) {
        printf("USAGE: partc FILEIN\n");
        return 1;
    }
    char* filename = argv[1];
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        printf("ERROR: %s not found\n", filename);
        return 2;
    }
    pid_t pid = fork();
    if (pid == -1) {
        printf("ERROR: Fork failed\n");
        close(fd);
        return 3;
    } else if (pid == 0) {
        // Child process
        int pipefd[2];
        if (pipe(pipefd) == -1) {
            printf("ERROR: pipe failed\n");
            close(fd);
            return 4;
        }

        pid_t child_pid = fork();
        if (child_pid == -1) {
            printf("ERROR: second fork failed\n");
            close(fd);
            close(pipefd[0]);
            close(pipefd[1]);
            return 5;
        }

        if (child_pid == 0) {
            // Grandchild process - runs parta
            close(pipefd[0]); // Close read end
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]);
            
            char* args[] = {"./parta", filename, NULL};
            execv(args[0], args);
            printf("ERROR: execv failed\n");
            return 6;
        } else {
            // Child process - reads and reorders output
            close(pipefd[1]); // Close write end
            char line[256];
            char* lines[5] = {NULL};
            int lineCount = 0;
            
            FILE* fp = fdopen(pipefd[0], "r");
            if (fp == NULL) {
                printf("ERROR: fdopen failed\n");
                return 7;
            }

            // Read all lines
            while (fgets(line, sizeof(line), fp) && lineCount < 5) {
                lines[lineCount] = strdup(line);
                lineCount++;
            }
            fclose(fp);

            // Print in the required order
            for (int i = 0; i < lineCount; i++) {
                if (strncmp(lines[i], "Number,", 7) == 0) printf("%s", lines[i]);
            }
            for (int i = 0; i < lineCount; i++) {
                if (strncmp(lines[i], "Other,", 6) == 0) printf("%s", lines[i]);
            }
            for (int i = 0; i < lineCount; i++) {
                if (strncmp(lines[i], "Upper,", 6) == 0) printf("%s", lines[i]);
            }
            for (int i = 0; i < lineCount; i++) {
                if (strncmp(lines[i], "Space,", 6) == 0) printf("%s", lines[i]);
            }
            for (int i = 0; i < lineCount; i++) {
                if (strncmp(lines[i], "Lower,", 6) == 0) printf("%s", lines[i]);
            }

            // Free allocated memory
            for (int i = 0; i < lineCount; i++) {
                free(lines[i]);
            }

            exit(0);
        }
        // If execv returns, there was an error
        printf("ERROR: execv failed\n");
        return 5;
    } else {
        // Parent process
        close(fd);
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
            if (exit_status != 0) {
                printf("ERROR: parta exited with status %d\n", exit_status);
                return 6;
            }
        } else {
            printf("ERROR: parta did not terminate normally\n");
            return 7;
        }
    }
    return 0;
}
