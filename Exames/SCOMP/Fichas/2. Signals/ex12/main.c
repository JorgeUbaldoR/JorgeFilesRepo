#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>


void handler_sigusr1(int sig, siginfo_t * info, void * context){
    char msg[100];
    int length = snprintf(msg,sizeof(msg),"I captured a SIGUSR1 sent by the process with PID %d\n",info->si_pid);
    write(STDOUT_FILENO,msg,length);
}

int main(void){
    printf("PROGRAM PID -> %d\n\n",getpid());
    struct sigaction act;
    sigset_t mask,pending;

    sigfillset(&mask);
    sigdelset(&mask,SIGUSR1);
    sigdelset(&mask,SIGINT);
    sigprocmask(SIG_BLOCK,&mask,NULL);

    memset(&act,0,sizeof(struct sigaction));

    sigfillset(&act.sa_mask);
    act.sa_sigaction = handler_sigusr1;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1,&act,NULL);

    do {
        char msg[100];
        int length = snprintf(msg,sizeof(msg),"If you want to continue send a SIGUSR1 SIGNAL to the pid: %d\n", getpid());
        write(STDOUT_FILENO,msg,length);
        sleep(20);
        sigpending(&pending);
    }while (sigismember(&pending,SIGUSR2));

    printf("Program finished...\n");
    return 0;
}