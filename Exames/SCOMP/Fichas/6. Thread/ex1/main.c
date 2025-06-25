#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void * func_first_name(void * arg) {
    char * first_name = (char*)arg;
    char msg[100];
    int length = snprintf(msg,sizeof(msg),"%s\n",first_name);
    write(STDOUT_FILENO,msg,length);
    free(first_name);
    pthread_exit(NULL);
}

void * func_second_name(void * arg) {
    char *second_name = (char *) arg;
    char msg[100];
    int length = snprintf(msg,sizeof(msg),"%s\n",second_name);
    write(STDOUT_FILENO,msg,length);
    free(second_name);
    pthread_exit(NULL);
}


int main() {
    pthread_t thread[2];
    char tempfisrtName[20], ltempastName[20];

    char msg[80];

    int length = snprintf(msg,sizeof(msg),"Insert your first name: ");
    write(STDOUT_FILENO,msg,length);
    scanf("%s",tempfisrtName);

    length = snprintf(msg,sizeof(msg),"Insert your last name: ");
    write(STDOUT_FILENO,msg,length);
    scanf("%s",ltempastName);

    char *firstName = malloc(strlen(tempfisrtName) +1);
    char *lastName = malloc(strlen(tempfisrtName) +1);

    strcpy(firstName,tempfisrtName);
    strcpy(lastName,ltempastName);

    pthread_create(&thread[0],NULL,func_first_name,firstName);
    pthread_create(&thread[1],NULL,func_second_name,lastName);

    for (int i = 0; i < 2; i++) {
        pthread_join(thread[i],NULL);
    }

    printf("\nAll threads finished...\n");
    return 0;
}
