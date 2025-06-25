#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc,char *argvs[]){

    if(argc <= 1){
        perror("Error: No commands to execute\n");
        exit(EXIT_FAILURE);
    }

    int numChilds = argc -1;
    int pid;

    for(int i = 0; i < numChilds; i++){
        pid = fork();

        if(pid == -1){
            perror("Error forking\n");
            exit(EXIT_FAILURE);
        }

        if(pid == 0){
            char msg[80];
            int length = snprintf(msg,sizeof(msg),"Child %d will now execute command %s.\n",i,argvs[i+1]);
            write(STDOUT_FILENO,msg,length);

            execlp(argvs[i+1],argvs[i+1],(char*)NULL);

            length = snprintf(msg,sizeof(msg),"Something went wrong with child %d executecuting command %s.\n",i,argvs[i+1]);
            write(STDOUT_FILENO,msg,length);
            exit(EXIT_FAILURE);
        }
        
    }

    for(int i = 0; i < numChilds; i++){
        wait(NULL);
    }

    printf("Parent will finish now\n");
    return 0;
}