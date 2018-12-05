#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "memoria.h"
#include "processo.h"
#include "fila.h"

#define MAX_MEMORY_FRAMES 64 //número máximo de frames na memória principal
#define MAX_PROCESSOS 10 //número máximo de processos 

void gerenciaMemoria(Memoria* mem_principal, Memoria* mem_virtual, Processo* p, int pag);
// Lida com page faults

int main(){

    time_t seed;
    srand((unsigned)time(&seed));

    int id = 1;
    int i;
    int pag;

    Memoria* mem_principal = criaMemoria(MAX_MEMORY_FRAMES); //Memória principal
    Memoria* mem_virtual = criaMemoria(MAX_MEMORY_FRAMES*MAX_PROCESSOS); //Memória virtual
    Processo** processos = (Processo**) malloc(sizeof(Processo*)*MAX_PROCESSOS); // processos criados
    Fila* LRU_swap = criaFila(); //Fila de processos para fazer LRU

    while(1){
        sleep((unsigned long int) 3); // Espera 3 segundos

        if(id<=MAX_PROCESSOS){ // Se o número máximo de processos ainda não foi atingido
            processos[id-1] = criaProcesso(id, mem_principal, mem_virtual); // Cria um novo processo, aleatóriamente
            push(LRU_swap, id);
            id++;
        }

        // Mostra a o estado das memórias principal e virtual
        printf("Memória principal:\n");
        printMemoria(mem_principal);
        /* printf("Memória virtual:\n");
        printMemoria(mem_virtual); */

        //Para todos os processos criados
        for(i=0;i<id-1;i++){
            // Solicita uma página aleatóriamente
            pag = solicitaPagina(processos[i], mem_principal, mem_virtual);
            if(pag!=-1){
                // Se ocorreu page fault, chama o gerenciador de memória para resolver
                gerenciaMemoria(mem_principal, mem_virtual, processos[i], pag);
            }
        }
    }
}

// Lida com page faults
void gerenciaMemoria(Memoria* mem_principal, Memoria* mem_virtual, Processo* p, int pag)
{
    int frame;

    // Checa se está dentro do working set limit
    if(p->fila_paginas->tamanho < WSL){
        // Checa se há espaço na memória principal
        if(mem_principal->free_frames->tamanho>0){
            // Adiciona o frame na memória principal
            frame = addPageToMemory(p, mem_principal, pag, presente);
            printf("O gerenciador de memória inseriu a página %d do processo %d no frame %d da memória principal\n\n", pag, p->PID, frame);
            return;
        }
        //SWAP-OUT DE OUTRO PROCESSO
        printf("Não há espaço na memória principal, outro processo deve sofrer swap-out\n\n");
    }

    // Se atingiu o WSL, fazer LRU
    frame = swapPagesLRU(p, mem_principal, mem_virtual, pag); // ALGO ESTÁ ERRADO
    printf("O gerenciador de memória fez LRU no processo %d e inseriu a página %d no frame %d da memória principal\n\n", p->PID, pag, frame);

}