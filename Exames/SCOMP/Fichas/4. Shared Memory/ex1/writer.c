#include "header.h"


int main() {
    int fd_student, size = sizeof(STUDENT_T);
    STUDENT_T *student;

    fd_student = shm_open(SHM_NAME,O_CREAT|O_EXCL|O_RDWR,S_IRUSR|S_IWUSR);

    if (fd_student == -1) {
        perror("Error opening shared memory segment");
        exit(EXIT_FAILURE);
    }

    int i = ftruncate(fd_student, size);
    if (i == -1) {
        perror("Error truncating shared memory segment");
        exit(EXIT_FAILURE);
    }

    student = (STUDENT_T *) mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_SHARED,fd_student,0);
    if (student == MAP_FAILED) {
        perror("Error mapping shared memory segment");
        exit(EXIT_FAILURE);
    }


    if (!student->flag_read) {
        char msg[80];
        int length = snprintf(msg,sizeof(msg),"Enter the number of the student: ");
        write(STDOUT_FILENO,msg,length);
        scanf("%d",&student->number);

         length = snprintf(msg,sizeof(msg),"Enter the name of the student: ");
        write(STDOUT_FILENO,msg,length);
        scanf("%s",student->name);

        while (getchar() != '\n');

        length = snprintf(msg,sizeof(msg),"Enter the address of the student: ");
        write(STDOUT_FILENO,msg,length);
        fgets(student->addres,sizeof(student->addres),stdin);

        length = snprintf(msg,sizeof(msg),"All information have been readed succesfully\n");
        write(STDOUT_FILENO,msg,length);
        student->flag_read = 1;
    }

    while (!student->flag_read) {
        printf("Writer waiting of a reader...\n");
        sleep(8);
    }

    munmap(student,size);
    close(fd_student);
    shm_unlink(SHM_NAME);
    return EXIT_SUCCESS;
}
