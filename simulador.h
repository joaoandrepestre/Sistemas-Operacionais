#ifndef SIMULADOR_H
#define SIMULADOR_H

#define MAX_PROCESSOS 100
#define TEMPO 1
#define DISCO 10
#define FITA 20
#define IMPRESSORA 30

typedef enum IO{nada=0, disco, fita, impressora} IO;
typedef enum prioridade{baixa = 0, alta} Prioridade;
typedef enum status{running = 0, ready, blocked} Status;

typedef struct PCB{
    int PID;
    int PPID;
    Prioridade prioridade;
    Status status;

} PCB;

typedef struct processo{
    PCB contexto;
    int tempo_inicial;
    int duracao_restante;
    IO tipo_IO;
    int tempo_inicial_IO; 

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

void push(Fila* fila, Processo* p);
//Adiciona processo ao fim da fila

Processo* pop(Fila* fila);
//Remove primeiro elemento da fila

void printFila(Fila* fila);
//Imprime a fila na tela

Processo* criaProcesso(int id, int parent_id, int t_inic, int t_fim, IO tipo_io, int t_io);
//Inicializa um novo processo

void insereProcesso(int tempo_atual, int probabilidade_criar_processo);
//Cria ou não um novo processo no intervalo de tempo atual com uma certa probabilidade 

void executaProcesso();
//Executar processos das filas

void chamaIO();
//Passa processos das filas de IO para execução de IO

void esperaIO();
//Retorna processos após término de IO


#endif //SIMULADOR_H