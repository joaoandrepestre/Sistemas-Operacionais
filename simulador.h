#ifndef SIMULADOR_H
#define SIMULADOR_H

//Define constantes para simulador
#define MAX_PROCESSOS 10 //número máximo de processos
#define TEMPO 3 //Duração de uma fatia de tempo em segundos
#define DISCO 3 //Duração de um IO de disco em fatias de tempo 
#define FITA 6 //Duração de um IO de fita em fatias de tempo
#define IMPRESSORA 9 //Duração de um IO de impressora em fatias de tempo

//Define tabelas de nomenclatura
typedef enum IO{nada=0, disco, fita, impressora} IO; //Tipos de IO
typedef enum prioridade{baixa = 0, alta} Prioridade; //Tipos de prioridade
typedef enum status{running = 0, ready, blocked} Status; //Tipos de status

//Define estrutura para o Process Control Block -- contexto do processo
typedef struct PCB{
    int PID;
    int PPID;
    Prioridade prioridade;
    Status status;

} PCB;

//Define estrutura para o Processo
typedef struct processo{
    PCB contexto;
    int tempo_inicial;
    int duracao_restante;
    IO tipo_IO;
    int tempo_inicial_IO; 

} Processo;

//Define estrutura para o item da fila(implementada como lista de adjacências)
typedef struct item{
    Processo* proc;
    struct item* proximo;   
} Item;

//Define estrutura para a fila
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

int filaVazia(Fila* fila);
//Retorna 1 sse a fila estiver vazia

void printFila(Fila* fila);
//Imprime a fila na tela

Processo* criaProcesso(int id, int parent_id, int t_inic, int t_fim, IO tipo_io, int t_io);
//Inicializa um novo processo

void insereProcesso(int tempo_atual, int* id, Fila* alta_prioridade, int probabilidade_criar_processo);
//Cria ou não um novo processo no intervalo de tempo atual com uma certa probabilidade 

void executaProcesso(int tempo_atual, Fila* alta_prioridade, Fila* baixa_prioridade, Fila* fila_disco, Fila* fila_fita, Fila* fila_impressora);
//Executar processos das filas

void executaIO(int* tempo_disco, int* tempo_fita, int* tempo_impressora, Fila* fila_disco, Fila* fila_fita, Fila* fila_impressora, Fila* alta_prioridade, Fila* baixa_prioridade);
//Passa processos das filas de IO para execução de IO



#endif //SIMULADOR_H