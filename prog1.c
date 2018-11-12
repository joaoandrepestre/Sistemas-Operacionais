#include <stdio.h>
#include<stdlib.h>
#include <wait.h>
#include <unistd.h>

int main(void)
{
    int status, id, j;
    int fork_ret;
    int pipe1[2];
    int pipe2[2];

    pipe(pipe1);
    pipe(pipe2);

    fork_ret = fork();
    id = getpid();
    printf("PID do processo corrente: %d\n\n", id);

    if (fork_ret>0) {
        printf("Processo pai sendo executado\n\n");

        printf("PAI: PID atual: %d\n     PID filho: %d\n\n",id,fork_ret);

        char msg_envia[30] = "Mensagem do pai para o filho.";
        close(pipe1[0]);
        write(pipe1[1],msg_envia,30);
        close(pipe1[1]);
        printf("PAI: Mensagem mandada: %s\n\n", msg_envia);

        char msg_recebe[30];
        close(pipe2[1]);
        read(pipe2[0],msg_recebe,30);
        close(pipe2[0]);
        printf("PAI: Resposta recebida: %s\n\n", msg_recebe);

        wait(&status);
        printf("PAI: Processo filho terminado, processo pai também ira terminar.\n\n");

    } else {
        printf("Processo filho sendo executado\n\n");

        int ppid = getppid();
        printf("FILHO: PID atual: %d\n       PID pai: %d\n\n",id,ppid);

        char msg_recebe[30];
        close(pipe1[1]);
        read(pipe1[0], msg_recebe,30);
        close(pipe1[0]);
        printf("FILHO: Mensagem recebida: %s\n\n",msg_recebe);

        char msg_envia[30] = "Mensagem do filho para o pai.";
        close(pipe2[0]);
        write(pipe2[1],msg_envia,30);
        printf("FILHO: Resposta enviada: %s\n\n", msg_envia);
        
        for (j = 0; j <= 10000; j++);
        char msg[12];
        sprintf(msg,"%d",j);
        write(pipe2[1],msg,12);
        printf("FILHO: Mensagem enviada: %s\n\n", msg);
        close(pipe2[1]);

        execl("/Bin/ls", "ls", NULL);
        printf("\nFILHO: Se execl não falhar, essa mensagem não aparecerá no console.\n");
        printf("FILHO: Como 'bin' está escrito incorretamente, execl falhará e essa mensagem aparecerá no concole.\n\n");
    }
    exit(0);
}