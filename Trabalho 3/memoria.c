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
    for(i=0;i<tamanho;i++){
        mem->frames[i].page_number = -1;
        push(mem->free_frames, i);
    }

    return mem;
}

// Limpa a memória da estrutura
void destroiMemoria(Memoria* mem)
{
    free(mem->frames);
    destroiFila(mem->free_frames);
    free(mem);
}

// Adiciona uma página à memória e retorna o frame em que a página está 
int addMemoryFrame(Memoria* mem, int id, int pag_num)
{
    memory_frame mem_fr = {id, pag_num};
    int fr = pop(mem->free_frames);
    mem->frames[fr] = mem_fr;

    return fr;
}

// Remove um frame da memória
void removeMemoryFrame(Memoria* mem, int fr)
{
    memory_frame mem_fr = {0,-1};
    mem->frames[fr] = mem_fr;
    push(mem->free_frames, fr);
}

// Imprime a memória na tela
void printMemoria(Memoria* mem)
{
    int i;
    printf("Frame\tPID\tPage Number\n");
    for(i=0;i<mem->tam;i++){
        if(mem->frames[i].page_number!=-1){
            printf("%d\t%d\t%d\n",i,mem->frames[i].PID,mem->frames[i].page_number);
        }
    }
    printf("\n\n");
}

