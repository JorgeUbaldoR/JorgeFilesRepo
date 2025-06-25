#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>

#define NUM_CHILDS 2

int main() {
    int pipeDown[2]= {-1},pipeMiddle[2]= {-1}, pipeUp[2] = {-1};

    if (pipe(pipeUp) == -1 || pipe(pipeDown) == -1 || pipe(pipeMiddle) == -1) {
        perror("Error creatinf pipes\n");
        exit(EXIT_FAILURE);
    }

    int pid = -1;

    for (int i = 0; i < NUM_CHILDS; i++) {
        pid = fork();

        if (pid == -1) {
            perror("Error fork\n");
            exit(EXIT_FAILURE);
        }

        if (pid == 0 && i == 0) {
            close(pipeDown[1]);
            close(pipeMiddle[0]);
            dup2(pipeMiddle[1],STDOUT_FILENO);
            close(pipeMiddle[1]);
            close(pipeUp[0]);
            close(pipeUp[1]);

            char outputPipedDown[PIPE_BUF];
            FILE * file = fopen("helperchild1.txt","w+");
            if (file == NULL) {
                perror("Error opening file\n");
                exit(EXIT_FAILURE);
            }

            while (read(pipeDown[0],outputPipedDown,PIPE_BUF) > 0) {
                fprintf(file,outputPipedDown);
            }

            fclose(file);
            close(pipeDown[0]);

            execlp("sort","sort","helperchild1.txt",(char *)NULL);

            perror("Error executing sort\n");
            exit(EXIT_FAILURE);
        }else if (pid == 0) {
            close(pipeDown[0]);
            close(pipeDown[1]);
            close(pipeMiddle[1]);
            dup2(pipeUp[1],STDOUT_FILENO);
            close(pipeUp[1]);

            FILE * file = fopen("helperchild2.txt","w+");
            if (file == NULL) {
                perror("Error opening file\n");
                exit(EXIT_FAILURE);
            }

            char outputPipedMiddle[PIPE_BUF];
            while (read(pipeMiddle[0],outputPipedMiddle,PIPE_BUF) > 0) {
                fprintf(file,outputPipedMiddle);
            }
            close(pipeMiddle[0]);

            execlp("sh","sh","man","helperchild2.txt","|","tr","'a-z'","A-Z",(char *)NULL);
            perror("Error executing sh\n");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
