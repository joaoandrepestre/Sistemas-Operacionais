#define MAX_PROCESSOS 100
#define TEMPO 1
#define DISCO 10
#define FITA 20
#define IMPRESSORA 30

#include "fila.h"

Fila altaprioridade;
Fila baixaprioridade;
Fila impressora;
Fila fita;
Fila disco;

int main(){

    int processos = 0;

    while(processos<MAX_PROCESSOS){
        criaProcessos(&processos); //no tempo atual, cria ou não um novo Processo
        printf("Processo PID chegou no tempo t e entrou  na fila y");
        executaProcesso();//executa o processo da fila de prioridade correta
        printf("Processo PID selecionado para execução");
        chamaIO();//executa processos que exigem IO das filas de IO
        printf("Processo PID selecionou IO x");
        esperaIO();//retorna IO acabados para devidas filas
        printf("Processo PID retornou do IO x para fila y");

        wait(TEMPO);
    }

    return 0;
}