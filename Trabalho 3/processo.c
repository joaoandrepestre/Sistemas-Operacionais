#include "processo.h"
#include <stdlib.h>
#include <stdio.h>

Processo *criaProcesso(int id, Memoria* mem_principal, Memoria* mem_virtual)
{
    int i;

    int fr_principal;
    int fr_virtual;

    // Aloca espaço para o processo
    Processo *p = (Processo*) malloc(sizeof(Processo));

    // Define o identificador do processo
    p->PID = id;

    // Define paginação
    int num_paginas = rand()%MAX_PAGINAS;
    p->page_table = criaPageTable(num_paginas);

    // Define fila para LRU
    p->fila_paginas = criaFila();

    // Coloca primeira página na memória principal
    addPageToMemory(p, mem_principal, 0, presente);

    // Coloca outras páginas na memória virtual
    for(i=1;i<p->page_table->tam;i++){
        addPageToMemory(p, mem_virtual, i, ausente);
    }

    //Imprime informações do processo
    printProcesso(p);

    return p;
}

// Destrutor para o processo
void destroiProcesso(Processo* p)
{
    free(p->page_table); //Libera memória da tabela de paginação
    destroiFila(p->fila_paginas); //Destroi a fila
    free(p); //Libera memória do ponteiro do processo
}

// Solicita acesso a uma página aleatóriamente, retorna o número da página em caso de page fault
int solicitaPagina(Processo* p, Memoria* mem_principal, Memoria* mem_virtual)
{
    int pag = rand() % p->page_table->tam; //Seleciona uma página aleatória para solicitar
    //int pag = 0;
    if(p->page_table->paginas[pag].P == presente){ // Se a página selecionada está na memória principal
        printf("Processo %d acessou página %d no frame %d da memória principal\n\n", p->PID, pag, p->page_table->paginas[pag].frame);
        p->page_table->paginas[pag].M = modificado; // Altera o bit modificado para indicar que a página foi modificada
        paraFim(p->fila_paginas, pag);
        return -1;
    }
 
    printf("PAGE FAULT: Processo %d tentou acessar página %d fora da memória principal\n\n", p->PID, pag);
    return pag;
    
}

// Adiciona página a memória, adiciona referência a page table, e adiciona página a fila
// Retorna o frame em que a página foi alocada
int addPageToMemory(Processo* p, Memoria* mem, int pag, presente_bit pres)
{
    int frame = addMemoryFrame(mem, p->PID, pag);
    addPageTableEntry(p->page_table, pag, pres, nao_modificado, frame);
    if(pres == presente) push(p->fila_paginas, pag);

    return frame;
}

// Imprime o processo na tela
void printProcesso(Processo* p)
{
    printf("Processo %d\n", p->PID);
    printf("PageTable:\n");
    printPageTable(p->page_table);
}
