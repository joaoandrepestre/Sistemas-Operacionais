#include "memoria.h"
#include <stdlib.h>
#include <stdio.h>

// Cria uma nova estrutura de memória
Memoria* criaMemoria(int tamanho)
{
    int i;
    Memoria* mem = (Memoria*) malloc(sizeof(Memoria));
    mem->tam = tamanho;
    mem->frames = (memory_frame*) calloc(tamanho, sizeof(memory_frame));
    mem->free_frames = criaFila();
    for(i=0;i<tamanho;i++) push(mem->free_frames, i);

    return mem;
}

// Limpa a memória da estrutura
void destroiMemoria(Memoria* mem)
{
    free(mem->frames);
    destroiFila(mem->free_frames);
    free(mem);
}

// Imprime a memória na tela
void printMemoria(Memoria* mem)
{
    int i;
    printf("Frame\tPID\tPage Number");
    for(i=0;i<mem->tam;i++){
        printf("%d\t%d\t%d\n",i,mem->frames[i].PID,mem->frames[i].page_number);
    }
    printf("\n\n");
}

