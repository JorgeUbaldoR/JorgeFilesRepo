#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



int main(){
    int fd[2];

    if(pipe(fd) == -1){
        perror("Error creating pipe");
        exit(EXIT_FAILURE);
    }
    int inputInt = -1;
    printf("Enter a number of your choice: ");
    scanf("%d",&inputInt);

    char inputName[100];
    printf("Enter a name of your choice: ");
    scanf("%s",inputName);

    pid_t pid = fork();

    if (pid == -1) {
        perror("Error forking");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        close(fd[1]);
        int readIntPipe;

        if (read(fd[0],&readIntPipe,sizeof(int)) == -1) {
            perror("Error reading from pipe");
            exit(EXIT_FAILURE);
        }

        printf("Number entered : %d\n",readIntPipe);

        char readName[100];
        if (read(fd[0],readName,sizeof(readName)) == -1) {
            perror("Error reading from pipe");
            exit(EXIT_FAILURE);
        }

        printf("Name entered : %s\n",readName);
        close(fd[0]);
        exit(EXIT_SUCCESS);
    }

    close(fd[0]);
    write(fd[1],&inputInt,sizeof(int));
    write(fd[1],&inputName,sizeof(inputName));
    close(fd[1]);
    return 0;
}