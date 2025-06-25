#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUM_ELEM_PER_VEC 1000
#define NUM_CHILDS 5

void closeNotUsedPipes(int pipeArray[][2], int indexToExclude) {
    for (int i = 0; i < NUM_CHILDS; i++) {
        if (i != indexToExclude) {
            close(pipeArray[i][0]);
            close(pipeArray[i][1]);
        }
    }
}

int main() {
    int vec1[NUM_ELEM_PER_VEC] = {-1}, vec2[NUM_ELEM_PER_VEC] = {-1};
    int pipeArray[NUM_CHILDS][2] = {-1};

    for (int i = 0; i < NUM_CHILDS; i++) {
        if (pipe(pipeArray[i]) == -1) {
            perror("Error creating pipe\n");
            exit(EXIT_FAILURE);
        }
    }

    int pidArray[NUM_CHILDS] = {-1};
    int eachChild = NUM_ELEM_PER_VEC / NUM_CHILDS;

    for (int i = 0; i < NUM_CHILDS; i++) {
        int start = i * eachChild, end = start + eachChild;

        int pid = fork();
        pidArray[i] = pid;

        if (pid == -1) {
            perror("Error forking\n");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            closeNotUsedPipes(pipeArray, i);
            close(pipeArray[i][0]);

            for (int j = start; j < end; j++) {
                int sum = vec1[j] + vec2[j];
                write(pipeArray[i][1],&sum,sizeof(int));
            }

            close(pipeArray[i][1]);
            exit(EXIT_SUCCESS);
        }
    }

    for (int i = 0; i < NUM_CHILDS; i++) {
        wait(NULL);
    }

    int counter = 0;

    for (int i = 0; i < NUM_CHILDS; i++) {
        close(pipeArray[i][1]);
        int input = -1;
        while (read(pipeArray[i][0],&input,sizeof(int)) > 0) {
            counter++;
        }
        close(pipeArray[i][0]);
    }

    printf("Num of readed inputs : %d\n",counter);

    return 0;
}
