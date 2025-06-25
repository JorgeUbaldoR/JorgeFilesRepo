#include <semaphore.h>   // sem_t, sem_init, sem_wait, sem_post, sem_destroy
#include <fcntl.h>       // O_CREAT, O_EXCL (se usares semáforos nomeados)
#include <sys/stat.h>    // Permissões (ex: 0666)
#include <unistd.h>      // sleep(), fork(), close(), etc.
#include <stdio.h>       // printf, perror
#include <stdlib.h>      // exit, EXIT_FAILURE
#include <wait.h>

#define NUM_REPETITIONS 10

int main() {
    sem_t * sem_child = sem_open("semaex5",O_CREAT,0644,0);
    sem_t * sem_parent = sem_open("semaex5parent",O_CREAT,0644,1);

    if (sem_child == SEM_FAILED || sem_parent == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    int pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        for (int i = 0; i < NUM_REPETITIONS; i++) {
            sem_wait(sem_child);

            printf("Im the child\n");

            sem_post(sem_parent);
        }

        exit(EXIT_SUCCESS);
    }

    for (int i = 0 ; i < NUM_REPETITIONS ; i++) {
        sem_wait(sem_parent);

        printf("Im the parent\n");

        sem_post(sem_child);
    }

    wait(NULL);

    sem_unlink("semaex5");
    sem_unlink("semaex5parent");
    return EXIT_SUCCESS;
}
