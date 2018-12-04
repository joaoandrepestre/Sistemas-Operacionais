#include "processo.h"
#include <stdlib.h>

Processo *criaProcesso(int id, memory_frame* memoria_principal, Fila* free_frames_principal, memory_frame* memoria_virtual, Fila* free_frames_virtual)
{
    int i;
    int frame_principal;
    int frame_virtual;

    // Aloca espaço para o processo
    Processo *p = (Processo*) malloc(sizeof(Processo));

    // Define o identificador do processo
    p->PID = id;

    // Define paginação
    p->tam_page_table = rand() % MAX_PAGINAS; // Define tamanho da tabela de paginação
    p->page_table = (page_table_entry*) malloc(sizeof(page_table_entry)*p->tam_page_table); // Aloca memória para tabela de paginação

    frame_principal = pop(free_frames_principal);

    // Coloca primeira página na memória principal
    memory_frame frame = {id, 0}; 
    memoria_principal[frame_principal] = frame;

    // Define entrada na tabela de paginação para primeira página
    page_table_entry pte = {presente, nao_modificado, frame_principal};
    p->page_table[0] = pte;

    for(i=1;i<p->tam_page_table;i++){
        frame_virtual = pop(free_frames_virtual);
        frame.page_number = i;
        memoria_virtual[frame_virtual] = frame;

        pte.P = ausente;
        pte.frame = frame_virtual;
        p->page_table[i] = pte;
    }



    // Define fila para LRU
    p->fila_paginas = criaFila();
    push(p->fila_paginas, 0);

    return p;
}

// Destrutor para o processo
void destroiProcesso(Processo* p)
{
    free(p->page_table); //Libera memória da tabela de paginação
    destroiFila(p->fila_paginas); //Destroi a fila
    free(p); //Libera memória do ponteiro do processo
}
