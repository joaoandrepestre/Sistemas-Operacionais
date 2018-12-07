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

    mem_principal->frames[frame_remov].page_number = pag; // Insere a nova página na memória principal
    // Altera o registro da nova página na page table
    p->page_table->paginas[pag].P = presente;
    p->page_table->paginas[pag].frame = frame_remov;

    mem_virtual->frames[frame].page_number = pag_remov; // Insere a página antiga na memória virtual
    // Altera registro da página antiga na page table
    p->page_table->paginas[pag_remov].P = ausente;
    p->page_table->paginas[pag_remov].frame = frame;

    // Insere nova página no fim da fila de páginas para LRU
    push(p->fila_paginas, pag);

    return frame_remov;
}

// Imprime o processo na tela
void printProcesso(Processo *p)
{
    printf("Processo %d\n", p->PID);
    printf("PageTable:\n");
    printPageTable(p->page_table);
}
