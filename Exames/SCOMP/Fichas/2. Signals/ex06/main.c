#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>

typedef struct{
    char cmd[32];
    int time;
}command_t;

volatile sig_atomic_t pidWorking = 0;

void handler_sigusr1(int sig, siginfo_t * info, void * context){
    char msg[] = "Time exedded";
    write(STDOUT_FILENO,msg,sizeof(msg));
    raise(SIGKILL);
}

void handler_alarm(int sig, siginfo_t * info, void * context){
    char msg[] = "Time exedded";
    write(STDOUT_FILENO,msg,sizeof(msg));
    kill(pidWorking,SIGUSR1);
}

int main(){
    command_t listOfCommands[] = {
        {"ls -la", 2},
        {"date", 1},
        {"whoami", 1},
        {"uname -r", 1},
        {"ps aux", 2}
    };

    int numCommands = sizeof(listOfCommands)/sizeof(command_t);
    int pid;

    for(int i = 0; i < numCommands; i++){
        pid = fork();

        if(pid == -1){
            perror("Error forking\n");
            exit(EXIT_FAILURE);
        }

        if(pid == 0){
            struct sigaction act;

            memset(&act,0,sizeof(struct sigaction));
            act.sa_sigaction = handler_sigusr1;
            act.sa_flags = SA_SIGINFO;

            sigfillset(&act.sa_mask);
            sigaction(SIGUSR1,&act,NULL);


            char *args[10];
            int j = 0;

            char *token = strtok(listOfCommands[i].cmd," ");
            while(token != NULL && j < 10){
                args[j++] = token;
                token = strtok(NULL," ");
            }

            args[j] = NULL;

            char msg[80];
            int length = snprintf(msg,sizeof(msg),"Child will now execute command -> %s within the time '%d'\n",listOfCommands[i].cmd,listOfCommands[i].time);
            write(STDOUT_FILENO,msg,length);

            raise(SIGSTOP);
            execvp(args[0],args);


            length = snprintf(msg,sizeof(msg),"Something wnet wrong triying to execute the command -> %s\n",listOfCommands[i].cmd);
            write(STDOUT_FILENO,msg,length);
            exit(EXIT_FAILURE);
        }
        pidWorking = pid;
        struct sigaction act;
        memset(&act,0,sizeof(struct sigaction));
        act.sa_sigaction = handler_alarm;
        act.sa_flags = SA_SIGINFO;
        sigaction(SIGALRM,&act,NULL);

        int status;
        waitpid(pid,&status,WUNTRACED);
        if(WIFSTOPPED(status)){
            alarm(listOfCommands[i].time);
            kill(pid,SIGCONT);
        }
        wait(NULL);
        sleep(7);
    }

    for(int i = 0; i < numCommands;i++){
        wait(NULL);
    }

    printf("PARENT --- All commands executed...\n");

    return 0;
}