#ifndef SIMULADOR_H
#define SIMULADOR_H

typedef struct PCB{
    int PID;
    int PPID;
    int prioridade;
    int status;

} Processo;

typedef struct item{
    Processo* proc;
    struct item* proximo;   
} Item;

typedef struct fila{
    int tamanho;
    Item* primeiro;
    Item* ultimo;

} Fila;

Fila* criaFila();
//Cria uma nova estrutura de fila

void destroiFila(Fila* fila);//ESSA FUNÇÃO FUNCIONA SEMPRE - JOÃO GOSTO
//Limpa a memória da fila

void push(Fila* fila, Processo p);
//Adiciona processo ao fim da fila

Processo* pop(Fila* fila);
//Remove primeiro elemento da fila

void criaProcessos();
//Cria ou não um novo processo no intervalo de tempo atual com uma certa probabilidade 

void executaProcesso();
//Executar processos das filas

void chamaIO();
//Passa processos das filas de IO para execução de IO

void esperaIO();
//Retorna processos após término de IO


#endif //SIMULADOR_H