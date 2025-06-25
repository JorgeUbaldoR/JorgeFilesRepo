#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>


#define PATH_SHM "/ex5"
const int NUM_ELEMENTS_IN_V = 1000;
const int NUM_CHILDS= 10;

typedef struct {
    int v[1000];
    int max[10];
}SHARED_T;

int main() {
    srand(time(NULL)*getpid());
    int fd, size_shm = sizeof(SHARED_T);
    SHARED_T *shared;

    fd = shm_open(PATH_SHM,O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);
    ftruncate(fd,size_shm);
    shared = (SHARED_T *) mmap(NULL,size_shm,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

    for (int i = 1 ; i < NUM_CHILDS; i++) {
        if (i != NUM_CHILDS -1) {
            printf("%d,",shared->max[i]);
        }else {
            printf("%d]\n",shared->max[i]);
        }
    }

    for (int i =0; i < NUM_ELEMENTS_IN_V; i++) {
        shared->v[i] = rand() % 100 +1;
    }

    int pid,each = NUM_ELEMENTS_IN_V/NUM_CHILDS;

    for (int i = 0 ; i < NUM_CHILDS; i++) {
        int start = i*each,end = start + each;
        pid = fork();

        if (pid == 0) {
            int greatestInChild = shared->v[start];
            for (int j = start+1; j < end; j++) {
                if (shared->v[j] > greatestInChild) {
                    greatestInChild = shared->v[j];
                }
            }

            shared->max[i]= greatestInChild;

            munmap(shared,size_shm);
            close(fd);
            exit(EXIT_SUCCESS);
        }
    }

    for (int i = 0 ; i < NUM_CHILDS; i++) {
        wait(NULL);
    }

    int greatest = shared->max[0];
    printf("[%d,",greatest);
    for (int i = 1 ; i < NUM_CHILDS; i++) {
        if (i != NUM_CHILDS -1) {
            printf("%d,",shared->max[i]);
        }else {
            printf("%d]\n",shared->max[i]);
        }

        if (greatest < shared->max[i]) {
            greatest = shared->max[i];
        }
    }

    printf("Greatest value found by PARENT is -> %d\n",greatest);

    munmap(shared,size_shm);
    close(fd);
    shm_unlink(PATH_SHM);
    return 0;
}
