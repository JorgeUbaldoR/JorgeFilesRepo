#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>

void chnagesCharInString(char *string) {
   for (int i = 0; i < strlen(string); i++) {
       if (string[i] >= 'A' && string[i] <= 'Z') {
           string[i] = string[i] + 32;
       }else if (string[i] >= 'a' && string[i] <= 'z') {
           string[i] = string[i] - 32;
       }
   }
}

int main(){
    int pipeUp[2], pipeDown[2];

    if(pipe(pipeUp) == -1 || pipe(pipeDown) == -1){
        perror("Error creating pipes...\n");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if(pid == -1){
        perror("Error forking \n");
        exit(EXIT_FAILURE);
    }

    if(pid == 0){
        close(pipeUp[0]);
        close(pipeDown[1]);

        char msgInput[] = "Enter a message: ";
        write(STDOUT_FILENO,msgInput,sizeof(msgInput));
        char msgRecived[PIPE_BUF];
        fgets(msgRecived,sizeof(msgRecived),stdin);

        write(pipeUp[1],msgRecived,sizeof(msgRecived));
        read(pipeDown[0],msgRecived,sizeof(msgRecived));

        printf("Conversion : %s\n",msgRecived);

        close(pipeUp[1]);
        close(pipeDown[0]);
        exit(EXIT_SUCCESS);
    }
    close(pipeUp[1]);
    close(pipeDown[0]);

    char msgRecived[PIPE_BUF];
    read(pipeUp[0],msgRecived,sizeof(msgRecived));

    chnagesCharInString(msgRecived);

    write(pipeDown[1],msgRecived,sizeof(msgRecived));

    close(pipeUp[0]);
    close(pipeDown[1]);
    return 0;
}
