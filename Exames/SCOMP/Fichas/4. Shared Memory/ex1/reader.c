#include "header.h"


int main() {
    int fd_student,size = sizeof(STUDENT_T);
    STUDENT_T *student;

    fd_student = shm_open(SHM_NAME,O_RDWR,S_IRUSR|S_IWUSR);
    while (fd_student < 0) {
        char msg[] = "Waiting for writer to open the shred memory...\n";
        write(STDOUT_FILENO,msg,sizeof(msg));
        sleep(5);
        fd_student = shm_open(SHM_NAME,O_RDWR,S_IRUSR|S_IWUSR);
    }

    student = (STUDENT_T *) mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,fd_student,0);

    if (student == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }

    while (student->flag_read == 0) {
        printf("Waiting for writer to write...\n");
        sleep(5);
    }

    printf("------------- STUDENT INFORMATION -------------\n");
    printf("- Number  : %d\n",student->number);
    printf("- Name    : %s\n",student->name);
    printf("- Address : %s\n",student->addres);

    student->flag_write = 1;

    munmap(student,size);
    close(fd_student);
    return 0;
}