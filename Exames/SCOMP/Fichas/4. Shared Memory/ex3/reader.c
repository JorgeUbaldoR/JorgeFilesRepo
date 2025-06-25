#include "header.h"




int main() {
    int fd, size = sizeof(ARRAY_T);
    ARRAY_T * array;

    fd = shm_open(SHM_NAME,O_RDWR,S_IRUSR|S_IWUSR);

    if (fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(fd,size) == -1) {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    array = (ARRAY_T *) mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);


    if (array == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }


    while (!array->flag_new_data) {
        char msg[]= "Waiting information to read...";
        write(STDOUT_FILENO,msg,strlen(msg));
    }

    int sum = 0;
    for (int i = 0 ; i < 10 ; i++) {
        sum += array->vec[i];
    }

    printf("Average of sum : %f\n",(float)sum/10);
    array->flag_new_data = 0;

    munmap(array,size);
    close(fd);
    return 0;
}
