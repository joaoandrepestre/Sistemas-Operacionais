#include "processo.h"
#include <stdlib.h>
#include <stdio.h>

Processo *criaProcesso(int id, Memoria *mem_principal, Memoria *mem_virtual)
{
    int i;

    int fr_principal;
    int fr_virtual;

    // Aloca espaço para o processo
    Processo *p = (Processo *)malloc(sizeof(Processo));

    // Define o identificador do processo
    p->PID = id;

    // Define bit de swap-out
    p->S = nao_swaped;

    // Define paginação
    int num_paginas = rand() % MAX_PAGINAS;
    num_paginas = num_paginas > 0 ? num_paginas : 1; //Impede que o número de páginas seja 0
    p->page_table = criaPageTable(num_paginas);

    // Define fila para LRU
    p->fila_paginas = criaFila();

    // Coloca primeira página na memória principal
    addPageToMemory(p, mem_principal, 0, presente, nao_swaped);

    // Coloca outras páginas na memória virtual
    for (i = 1; i < p->page_table->tam; i++)
    {
        addPageToMemory(p, mem_virtual, i, ausente, nao_swaped);
    }

    //Imprime informações do processo
    printProcesso(p);

    return p;
}

// Destrutor para o processo
void destroiProcesso(Processo *p)
{
    free(p->page_table);          //Libera memória da tabela de paginação
    destroiFila(p->fila_paginas); //Destroi a fila
    free(p);                      //Libera memória do ponteiro do processo
}

// Solicita acesso a uma página aleatóriamente, retorna o número da página em caso de page fault
int solicitaPagina(Processo *p, Memoria *mem_principal, Memoria *mem_virtual)
{
    int pag = rand() % p->page_table->tam; //Seleciona uma página aleatória para solicitar
    //int pag = 0;
    if (p->page_table->paginas[pag].P == presente)
    { // Se a página selecionada está na memória principal
        printf("Processo %d acessou página %d no frame %d da memória principal\n\n", p->PID, pag, p->page_table->paginas[pag].frame);
        paraFim(p->fila_paginas, pag);
        return -1;
    }

    printf("PAGE FAULT: Processo %d tentou acessar página %d fora da memória principal\n\n", p->PID, pag);
    return pag;
}

// Adiciona página a memória, adiciona referência a page table, e adiciona página a fila
// Retorna o frame em que a página foi alocada
int addPageToMemory(Processo *p, Memoria *mem, int pag, presente_bit pres, swaped_out swap)
{
    int frame = addMemoryFrame(mem, p->PID, pag);
    addPageTableEntry(p->page_table, pag, pres, swap, frame);
    if (pres == presente)
        push(p->fila_paginas, pag);

    return frame;
}

// Insere a nova página na memória trocando com uma antiga usando LRU
// Retorna o frame em que a página foi alocada
int swapPagesLRU(Processo *p, Memoria *mem_principal, Memoria *mem_virtual, int pag)
{
    int frame;
    int pag_remov;
    int frame_remov;

    frame = p->page_table->paginas[pag].frame; //Encontra onde a página nova se encontra na memória virtual

    pag_remov = pop(p->fila_paginas);                      //Pega a página mais antiga para ser removida da memória principal
    frame_remov = p->page_table->paginas[pag_remov].frame; //Encontra onde a página se encontra na memória principal

    memory_frame mem_fr_novo = {p->PID, pag};
    mem_principal->frames[frame_remov] = mem_fr_novo; // Insere a nova página na memória principal
    // Altera o registro da nova página na page table
    addPageTableEntry(p->page_table, pag, presente, nao_swaped, frame_remov);

    memory_frame mem_fr_remov = {p->PID, pag_remov};
    mem_virtual->frames[frame] = mem_fr_remov; // Insere a página antiga na memória virtual
    // Altera registro da página antiga na page table
    addPageTableEntry(p->page_table, pag_remov, ausente, nao_swaped, frame);

    // Insere nova página no fim da fila de páginas para LRU
    push(p->fila_paginas, pag);

    return frame_remov;
}

// Imprime o processo na tela
void printProcesso(Processo *p)
{
    printf("Processo %d\tSwap: %d\n", p->PID, p->S);
    printf("PageTable:\n");
    printPageTable(p->page_table);
    printFila(p->fila_paginas);
}
