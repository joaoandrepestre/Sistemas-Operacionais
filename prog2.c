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

#include <unistd.h>
#include <stdio.h>

int main(){
    //Lê linha de comando;
    while(1){
        //Percorre a linha retirando o nome do comando;
        int fork_ret = fork();
        if(fork_ret==0){
            char* nome_do_comando;
            execl("caminho para comandos", nome_do_comando,0);
        }
        else{
            int wstatus;
            wait(&wstatus);
            if(wstatus==0) printf("Executado com sucesso.\n");
            else printf("Código de retorno = %d\n",wstatus);
        }
        //Lê linha de comando;
    }

    return 0;
}