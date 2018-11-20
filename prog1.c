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
    
    //  Insira um comando para pegar o PID do processo corrente e mostre na
    //  tela da console.
    id = getpid();
    printf("PID do processo corrente: %d\n\n", id);

    // if (insira um comando para criar um subprocesso) {
    if ( (fork_ret = fork()) > 0) {
        // Faça com que o processo pai execute este trecho de código
        // Feito pelo condicional -> fork retorna 0 para processo filho e o id do filho ( > 0) para processo pai

        printf("Processo pai sendo executado\n\n");

        // Mostre na console o PID do processo pai e do processo filho
        // Como este é o processo pai, o id do processo corrente é o seu id
        // E o id do filho é o retorno do fork
        printf("PAI\tPID atual: %d\n     \tPID filho: %d\n\n",id,fork_ret);

        // Monte uma mensagem e a envie para o processo filho
        char msg_envia[21] = "Olá filho, tudo bem?";

        // Pipe 1 representa mesagens do pai para o filho
        // O pai fecha a ponta de leitura do pipe 1
        close(pipe1[0]);
        // Escreve na ponta de escrita
        write(pipe1[1],msg_envia,21);
        // E fecha a ponta de escrita
        close(pipe1[1]);

        // Mostre na tela o texto da mensagem enviada
        printf("PAI\tMensagem enviada: %s\n\n", msg_envia);

        // Aguarde a resposta do processo filho
        char msg_recebe[23];
        // Pipe 2 representa mensagens do filho para o pai
        // O pai fecha a ponta de escrita do pipe 2
        close(pipe2[1]);
        // Lê da ponta de leitura
        read(pipe2[0],msg_recebe,23);

        // Mostre na tela o texto recebido do processo filho
        printf("PAI\tResposta recebida: %s\n\n", msg_recebe);

        // Aguarde mensagem do filho
        // Para isso a ponta de leitura do pipe 2 foi mantida aberta
        // O pai lê da ponta de leitura 
        read(pipe2[0],msg_recebe,12);
        // E fecha a ponta de leitura
        close(pipe2[0]);

        // e mostre o texto recebido
        printf("PAI\tMensagem recebida: %s\n\n", msg_recebe);
      
        // Aguarde o término do processo filho
        wait(&status);
        // Informe na tela que o filho terminou e que o processo pai também vai encerrarar
        printf("PAI\tProcesso filho terminado, processo pai também ira terminar.\n\n");

    } else {
        // Faça com que o processo filho execute este trecho de código
        // Definido pelo condicional -> processos filhos tem fork() retornando zero

        printf("Processo filho sendo executado\n\n");
    
        // Como este é o processo filho, é necessário obter o seu id por getpid
        // E o id do seu pai é o id do encontrado no início do programa
        int id_filho = getpid();

        // Mostre na tela o PID do processo corrente e do processo pai
        printf("FILHO\tPID atual: %d\n       \tPID pai: %d\n\n",id_filho,id);

        // Aguarde a mensagem do processo pai 
        char msg_recebe[21];
        // Pipe 1 representa mensagens do pai para o filho
        // O filho fecha a ponta de escrita do pipe 1
        close(pipe1[1]);
        // Lê a ponta de leitura
        read(pipe1[0], msg_recebe,21);
        // E fecha a ponta de leitura
        close(pipe1[0]);

        // Mostre o texto na tela
        printf("FILHO\tMensagem recebida: %s\n\n",msg_recebe);

        // Envie uma mensagem resposta ao pai
        char msg_envia[23] = "Olá pai, tudo bem sim!";
        // Pipe 2 representa mensagens do filho para o pai
        // O filho fecha a ponta de leitura
        close(pipe2[0]);
        // Escreve na ponta de escrita
        write(pipe2[1],msg_envia,23);

        printf("FILHO\tResposta enviada: %s\n\n", msg_envia);
        
        // Execute o comando “for” abaixo
        for (j = 0; j <= 10000; j++);

        // Envie mensagem ao processo pai com o valor final de “j”
        sprintf(msg_envia,"%d",j);
        // A ponta de escrita do pipe 2 foi mantida aberta
        // Porque o filho escreve novamente na ponta de escrita
        write(pipe2[1],msg_envia,12);
        // E fecha a ponta de escrita
        close(pipe2[1]);
        
        printf("FILHO\tMensagem enviada: %s\n\n", msg_envia);
        

        // Execute o comando abaixo e responda às perguntas
        execl("/Bin/ls", "ls", NULL);
        
        // O que acontece após este comando?
        // O programa tenta executar o programa ls. Caso ele consiga, o processo atual é substituido pelo do ls.
        // O que pode acontecer se o comando “execl” falhar?
        // Caso falhe, o programa segue normalmente, executando as mensagens abaixo
        
        printf("FILHO\tSe execl não falhar, essa mensagem não aparecerá no console.\n\n");
        printf("FILHO\tComo 'bin' está escrito incorretamente, execl falhará e essa mensagem aparecerá no console.\n\n");
    }
    exit(0);
}