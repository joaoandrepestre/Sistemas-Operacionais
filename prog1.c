#include <stdio.h>
#include <stdlib.h>
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

    // ********* Insira um comando para pegar o PID do processo corrente e mostre na
    //  tela da console.
    id = getpid();
    printf("PID do processo corrente: %d\n\n", id);

    // if (*** insira um comando para criar um subprocesso) { - talvez deva ser chamado direto no fork
    // ***** Faça com que o processo pai execute este trecho de código
    if (fork_ret>0) {
        printf("Processo pai sendo executado\n\n");

        // ***** Mostre na console o PID do processo pai e do processo filho
        printf("PAI: PID atual: %d\n     PID filho: %d\n\n",id,fork_ret);

        // ***** Monte uma mensagem e a envie para o processo filho
        char msg_envia[30] = "Mensagem do pai para o filho.";
        close(pipe1[0]);
        write(pipe1[1],msg_envia,30);
        close(pipe1[1]);

        // ***** Mostre na tela o texto da mensagem enviada
        printf("PAI: Mensagem mandada: %s\n\n", msg_envia);

        // ***** Aguarde a resposta do processo filho - ????
        
        // ***** Mostre na tela o texto recebido do processo filho - ???


        // ***** Aguarde mensagem do filho e mostre o texto recebido
        char msg_recebe[30];
        close(pipe2[1]);
        read(pipe2[0],msg_recebe,30);
        close(pipe2[0]);
        printf("PAI: Resposta recebida: %s\n\n", msg_recebe);

        // ***** Aguarde o término do processo filho
        wait(&status);
        // ***** Informe na tela que o filho terminou e que o processo pai também vai encerrarar
        // O pai não deveria ter um exit?
        printf("PAI: Processo filho terminado, processo pai também ira terminar.\n\n");

    } else {
        printf("Processo filho sendo executado\n\n");

        // ***** Faça com que o processo filho execute este trecho de código - fora de ordem
        
        //  ***** Mostre na tela o PID do processo corrente e do processo pai
        int ppid = getppid();
        printf("FILHO: PID atual: %d\n       PID pai: %d\n\n",id,ppid);

        // ***** Aguarde a mensagem do processo pai e ao receber mostre o texto na tela
        char msg_recebe[30];
        close(pipe1[1]);
        read(pipe1[0], msg_recebe,30);
        close(pipe1[0]);
        printf("FILHO: Mensagem recebida: %s\n\n",msg_recebe);

        // ***** Envie uma mensagem resposta ao pai
        char msg_envia[30] = "Mensagem do filho para o pai.";
        close(pipe2[0]);
        write(pipe2[1],msg_envia,30);
        printf("FILHO: Resposta enviada: %s\n\n", msg_envia);
        
        // ***** Execute o comando “for” abaixo
        for (j = 0; j <= 10000; j++);
        char msg[12];
        // ***** Envie mensagem ao processo pai com o valor final de “j”
        sprintf(msg,"%d",j);
        write(pipe2[1],msg,12);
        printf("FILHO: Mensagem enviada: %s\n\n", msg);
        close(pipe2[1]);

        // ***** Execute o comando abaixo e responda às perguntas
        execl("/Bin/ls", "ls", NULL);
        
        // ***** O que acontece após este comando?
        printf("\nFILHO: Se execl não falhar, essa mensagem não aparecerá no console.\n");
        // ***** O que pode acontecer se o comando “execl” falhar?
        printf("FILHO: Como 'bin' está escrito incorretamente, execl falhará e essa mensagem aparecerá no concole.\n\n");
    }
    exit(0);
}