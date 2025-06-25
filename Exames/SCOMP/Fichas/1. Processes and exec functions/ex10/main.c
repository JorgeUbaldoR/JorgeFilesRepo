#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>


#define SIZE_ARRAY 100000
#define NUM_CHILDS 5

int main(){
    srand(time(NULL)*getpid());
    int array[SIZE_ARRAY]= {0};

    for(int i = 0; i < SIZE_ARRAY; i++){
        array[i] = i;
    }

    int pids[NUM_CHILDS];
    int find = rand() % SIZE_ARRAY;
    printf("NUMBER TO FIND : %d\n",find);

    for(int i = 0; NUM_CHILDS > i;i++){
        int start = i*(SIZE_ARRAY/NUM_CHILDS);
        int end = start + (SIZE_ARRAY/NUM_CHILDS);

        pids[i] = fork();

        if(pids[i] == -1){
            perror("Error forking\n");
            exit(EXIT_FAILURE);
        }

        if(pids[i] == 0){
            
            while(start < end){
                if(array[start] == find){
                    char msg[60];
                    int length = snprintf(msg,sizeof(msg),"Child [%d] number %d found number in position %d\n",getpid(),i+1,start);
                    write(STDOUT_FILENO,msg,length);
                    exit(i+1);
                }
                start++;
            }
            char msg[50];
            int length = snprintf(msg,sizeof(msg),"Child [%d] number %d DO NOT found number\n",getpid(),i+1);
            write(STDOUT_FILENO,msg,length);
            exit(0);
        }
    }

    int pid;
    int status;
    for(int i = 0; i < NUM_CHILDS; i++){

        pid = wait(&status);
        int exitstatus = WEXITSTATUS(status);
        if(WIFEXITED(status) && exitstatus != 0){
            printf("FATHER - Child that found the number: %d with pid %d\n",exitstatus,pid);
            
            printf("Killing all childs...");
            for(int child = 0; child < NUM_CHILDS; child++){
                if(pids[child] != pid){
                    kill(pids[child],SIGKILL);
                }
            }
            return 0;
        }
       printf("!!!FATHER - Child number %d with pid %d DO NOT found the number\n",exitstatus,pid);
    }



    return 0;
}