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
    int pipe3[2];

    pipe(pipe1);
    pipe(pipe2);
    pipe(pipe3);
    
    // ********* Insira um comando para pegar o PID do processo corrente e mostre na
    //  tela da console.
    id = getpid();
    printf("PID do processo corrente: %d\n\n", id);

    // if (*** insira um comando para criar um subprocesso) {
    if ( (fork_ret = fork()) > 0) {
        // ***** Faça com que o processo pai execute este trecho de código
        // Feito pelo condicional -> fork retorna 0 para processo filho e o id do filho ( > 0) para processo pai

        printf("Processo pai sendo executado\n\n");

        // ***** Mostre na console o PID do processo pai e do processo filho
        printf("PAI\tPID atual: %d\n     \tPID filho: %d\n\n",id,fork_ret);

        // ***** Monte uma mensagem e a envie para o processo filho
        // Pipe 1 representa uma mesagem do pai para o filho
        // Logo, o pai fecha o input 
        char msg_envia[30] = "Olá filho, tudo bem?";
        close(pipe1[0]);
        write(pipe1[1],msg_envia,30);
        close(pipe1[1]);

        // ***** Mostre na tela o texto da mensagem enviada
        printf("PAI\tMensagem mandada: %s\n\n", msg_envia);

        // ***** Aguarde a resposta do processo filho
        wait(NULL);

        // ***** Mostre na tela o texto recebido do processo filho
        char msg_recebe[30];
        close(pipe2[1]);
        read(pipe2[0],msg_recebe,30);
        close(pipe2[0]);
        printf("PAI\tResposta recebida: %s\n\n", msg_recebe);



        // ***** Aguarde mensagem do filho e mostre o texto recebido
        char msg_recebe2[12];
        close(pipe3[1]);
        read(pipe3[0],msg_recebe2,12);
        close(pipe2[0]);
        printf("PAI\tResposta recebida: %s\n\n", msg_recebe2);
      
          // ***** Aguarde o término do processo filho
        wait(&status);
        // ***** Informe na tela que o filho terminou e que o processo pai também vai encerrarar
        printf("PAI\tProcesso filho terminado, processo pai também ira terminar.\n\n");

    } else {
        printf("Processo filho sendo executado\n\n");

        // ***** Faça com que o processo filho execute este trecho de código
        // Definido pelo condicional -> processos filhos tem fork() retornando zero
        
        //  ***** Mostre na tela o PID do processo corrente e do processo pai
        int ppid = getppid();
        printf("FILHO\tPID atual: %d\n       \tPID pai: %d\n\n",id,ppid);

        // ***** Aguarde a mensagem do processo pai e ao receber mostre o texto na tela
        char msg_recebe[30];
        close(pipe1[1]);
        read(pipe1[0], msg_recebe,30);
        close(pipe1[0]);
        printf("FILHO\tMensagem recebida: %s\n\n",msg_recebe);

        // ***** Envie uma mensagem resposta ao pai
        char msg_envia[30] = "Olá pai, tudo bem sim!";
        close(pipe2[0]);
        write(pipe2[1],msg_envia,30);
        close(pipe2[1]);
        printf("FILHO\tResposta enviada: %s\n\n", msg_envia);
        
        // ***** Execute o comando “for” abaixo
        for (j = 0; j <= 10000; j++);
        char msg[12];
        // ***** Envie mensagem ao processo pai com o valor final de “j”
        sprintf(msg,"%d",j);
        close(pipe3[0]);
        write(pipe3[1],msg,12);
        printf("FILHO\tMensagem enviada: %s\n\n", msg);
        close(pipe3[1]);

        // // ***** Execute o comando abaixo e responda às perguntas
        execl("/Bin/ls", "ls", NULL);
        
        // // ***** O que acontece após este comando?
        printf("FILHO\tSe execl não falhar, essa mensagem não aparecerá no console.\n");
        // // ***** O que pode acontecer se o comando “execl” falhar?
        printf("FILHO\tComo 'bin' está escrito incorretamente, execl falhará e essa mensagem aparecerá no concole.\n\n");
    }
    exit(0);
}