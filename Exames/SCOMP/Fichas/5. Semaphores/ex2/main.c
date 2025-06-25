#include <semaphore.h>   // sem_t, sem_init, sem_wait, sem_post, sem_destroy
#include <fcntl.h>       // O_CREAT, O_EXCL (se usares semáforos nomeados)
#include <sys/stat.h>    // Permissões (ex: 0666)
#include <unistd.h>      // sleep(), fork(), close(), etc.
#include <stdio.h>       // printf, perror
#include <stdlib.h>      // exit, EXIT_FAILURE
#include <sys/wait.h>


const int NUM_CHILDS = 5;
#define NUMS_EACH 20
#define SEMAPHORE_NAME1 "semaex2_child1"
#define SEMAPHORE_NAME2 "semaex2_child2"
#define SEMAPHORE_NAME3 "semaex2_child3"
#define SEMAPHORE_NAME4 "semaex2_child4"
#define SEMAPHORE_NAME5 "semaex2_child5"


int main() {
    int pid;

    sem_t *sem1 = sem_open(SEMAPHORE_NAME1,O_CREAT|O_EXCL,0644,1);
    sem_t *sem2 = sem_open(SEMAPHORE_NAME2,O_CREAT|O_EXCL,0644,0);
    sem_t *sem3 = sem_open(SEMAPHORE_NAME3,O_CREAT|O_EXCL,0644,0);
    sem_t *sem4 = sem_open(SEMAPHORE_NAME4,O_CREAT|O_EXCL,0644,0);
    sem_t *sem5 = sem_open(SEMAPHORE_NAME5,O_CREAT|O_EXCL,0644,0);

    if (sem1 == SEM_FAILED || sem2 == SEM_FAILED || sem3 == SEM_FAILED || sem4 == SEM_FAILED || sem5 == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }
    FILE * file = fopen("info.txt","w");

    sem_t *semaArray[]= {sem1,sem2,sem3,sem4,sem5};

    for (int i = 0 ; i < NUM_CHILDS;i++) {
        pid = fork();

        int start = i* NUMS_EACH, end = start + NUMS_EACH;

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            sem_wait(semaArray[i]);
            FILE * file = fopen("info.txt","a");

            for (int j = start ; j < end ; j++) {
                fprintf(file,"%d\n",j);

            }

            fclose(file);
            sem_post(semaArray[i+1]);
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

    sem_unlink(SEMAPHORE_NAME1);
    sem_unlink(SEMAPHORE_NAME2);
    sem_unlink(SEMAPHORE_NAME3);
    sem_unlink(SEMAPHORE_NAME4);
    sem_unlink(SEMAPHORE_NAME5);
    return EXIT_SUCCESS;
}
