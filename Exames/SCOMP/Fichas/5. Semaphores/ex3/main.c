#include <semaphore.h>   // sem_t, sem_init, sem_wait, sem_post, sem_destroy
#include <fcntl.h>       // O_CREAT, O_EXCL (se usares semáforos nomeados)
#include <sys/stat.h>    // Permissões (ex: 0666)
#include <unistd.h>      // sleep(), fork(), close(), etc.
#include <stdio.h>       // printf, perror
#include <stdlib.h>      // exit, EXIT_FAILURE
#include <sys/wait.h>

#define NUM_CHILDS 5

int main(int argv, char *argc[]) {
    if (argv <= 1) {
        perror("Wrong number of arguments");
        exit(EXIT_FAILURE);
    }

    sem_t * sem = sem_open("/semex3",O_CREAT,0644,1);

    FILE * file = fopen(argc[1], "w");

    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }


    int pid = -1;
    for (int i = 0; i < NUM_CHILDS; i++) {
        pid = fork();

        if (pid == -1) {
            perror("Error forking");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            sem_wait(sem);
            FILE * file1 = fopen(argc[1], "w");

            fprintf(file1, "I am the process with PID %d\n",getpid());
            fclose(file1);

            sem_post(sem);
            exit(EXIT_SUCCESS);
        }
        sleep(2);
    }

    sem_unlink("/semex3");
    return EXIT_SUCCESS;
}
