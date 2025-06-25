#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>


int main() {
    int pipeUp[2];

    if (pipe(pipeUp) == -1) {
        perror("Error creating pipe");
        exit(EXIT_FAILURE);
    }

    int pid = fork();

    if (pid == -1) {
        perror("Error forking");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        close(pipeUp[0]);
        dup2(pipeUp[1], STDOUT_FILENO);
        close(pipeUp[1]);

        execlp("sort","sort","fx.txt",(char *)NULL);

        perror("Error executing the exec function\n");
        exit(EXIT_SUCCESS);
    }

    close(pipeUp[1]);
    char outputPipe[PIPE_BUF];
    printf("Im the father!\n\n");

    while (read(pipeUp[0], outputPipe, sizeof(outputPipe)) > 0) {
        write(STDOUT_FILENO,outputPipe,sizeof(outputPipe));
    }


    close(pipeUp[0]);
    return 0;
}
