/* Início
    Lê linha de comando;
    Enquanto não fim faça
        Início
            Percorre a linha retirando o nome do comando;
            Executa um fork para criar um novo processo;
            Se processo filho então
                Executa execl especificando o nome do comando como parâmetro;
            Senão
                Inicio
                Executa wait para esperar que a execução do comando termine;
                Se codigo retorno = zero então
                    Escreva "Executado com sucesso."
                Senão
                    Escreva "Código de retorno = ", codigo_retorno;
                Fim
            Fim se; 
            Lê linha de comando;
        Fim;
Fim; */

#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_COMMAND_BUFFER_SIZE 50
#define MAX_PATH_BUFFER_SIZE    55

int main(){
    char command_buffer[MAX_COMMAND_BUFFER_SIZE];
    char path[MAX_PATH_BUFFER_SIZE];

    strcpy(path, "/bin/");

    //Lê linha de comando;
    while(1){
        
        // Limpa os buffers
        memset(command_buffer, 0, sizeof(command_buffer));
        memset(path+5, 0, sizeof(path-5));

        // Pede input ao usuário
        printf(">> ");

        //Percorre a linha retirando o nome do comando;
        fgets(command_buffer, sizeof command_buffer, stdin);
        if(command_buffer[0] == '\n') continue;

        // Checa o tamanho do comando, descartando o char nulo
        char* command_end = strchr(command_buffer, 0);
        int command_size = (int) (command_end-command_buffer-1);

        // Cria o path completo para o executável
        strncat(path, command_buffer, command_size);

        int fork_ret = fork();

        if(fork_ret == 0)   execl(path, command_buffer, NULL);

        else{
            int wstatus;
            wait(&wstatus);
            if(wstatus==0) printf("Executado com sucesso.\n");
            else printf("Código de retorno = %d\n",wstatus);
        }
    }

    return 0;
}
