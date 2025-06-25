#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

volatile sig_atomic_t recived_SIGUSR1 = 1;
volatile sig_atomic_t recived_SIGUSR2 = 1;

void handler_sigusr1(int sig, siginfo_t *si,void * context) {
    recived_SIGUSR1 = 0;
}

void handler_sigusr2(int sig, siginfo_t *si,void * context) {
    recived_SIGUSR2 = 0;
}

int main(){
    FILE * ficheiro = fopen("./data.txt","r");

    if (ficheiro == NULL) {
        perror("Error opening file\n");
        exit(EXIT_FAILURE);
    }

    struct sigaction act;
    sigset_t mask, pending;

    sigfillset(&mask);
    sigdelset(&mask, SIGUSR1);
    sigdelset(&mask, SIGUSR2);
    sigprocmask(SIG_BLOCK,&mask,NULL);

    memset(&act,0,sizeof(act));

    act.sa_sigaction = handler_sigusr1;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1,&act,NULL);


    int pid = fork();

    if (pid == -1) {
        perror("Error forking\n");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        struct sigaction act;

        memset(&act,0,sizeof(struct sigaction));

        act.sa_sigaction = handler_sigusr2;
        act.sa_flags = SA_SIGINFO;
        sigaction(SIGUSR2,&act,NULL);


        sigset_t mask, pending;

        sigfillset(&mask);
        sigdelset(&mask, SIGUSR1);
        sigdelset(&mask, SIGUSR2);
        sigprocmask(SIG_BLOCK,&mask,NULL);


        srand(time(NULL)*getpid());
        kill(getppid(),SIGUSR1);


        while(recived_SIGUSR2) {
            sigpending(&pending);
            if (sigismember(&pending,SIGINT)) {
                kill(0,SIGKILL);
            }
            sleep(5);
        }

        FILE *f = fopen("./data.txt","w");

        if (f == NULL) {
            perror("Error opening file\n");
            exit(EXIT_FAILURE);
        }

        fprintf(f, "%d\n", rand() % 100);
        fclose(f);

        kill(getppid(),SIGUSR1);
        exit(EXIT_SUCCESS);
    }


    while(recived_SIGUSR1) {
        sigpending(&pending);
        if (sigismember(&pending,SIGINT)) {
            kill(0,SIGKILL);
        }
        sleep(5);
    }
    recived_SIGUSR1 = 1;

    int numLido;
    fscanf(ficheiro,"%d",&numLido);
    printf("Lido = %d\n",numLido);
    kill(pid,SIGUSR2);

    while(recived_SIGUSR1) {
        sigpending(&pending);
        if (sigismember(&pending,SIGINT)) {
            kill(0,SIGKILL);
        }
        sleep(5);
    }

    wait(NULL);
    printf("Parent will finish now...");

    fclose(ficheiro);

    return 0;
}
