#include "memoria.h"
#include <stdlib.h>
#include <stdio.h>

// Cria uma nova estrutura de memória
Memoria *criaMemoria(int tamanho)
{
    int i;
    Memoria *mem = (Memoria *)malloc(sizeof(Memoria));
    mem->tam = tamanho;
    mem->frames = (memory_frame *)calloc(tamanho, sizeof(memory_frame));
    mem->free_frames = (int *)malloc(sizeof(int) * tamanho);
    for (i = 0; i < tamanho; i++)
    {
        mem->frames[i].page_number = -1;
        mem->free_frames[i] = 1;
    }
    mem->tam_free_frames = tamanho;

    return mem;
}

// Limpa a memória da estrutura
void destroiMemoria(Memoria *mem)
{
    free(mem->frames);
    free(mem->free_frames);
    free(mem);
}

// Adiciona uma página à memória e retorna o frame em que a página está
int addMemoryFrame(Memoria *mem, int id, int pag_num)
{
    int fr;
    memory_frame mem_fr = {id, pag_num};
    for (fr = 0; fr < mem->tam; fr++)
    {
        if (mem->free_frames[fr] == 1)
            break;
    }
    mem->frames[fr] = mem_fr;
    mem->free_frames[fr] = 0;
    mem->tam_free_frames--;

    return fr;
}

// Remove um frame da memória
void removeMemoryFrame(Memoria *mem, int fr)
{
    memory_frame mem_fr = {0, -1};
    mem->frames[fr] = mem_fr;
    mem->free_frames[fr] = 1;
    mem->tam_free_frames++;
}

// Imprime a memória na tela
void printMemoria(Memoria *mem)
{
    int i;
    printf("Frame\tPID\tPage Number\n");
    for (i = 0; i < mem->tam; i++)
    {
        if (mem->frames[i].page_number != -1)
        {
            printf("%d\t%d\t%d\n", i, mem->frames[i].PID, mem->frames[i].page_number);
        }
    }
    printf("\n\n");
}
