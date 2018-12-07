#ifndef PROCESSO_H
#define PROCESSO_H

#include "fila.h"
#include "pagetable.h"
#include "memoria.h"

//Define constantes
#define WSL 4          // Working set limit: número máximo de páginas que podem estar na memória por processo
#define MAX_PAGINAS 64 // número máximo de páginas virtuais por processo

typedef struct Processo
{
    int PID;               // identificador do processo
    swaped_out S;          // marca se o processo sofreu swap-out
    PageTable *page_table; // Tabela de paginação
    Fila *fila_paginas;    // Fila de páginas na memória para fazer LRU
} Processo;

Processo *criaProcesso(int id, Memoria *mem_principal, Memoria *mem_virtual);
// Cria um novo processo com número de páginas aleatório e acessos a páginas aleatórios

void destroiProcesso(Processo *p);
// Destrutor para o processo

int solicitaPagina(Processo *p, Memoria *mem_principal, Memoria *mem_virtual);
// Solicita acesso a uma página aleatóriamente, retorna o número da página em caso de page fault

int addPageToMemory(Processo *p, Memoria *mem, int pag, presente_bit pres, swaped_out swap);
// Adiciona página a memória, adiciona referência a page table, e adiciona página a fila
// Retorna o frame em que a página foi alocada

int swapPagesLRU(Processo *p, Memoria *mem_principal, Memoria *mem_virtual, int pag);
// Insere a nova página na memória trocando com uma antiga usando LRU
// Retorna o frame em que a página foi alocada

void printProcesso(Processo *p);
// Imprime o processo na tela

#endif //PROCESSO_H