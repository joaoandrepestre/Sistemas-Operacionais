#ifndef MEMORIA_H
#define MEMORIA_H

#include "fila.h"

typedef struct memory_frame
{
    int PID;
    int page_number;
} memory_frame;

typedef struct memoria
{
    int tam;
    memory_frame *frames; //frames da memória
    int *free_frames;     // array binário com os frames livres 
    int tam_free_frames; // número de frames livres
} Memoria;

Memoria *criaMemoria(int tamanho);
// Cria uma nova estrutura de memória

void destroiMemoria(Memoria *mem);
// Limpa a memória da estrutura

int addMemoryFrame(Memoria *mem, int id, int pag_num);
// Adiciona um frame a memória

void removeMemoryFrame(Memoria *mem, int fr);
// Remove um frame da memória

void printMemoria(Memoria *mem, int x, int y);
// Imprime a memória na tela

#endif //MEMORIA_H