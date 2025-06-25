#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>

volatile sig_atomic_t win = 1 ;

void handler_alarm(int sig, siginfo_t *info, void * context) {
    win = 0;
}

int main(){ 
    struct sigaction act;
    sigset_t mask, pending;

    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGUSR2);
    sigprocmask(SIG_BLOCK,&mask,0);

    memset(&act,0,sizeof(struct sigaction));
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask,SIGUSR1);
    sigaddset(&act.sa_mask,SIGUSR2);

    act.sa_sigaction = handler_alarm;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGALRM,&act,NULL);

    alarm(20);
    char msg[] = "Enter a sentece of your choice: ";
    write(STDOUT_FILENO,msg,sizeof(msg));

    char input[200];
    fgets(input,sizeof(input),stdin);
    alarm(0);

    while (win) {
        printf("Your fast enough...\n");

        sigpending(&pending);
        if(sigismember(&pending,SIGUSR1) || sigismember(&pending,SIGUSR2)) {
            printf("O programa tem um sinal SIGUSR1/2 pending...\n");
        }
        return EXIT_SUCCESS;
    }

    printf("\n Too slow, try next time...\n");

    return 0;
}