#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

int main(){
    
    printf("Insira o PATH completo do arquivo a fazer backup:");
    char pathOfFile[80];
    scanf("%s",pathOfFile);

    printf("Insira o PATH completo do arquivo onde deseja guardar o backup:");
    char pathToSave[80];
    scanf("%s",pathToSave);

    int pid = fork();

    if(pid == -1){
        perror("Error forking\n");
        exit(EXIT_FAILURE);
    }

    if(pid == 0){
        char msg[] = "Child will now execute the process to create the backup...\n";
        write(STDOUT_FILENO,msg,sizeof(msg));

        execlp("cp","cp",pathOfFile,pathToSave,(char*)NULL);

        char error[] = "Something went wrong triying to make the backup.\n";
        write(STDOUT_FILENO,error,sizeof(error));
        exit(EXIT_FAILURE);
    }

    wait(NULL);

    printf("Processo Backup concluído (verifique se foi possivel).\n");

    return 0;
}