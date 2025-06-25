#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define NUM_ELEM_ARRAY 1000
#define NUM_CHILDS 5

void populateArray(int array[]) {
    srand(time(NULL)*getpid());
    for (int i = 0; i < NUM_ELEM_ARRAY; i++) {
        array[i] = rand() % 100 + 1;
    }
}

int main(){
    int array1[NUM_ELEM_ARRAY] = {0}, array2[NUM_ELEM_ARRAY] = {0};
    populateArray(array1);
    populateArray(array2);
    int eachChild = NUM_ELEM_ARRAY/NUM_CHILDS;

    int pipeUp[2];

    if(pipe(pipeUp) == -1){
        perror("Errro creating pipe\n");
        exit(EXIT_FAILURE);
    }

    pid_t childs[NUM_CHILDS] = {-1};

    for(int childNum = 0; childNum < NUM_CHILDS; childNum++){
        childs[childNum] = fork();
        
        int start = childNum * eachChild ;
        int end = start + eachChild;

        if(childs[childNum] == -1){
            perror("Error forking\n");
            exit(EXIT_FAILURE);
        }

        if(childs[childNum] == 0){
            close(pipeUp[0]);

            int sum = 0;
            for(int i = start; i < end; i++){
                sum += array1[i] + array2[i];
            }

            char msg[80];
            int lenght = snprintf(msg,sizeof(msg),"Child numeber -> %d | Total sum : %d\n",childNum,sum);
            write(STDOUT_FILENO,msg,lenght);

            write(pipeUp[1],&sum,sizeof(int));

            close(pipeUp[1]);
            exit(EXIT_FAILURE);
        }
    }   

    close(pipeUp[1]);
        int finalSum = 0;
    
        for(int i = 0; i < NUM_CHILDS; i++){
            int readed;
            int bytes = read(pipeUp[0],&readed,sizeof(readed));

            if(bytes == -1){
                perror("Error reading from pipe\n");
                exit(EXIT_FAILURE);
            }
            
            finalSum +=readed;
        }

    close(pipeUp[0]);
    printf("\nFinal result -> %d\n",finalSum);
    return 0;
}