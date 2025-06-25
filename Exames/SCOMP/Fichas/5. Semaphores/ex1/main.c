#include <semaphore.h>   // sem_t, sem_init, sem_wait, sem_post, sem_destroy
#include <fcntl.h>       // O_CREAT, O_EXCL (se usares semáforos nomeados)
#include <sys/stat.h>    // Permissões (ex: 0666)
#include <unistd.h>      // sleep(), fork(), close(), etc.
#include <stdio.h>       // printf, perror
#include <stdlib.h>      // exit, EXIT_FAILURE
#include <sys/wait.h>


const int NUM_CHILDS = 5;
#define NUMS_EACH 20
#define SEMAPHORE_NAME "semaex1"


int main() {
    int pid;
    sem_unlink(SEMAPHORE_NAME);

    sem_t *sem = sem_open(SEMAPHORE_NAME,O_CREAT|O_EXCL,0644,1);

    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    for (int i = 0 ; i < NUM_CHILDS;i++) {
        pid = fork();

        int start = i* NUMS_EACH, end = start + NUMS_EACH;

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            FILE * file = fopen("info.txt","a");

            for (int j = start ; j < end ; j++) {
                sem_wait(sem);
                fprintf(file,"%d\n",j);
                sem_post(sem);

            }

            fclose(file);
            exit(EXIT_SUCCESS);
        }
    }

    for (int i = 0 ; i < NUM_CHILDS ; i++) {
        wait(NULL);
    }

    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        printf("All childs already write their numbers -> Parent set to shopw them...");
        sleep(5);
        execlp("more","more","info.txt",(char *)NULL);

        perror("execlp");
        exit(EXIT_FAILURE);
    }

    wait(NULL);
    printf("------- FINISH\n");

    sem_unlink(SEMAPHORE_NAME);
    return EXIT_SUCCESS;
}
