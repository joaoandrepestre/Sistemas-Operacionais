#include "simulador.h"
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

int main()
{
    time_t seed;
    srand((unsigned)time(&seed));

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

    while (!filaVazia(alta_prioridade) || !filaVazia(baixa_prioridade) || !filaVazia(fila_disco) || !filaVazia(fila_fita) || !filaVazia(fila_impressora))
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

        executaIO(fila_disco, fila_fita, fila_impressora, alta_prioridade, baixa_prioridade); //executa processos que exigem IO das filas de IO e retorna para as filas corretas

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

    destroiFila(alta_prioridade);
    destroiFila(baixa_prioridade);
    destroiFila(fila_disco);
    destroiFila(fila_fita);
    destroiFila(fila_impressora);
    return 0;
}