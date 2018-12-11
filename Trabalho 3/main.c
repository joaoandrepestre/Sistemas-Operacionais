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

int swapOutProcessos(Memoria *mem_principal, Memoria *mem_virtual, Processo **processos, Fila *fila_processos, Processo *p);
// Faz swap-out no processo mais antigo para colocar a página nova na memória principal

void swapInProcessos(Memoria *mem_principal, Memoria *mem_virtual, Processo **processos, Fila *fila_processos, Processo *p);
// Faz swap-in no processo que sofreu swap-out

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
        sleep((unsigned long int)3); // Espera 3 segundos

        if (id <= MAX_PROCESSOS)
        {                                                                     // Se o número máximo de processos ainda não foi atingido
            processos[id - 1] = criaProcesso(id, mem_principal, mem_virtual); // Cria um novo processo, aleatóriamente
            push(fila_processos, id);
            id++;
        }

        // Mostra a o estado das memórias principal e virtual
        printf("Memória principal:\n");
        printMemoria(mem_principal);

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
            // Manda o processo para o fim da fila
            paraFim(fila_processos, processos[i]->PID);
        }
    }

    // Limpeza de memória   TALVEZ PENSAR EM UMA FORMA PARA O LOOP ACABAR
    destroiMemoria(mem_principal);
    destroiMemoria(mem_virtual);
    for(i=0;i<MAX_PROCESSOS;i++) destroiProcesso(processos[i]);
    destroiFila(fila_processos);
}

// Lida com page faults
void gerenciaMemoria(Memoria *mem_principal, Memoria *mem_virtual, Processo **processos, Fila *fila_processos, Processo *p, int pag)
{
    int frame;
    int proc_swap;

    // Checa se a página pedida sofreu swap-out
    if (p->page_table->paginas[pag].S)
    {
        // SWAP-IN DO PROCESSO
        printf("A página %d sofreu swap-out, deve ser feito swap-in do processo %d\n\n", pag, p->PID);
        swapInProcessos(mem_principal, mem_virtual, processos, fila_processos, p);
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
        proc_swap = swapOutProcessos(mem_principal, mem_virtual, processos, fila_processos, p);
        printf("O processo %d sofreu swap-out.\n", proc_swap);
        frame = addPageToMemory(p, mem_principal, pag, presente, nao_swaped);
        printf("O gerenciador de memória inseriu a página %d do processo %d no frame %d da memória principal\n\n", pag, p->PID, frame);
        return;
    }

    // Se atingiu o WSL, fazer LRU
    printf("WORKING SET LIMIT do processo %d atingido.\n", p->PID);
    frame = swapPagesLRU(p, mem_principal, mem_virtual, pag);
    printf("O gerenciador de memória fez LRU no processo e inseriu a página %d no frame %d da memória principal\n\n", pag, frame);
}

int swapOutProcessos(Memoria *mem_principal, Memoria *mem_virtual, Processo **processos, Fila *fila_processos, Processo *p)
{
    int i, j, lixo;
    int proc_remov;

    // Pega processo mais antigo diferente de p e que não sofreu swap-out
    proc_remov = pop(fila_processos);
    // Enquanto o processo mais antigo for igual a p ou já tiver sofrido swap-out
    while (proc_remov == p->PID || processos[proc_remov - 1]->S)
    {
        // Põe ele no fim da fila
        push(fila_processos, proc_remov);
        // Pega o próximo da fila
        proc_remov = pop(fila_processos);
    }

    // Define bit S como swaped
    processos[proc_remov - 1]->S = swaped;

    // Encontra todas as páginas desse processo que estão na memória principal
    j = 0;
    for (i = 0; i < processos[proc_remov - 1]->page_table->tam; i++)
    {
        if (j == WSL)
            break;
        if (processos[proc_remov - 1]->page_table->paginas[i].P)
        {
            // Remove da memória principal
            removeMemoryFrame(mem_principal, processos[proc_remov - 1]->page_table->paginas[i].frame);
            // Adiciona a memória virtual
            addPageToMemory(processos[proc_remov - 1], mem_virtual, i, ausente, swaped);
            // Remove uma página da fila de páginas
            pop(processos[proc_remov - 1]->fila_paginas);
            j++;
        }
    }
    // Insere o processo removido na fila de processos novamente
    push(fila_processos, proc_remov);

    return proc_remov;
}

// Faz swap-in no processo que sofreu swap-out
void swapInProcessos(Memoria *mem_principal, Memoria *mem_virtual, Processo **processos, Fila *fila_processos, Processo *p)
{
    int i, j, frame;

    int *pag_recoloc = (int *)malloc(sizeof(int) * WSL);
    for (i = 0; i < WSL; i++)
        pag_recoloc[i] = -1;

    // Encontrar todas as páginas do processo que sofreram swap-out
    j = 0;
    for (i = 0; i < p->page_table->tam; i++)
    {
        // Se já foram encontradas WSL páginas, para a busca
        if (j == WSL)
            break;
        // Se a página sofreu swap-out
        if (p->page_table->paginas[i].S)
        {
            p->page_table->paginas[i].S = nao_swaped;
            gerenciaMemoria(mem_principal, mem_virtual, processos, fila_processos, p, i);
            pag_recoloc[j] = i; // Coloca a página no grupo de páginas que sofreram swap-out
            j++;                // Incrementao contador de páginas encontradas
        }
    }

    // Redefine bit S como nao_swaped
    p->S = nao_swaped;
}
