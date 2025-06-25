#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int main(){
    int fd[2];

    if(pipe(fd) == -1){
        perror("Error creating pipe\n");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if(pid == -1){
        perror("Error forking\n");
        exit(EXIT_FAILURE);
    }

    if(pid == 0){
        close(fd[1]);

        char line[200];

        while(read(fd[0],line,sizeof(line)) > 0){
            printf("%s\n\n",line);
        }

        close(fd[0]);
        exit(EXIT_SUCCESS);
    }

    close(fd[0]);

    FILE *file = fopen("./info.txt","r");

    if(file == NULL){
        perror("Error opening file\n");
        kill(pid,SIGKILL);
        exit(EXIT_FAILURE);
    }

    char readed[200];
    while(fgets(readed, sizeof(readed), file) > 0){
        write(fd[1],readed,sizeof(readed));
    }

    close(fd[1]);
    return 0;
}