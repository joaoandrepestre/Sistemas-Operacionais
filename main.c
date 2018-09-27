#include "simulador.h"
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

int main()
{
    time_t seed;
    srand((unsigned)time(&seed));

    int tempo_disco = 0; //Tempo restante para disco ser liberado. Se 0, então disco está livre para IO
    int tempo_fita = 0;//Tempo restante para fita ser liberada. Se 0, então fita está livre para IO
    int tempo_impressora = 0;//Tempo restante para impressora ser liberada. Se 0, então impressora está livre para IO

    //Inicia as filas de alta e baixa prioridade e uma fila para cada tipo  de IO
    Fila *alta_prioridade;
    Fila *baixa_prioridade;
    Fila *fila_disco;
    Fila *fila_fita;
    Fila *fila_impressora;

    alta_prioridade = criaFila();
    baixa_prioridade = criaFila();
    fila_disco = criaFila();
    fila_fita = criaFila();
    fila_impressora = criaFila();

    int tempo = 0;
    int id = 1;

    while (tempo_disco!=0 || tempo_fita!=0 || tempo_impressora!=0 || id<=MAX_PROCESSOS || !filaVazia(alta_prioridade) || !filaVazia(baixa_prioridade) || !filaVazia(fila_disco) || !filaVazia(fila_fita) || !filaVazia(fila_impressora))
    {
        printf("Tempo t = %d\n",tempo);

        printf("Fila de Alta Prioridade: ");
        printFila(alta_prioridade);

        printf("Fila de Baixa Prioridade: ");
        printFila(baixa_prioridade);

        printf("Fila de Disco: ");
        printFila(fila_disco);

        printf("Fila de Fita: ");
        printFila(fila_fita);

        printf("Fila de Impressora: ");
        printFila(fila_impressora);
        printf("\n");
        
        insereProcesso(tempo, &id, alta_prioridade, 60); //no tempo atual, cria ou não um novo Processo

        printf("Após criação de processos:\n");
        printf("Fila de Alta Prioridade: ");
        printFila(alta_prioridade);

        printf("Fila de Baixa Prioridade: ");
        printFila(baixa_prioridade);

        printf("Fila de Disco: ");
        printFila(fila_disco);

        printf("Fila de Fita: ");
        printFila(fila_fita);

        printf("Fila de Impressora: ");
        printFila(fila_impressora);
        printf("\n");

        executaProcesso(tempo, alta_prioridade, baixa_prioridade, fila_disco, fila_fita, fila_impressora); //executa o processo da fila de prioridade correta

        printf("Após execução de processos:\n");
        printf("Fila de Alta Prioridade: ");
        printFila(alta_prioridade);

        printf("Fila de Baixa Prioridade: ");
        printFila(baixa_prioridade);

        printf("Fila de Disco: ");
        printFila(fila_disco);

        printf("Fila de Fita: ");
        printFila(fila_fita);

        printf("Fila de Impressora: ");
        printFila(fila_impressora);
        printf("\n");

        executaIO(&tempo_disco, &tempo_fita, &tempo_impressora, fila_disco, fila_fita, fila_impressora, alta_prioridade, baixa_prioridade); //executa processos que exigem IO das filas de IO e retorna para as filas corretas

        //printf("Processos em IO:\n\tDisco : %d\n\tFita: %d\n\tImpressora: %d\n\n",io_disco->contexto.PID,io_fita->contexto.PID,io_impressora->contexto.PID);
        printf("Após execução de IO:\n");
        printf("Fila de Alta Prioridade: ");
        printFila(alta_prioridade);

        printf("Fila de Baixa Prioridade: ");
        printFila(baixa_prioridade);

        printf("Fila de Disco: ");
        printFila(fila_disco);

        printf("Fila de Fita: ");
        printFila(fila_fita);

        printf("Fila de Impressora: ");
        printFila(fila_impressora);
        printf("\n");

        printf("\n----------------------------------------------------------------\n\n");
        sleep((unsigned long int)TEMPO);

        tempo++;
    }

    printf("\nTodos os processos foram finalizados em %d fatias de tempo.\n", tempo);

    destroiFila(alta_prioridade);
    destroiFila(baixa_prioridade);
    destroiFila(fila_disco);
    destroiFila(fila_fita);
    destroiFila(fila_impressora);
    return 0;
}