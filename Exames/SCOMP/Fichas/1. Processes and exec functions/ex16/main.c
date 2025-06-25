#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

#define PATH "/home/jorge-rodrigues/Desktop/2_ano/2_Semestre/Exames/SCOMP/Fichas/1. Processes and exec functions/ex16/ShellHistory.txt"

int main() {
    char msg[] = "WELCOME TO YOUR SHELL\n   to exit 'end'\n";
    write(STDOUT_FILENO, msg, strlen(msg));

    while (1) {
        char output[] = "Enter a command to execute: ";
        write(STDOUT_FILENO, output, strlen(output));

        char inputCommand[30];
        ssize_t len = read(STDIN_FILENO, inputCommand, sizeof(inputCommand) - 1);
        inputCommand[len - 1] = '\0'; // remove '\n'

        if (strcmp(inputCommand, "end") == 0) {
            break;
        }

        int pid = fork();

        if (pid == -1) {
            perror("Error forking");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            // FILHO
            // Abre o ficheiro de histórico
            int fd = open(PATH, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0) {
                perror("Erro ao abrir o ficheiro");
                exit(EXIT_FAILURE);
            }

            // Redireciona stdout e stderr para o ficheiro
            dup2(fd, STDOUT_FILENO);
            dup2(fd, STDERR_FILENO);
            close(fd);

            // Executa o comando com o shell (para que o redirecionamento funcione)
            execlp("sh", "sh", "-c", inputCommand, (char *)NULL);

            // Se execlp falhar
            perror("execlp falhou");
            exit(EXIT_FAILURE);
        }

        wait(NULL);
    }

    printf("Closing MyShell...\n");
    return 0;
}
