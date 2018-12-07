#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "memoria.h"
#include "processo.h"
#include "fila.h"

#define MAX_MEMORY_FRAMES 64 //número máximo de frames na memória principal
#define MAX_PROCESSOS 20     //número máximo de processos

void gerenciaMemoria(Memoria *mem_principal, Memoria *mem_virtual, Processo **processos, Fila *fila_processos, Processo *p, int pag);
// Lida com page faults

int *swapOutProcessos(Memoria *mem_principal, Memoria *mem_virtual, Processo **processos, Fila *fila_processos, Processo *p, int pag);
// Faz swap-out no processo mais antigo para colocar a página nova na memória principal

int main()
{

    time_t seed;
    srand((unsigned)time(&seed));

    int id = 1;
    int i;
    int pag;

    Memoria *mem_principal = criaMemoria(MAX_MEMORY_FRAMES);                        //Memória principal
    Memoria *mem_virtual = criaMemoria(MAX_MEMORY_FRAMES * MAX_PROCESSOS);          //Memória virtual
    Processo **processos = (Processo **)malloc(sizeof(Processo *) * MAX_PROCESSOS); // processos criados
    Fila *fila_processos = criaFila();                                              //Fila de processos para fazer LRU

    while (1)
    {
        //sleep((unsigned long int)3); // Espera 3 segundos

        if (id <= MAX_PROCESSOS)
        {                                                                     // Se o número máximo de processos ainda não foi atingido
            processos[id - 1] = criaProcesso(id, mem_principal, mem_virtual); // Cria um novo processo, aleatóriamente
            push(fila_processos, id);
            id++;
        }

        // Mostra a o estado das memórias principal e virtual
        printf("Memória principal:\n");
        printMemoria(mem_principal);
        /* printf("Memória virtual:\n");
        printMemoria(mem_virtual); */

        //Para todos os processos criados
        for (i = 0; i < id - 1; i++)
        {
            // Solicita uma página aleatóriamente
            pag = solicitaPagina(processos[i], mem_principal, mem_virtual);
            if (pag != -1)
            {
                // Se ocorreu page fault, chama o gerenciador de memória para resolver
                gerenciaMemoria(mem_principal, mem_virtual, processos, fila_processos, processos[i], pag);
            }
            paraFim(fila_processos, processos[i]->PID);
        }
    }
}

// Lida com page faults
void gerenciaMemoria(Memoria *mem_principal, Memoria *mem_virtual, Processo **processos, Fila *fila_processos, Processo *p, int pag)
{
    int frame;
    int *swap_ret;

    // Checa se a página pedida sofreu swap-out
    if (p->page_table->paginas[pag].S)
    {
        // SWAP-IN DO PROCESSO
        printf("A página sofreu swap-out, deve ser feito swap-in do processo\n\n");
        return;
    }

    // Checa se está dentro do working set limit
    if (p->fila_paginas->tamanho < WSL)
    {
        // Checa se há espaço na memória principal
        if (mem_principal->tam_free_frames > 0)
        {
            // Adiciona o frame na memória principal
            frame = addPageToMemory(p, mem_principal, pag, presente, nao_swaped);
            printf("O gerenciador de memória inseriu a página %d do processo %d no frame %d da memória principal\n\n", pag, p->PID, frame);
            return;
        }
        // Se não houver espaço na memória principal
        //SWAP-OUT DE OUTRO PROCESSO
        printf("Não há espaço na memória principal, outro processo deve sofrer swap-out\n");
        swap_ret = swapOutProcessos(mem_principal, mem_virtual, processos, fila_processos, p, pag);
        printf("O processo %d sofreu swap-out.\n", swap_ret[0]);
        printf("O gerenciador de memória inseriu a página %d do processo %d no frame %d da memória principal\n\n", pag, p->PID, swap_ret[1]);
        return;
    }

    // Se atingiu o WSL, fazer LRU
    printf("WORKING SET LIMIT do processo %d atingido.\n", p->PID);
    frame = swapPagesLRU(p, mem_principal, mem_virtual, pag);
    printf("O gerenciador de memória fez LRU no processo e inseriu a página %d no frame %d da memória principal\n\n", pag, frame);
}

int *swapOutProcessos(Memoria *mem_principal, Memoria *mem_virtual, Processo **processos, Fila *fila_processos, Processo *p, int pag)
{
    int *ret = (int *)malloc(sizeof(int) * 2);

    int i;
    int j;
    int proc_remov;
    // Lista de frames e páginas a serem removidos iniiados como -1
    int frames_remov[4] = {-1, -1, -1, -1};
    int pags_remov[4] = {-1, -1, -1, -1};
    int frame;

    proc_remov = pop(fila_processos); // Pega processo mais antigo
    if (proc_remov == p->PID)
    { // Se o processo mais antigo for o prórpio processo
        // Pega o próximo da fila
        push(fila_processos, proc_remov);
        proc_remov = pop(fila_processos);
    }
    // Encontra todas as páginas desse processo que estão na memória principal
    j = 0;
    for (i = 0; i < processos[proc_remov - 1]->page_table->tam; i++)
    {
        if (processos[proc_remov - 1]->page_table->paginas[i].P)
        {
            frames_remov[j] = processos[proc_remov - 1]->page_table->paginas[i].frame;
            pags_remov[j] = i;
            j++;
        }
    }

    // Remove páginas da memória principal
    for (i = 0; i < 4; i++)
    {
        if (frames_remov[i] != -1)
        {
            // Remove da memória principal
            removeMemoryFrame(mem_principal, frames_remov[i]);
            // Adiciona a memória virtual
            addPageToMemory(processos[proc_remov - 1], mem_virtual, pags_remov[i], ausente, swaped);
        }
    }

    // Insere o processo removido na fila de processos novamente
    push(fila_processos, proc_remov);

    //Adiciona página nova a memória principal
    ret[1] = addPageToMemory(p, mem_principal, pag, presente, nao_swaped);

    ret[0] = proc_remov;
    return ret;
}