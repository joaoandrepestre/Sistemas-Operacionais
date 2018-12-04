#ifndef MEMORIA_H
#define MEMORIA_H

#include "fila.h"

typedef struct memory_frame{
    int PID;
    int page_number;
} memory_frame;

typedef struct memoria{
    int tam;
    memory_frame* frames; //frames da memória
    Fila* free_frames; // fila com os frames livres
} Memoria;

Memoria* criaMemoria(int tamanho);
// Cria uma nova estrutura de memória

void destroiMemoria(Memoria* mem);
// Limpa a memória da estrutura

void printMemoria(Memoria* mem);
// Imprime a memória na tela

#endif //MEMORIA_H