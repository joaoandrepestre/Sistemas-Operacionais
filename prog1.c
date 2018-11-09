#include <stdio.h>
#include <wait.h>
#include <unistd.h>

int main(void)
{
    int status, id, j;

    status = fork();
    id = getpid();
    printf("PID do processo corrente: %d\n\n", id);

    if (status!=0) {
        printf("Processo pai sendo executado\n\n");

        printf("PAI: PID atual: %d\nPID filho: %d\n\n",id,status);

        ***** Monte uma mensagem e a envie para o processo filho
        ***** Mostre na tela o texto da mensagem enviada
        ***** Aguarde a resposta do processo filho
        ***** Mostre na tela o texto recebido do processo filho
        ***** Aguarde mensagem do filho e mostre o texto recebido

        wait();
        ***** Aguarde o término do processo filho
        printf("PAI: Processo filho terminado, processo pai também ira terminar.\n\n");

    } else {
        printf("Processo filho sendo executado\n\n");

        int ppid = getppid();
        printf("FILHO: PID atual: %d\nPID pai: %d\n\n",id,ppid);
        ***** Aguarde a mensagem do processo pai e ao receber mostre o texto na tela
        ***** Envie uma mensagem resposta ao pai
        
        for (j = 0; j <= 10000; i++){
            ***** Envie mensagem ao processo pai com o valor final de “j”
        }

        execl("/Bin/ls", "ls", NULL);
        printf("\nFILHO: Se execl não falhar, essa mensagem não aparecerá no console.\n");
        printf("FILHO: Como 'bin' está escrito incorretamente, execl falhará e essa mensagem aparecerá no concole.\n\n");
    }
    exit(0);
}