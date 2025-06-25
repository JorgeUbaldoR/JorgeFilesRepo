#include <time.h>
#include "header.h"


int main() {
    srand(time(NULL) * getpid());
    int fd, size_share_memory = sizeof(ARRAY_T);
    ARRAY_T *array;

    fd = shm_open(SHM_NAME,O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);

    if (fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(fd,size_share_memory) == -1) {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    array = (ARRAY_T *) mmap(NULL,size_share_memory,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

    if (array == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    int numElements = sizeof(array->vec)/sizeof(array->vec[0]);
    for (int i = 0; i < numElements; i++) {
        array->vec[i] = rand() % 20 + 1;
    }

    array->flag_new_data = 1;

    char msg[80] = "Waiting for reader to read...";
    while (array->flag_new_data == 1) {
        write(STDOUT_FILENO,msg,strlen(msg));
        sleep(5);
    }

    int length = snprintf(msg,sizeof(msg),"All information have been readed.\n");
    write(STDOUT_FILENO,msg,length);



    munmap(array,size_share_memory);
    close(fd);
    shm_unlink(SHM_NAME);
    return EXIT_SUCCESS;
}
