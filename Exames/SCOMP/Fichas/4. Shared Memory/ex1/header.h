//
// Created by jorge-rodrigues on 22-06-2025.
//

#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>       // printf, perror, etc.
#include <stdlib.h>      // exit, EXIT_FAILURE, etc.
#include <unistd.h>      // ftruncate, close
#include <fcntl.h>       // O_CREAT, O_RDWR, shm_open
#include <sys/mman.h>    // mmap, munmap, PROT_*, MAP_*
#include <sys/stat.h>    // Permissões (S_IRUSR, S_IWUSR, etc.)

#define SHM_NAME "/ex1shm"
typedef struct {
    int number;
    char name[80];
    char addres[80];

    int flag_read;
    int flag_write;
}STUDENT_T;

#endif //HEADER_H
