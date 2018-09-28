#include "simulador.h"
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

int main()
{
    time_t seed;
    srand((unsigned)time(&seed));

    //Define variáveis para IO
    int tempo_disco = 0; //Tempo restante para disco ser liberado. Se 0, então disco está livre para IO
    Processo* io_disco = NULL;//Processo em execução de IO de disco. Se NULL, disco está livre para IO
    int tempo_fita = 0;//Tempo restante para fita ser liberada. Se 0, então fita está livre para IO
    Processo* io_fita = NULL;//Processo em execução de IO de fita. Se NULL, fita está livre para IO
    int tempo_impressora = 0;//Tempo restante para impressora ser liberada. Se 0, então impressora está livre para IO
    Processo* io_impressora;//Processo em execução de IO de impressora. Se NULL, impressora está livre para IO

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
    int fim = 0;

    while (!fim) // Enquanto a simulação não chego uao fim
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

        executaIO(disco,&tempo_disco, &io_disco, fila_disco, baixa_prioridade);//Executa IO do processo na cabeça da fila de disco
        executaIO(fita,&tempo_fita, &io_fita, fila_fita, alta_prioridade);//Executa IO do processo na cabeça da fila de fita
        executaIO(impressora,&tempo_impressora, &io_impressora, fila_impressora, alta_prioridade);//Executa IO do processo na cabeça da fila de impressora

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
        fim = fim_simulacao(tempo_disco, tempo_fita, tempo_impressora, id, alta_prioridade, baixa_prioridade, fila_disco, fila_fita, fila_impressora);//Checa se a simulação chegou ao fim
    }

    printf("\nTodos os processos foram finalizados em %d fatias de tempo.\n", tempo);

    destroiFila(alta_prioridade);
    destroiFila(baixa_prioridade);
    destroiFila(fila_disco);
    destroiFila(fila_fita);
    destroiFila(fila_impressora);
    return 0;
}