#ifndef FILA_H
#define FILA_H

//Define estrutura para o item da fila (implementada como lista encadeada)
typedef struct item{
    int pagina;
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

void push(Fila* fila, int pag);
//Adiciona página ao fim da fila

int pop(Fila* fila);
//Remove primeiro elemento da fila

void paraFim(Fila* fila, int pag);
//Move página pag para o fim da fila

int filaVazia(Fila* fila);
//Retorna 1 sse a fila estiver vazia

void printFila(Fila* fila);
//Imprime a fila na tela

#endif //FILA_H