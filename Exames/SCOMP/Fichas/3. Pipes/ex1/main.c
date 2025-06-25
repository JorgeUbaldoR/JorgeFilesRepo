#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

int main() {
    int fd[2];

    if (pipe(fd) == -1) {
        perror("Error in pipe\n");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("Error in fork\n");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        close(fd[1]);
        pid_t inputPipe = -1;
        int nBytesReaded = read(fd[0],&inputPipe,sizeof(pid_t));

        if (nBytesReaded == -1) {
            perror("Error in read\n");
            exit(EXIT_FAILURE);
        }

        char msg[30];
        int length = snprintf(msg,sizeof(msg),"Child read from pipe -> %d\n",inputPipe);
        write(STDOUT_FILENO,msg,length);
        close(fd[0]);
        exit(EXIT_SUCCESS);
    }
    close(fd[0]);

    pid_t parentPid = getpid();
    char msg[20];
    int length = snprintf(msg,sizeof(msg),"Parent PID -> %d\n",parentPid);
    write(STDOUT_FILENO,msg,length);

    write(fd[1],&parentPid,sizeof(pid_t));
    close(fd[1]);
    wait(NULL);
    return 0;
}