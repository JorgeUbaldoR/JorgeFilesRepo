#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

typedef struct {
    int num1;
    int num2;

    volatile sig_atomic_t flag_parent;
    volatile sig_atomic_t flag_child;
    volatile sig_atomic_t flag_childdone;
    volatile sig_atomic_t flag_parentdone;
} NUMBERS_T;

const int REPETITIONS = 1000000;

#define PATH_SHM "/shmex2"

int main() {
    int fd_numbers, size = sizeof(NUMBERS_T);
    NUMBERS_T *numbers_shared;

    fd_numbers = shm_open(PATH_SHM,O_CREAT | O_EXCL | O_RDWR,S_IWUSR | S_IRUSR);

    if (fd_numbers == -1) {
        perror("Error oppening share memory");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(fd_numbers, size) == -1) {
        perror("Error truncating share memory");
        exit(EXIT_FAILURE);
    }

    numbers_shared = (NUMBERS_T *) mmap(NULL, size,PROT_READ | PROT_WRITE,MAP_SHARED, fd_numbers, 0);

    if (numbers_shared == MAP_FAILED) {
        perror("Error mapping shared memory");
        exit(EXIT_FAILURE);
    }

    numbers_shared->num1 = 10000;
    numbers_shared->num2 = 500;
    numbers_shared->flag_parent = 1;

    int pid = fork();

    if (pid == -1) {
        perror("Error forking");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        for (int iteretation = 0; iteretation < REPETITIONS; iteretation++) {
            while (!numbers_shared->flag_child) {
            }
            numbers_shared->num2--;
            numbers_shared->num1++;

            if (iteretation == REPETITIONS - 1) {
                numbers_shared->flag_childdone = 1;
            }

            numbers_shared->flag_child = 0;
            numbers_shared->flag_parent = 1;
        }

        munmap(numbers_shared,size);
        close(fd_numbers);
        exit(EXIT_FAILURE);
    }

    for (int iteretation = 0; iteretation < REPETITIONS; iteretation++) {
        while (!numbers_shared->flag_parent){}
        numbers_shared->num1--;
        numbers_shared->num2++;


        if (iteretation == REPETITIONS - 1) {
            numbers_shared->flag_parentdone = 1;
        }

        numbers_shared->flag_parent = 0;
        numbers_shared->flag_child = 1;

    }

    while ((!numbers_shared->flag_childdone && !numbers_shared->flag_parentdone)
        || (!numbers_shared->flag_childdone && numbers_shared->flag_parentdone)
        || (numbers_shared->flag_childdone && !numbers_shared->flag_parentdone)) {}


    printf("Number 1 -> %d\n",numbers_shared->num1);
    printf("Number 2 -> %d\n",numbers_shared->num2);

    munmap(numbers_shared,size);
    close(fd_numbers);
    shm_unlink(PATH_SHM);
    exit(EXIT_SUCCESS);
}
