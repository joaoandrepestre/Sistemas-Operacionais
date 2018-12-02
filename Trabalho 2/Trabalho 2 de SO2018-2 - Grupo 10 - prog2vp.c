#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_COMMAND_BUFFER_SIZE 50
#define MAX_PATH_BUFFER_SIZE    55

char** separate_tokens(char* command_buffer){
    int i = 0;
    char* token;
    char** tokens = malloc(MAX_COMMAND_BUFFER_SIZE * sizeof(char*));

    token = strtok(command_buffer, " \n\t\a\r");

    while(token != NULL){
        tokens[i++] = token;
        token = strtok(NULL, " \n\t\a\r");
    }

    tokens[i] = NULL;
    return tokens;
}


int main(){
    char command_buffer[MAX_COMMAND_BUFFER_SIZE];
    char** args;

    //Lê linha de comando;
    while(1){
        
        // Limpa os buffers
        memset(command_buffer, 0, sizeof(command_buffer));

        // Pede input ao usuário
        printf(">> ");

        //Percorre a linha retirando o nome do comando;
        fgets(command_buffer, sizeof(command_buffer), stdin);
        if(command_buffer[0] == '\n') continue;

        args = separate_tokens(command_buffer);

        int fork_ret = fork();

        if(fork_ret == 0){
            execvp(args[0], args);
        }

        else{
            int wstatus;
            wait(&wstatus);
            if(wstatus==0) printf("Executado com sucesso.\n");
            else printf("Código de retorno = %d\n",wstatus);
        }

        free(args);
    }

    return 0;
}
