#ifndef PROCESSO_H
#define PROCESSO_H 

#include "fila.h"
#include "pagetable.h"
#include "memoria.h"

//Define constantes
#define WSL 4 // Working set limit: número máximo de páginas que podem estar na memória por processo
#define MAX_PAGINAS 64 // número máximo de páginas virtuais por processo

typedef struct Processo{
    int PID; // identificador do processo
    int tam_page_table; // número de páginas que o processo usa 
    page_table_entry* page_table; // associa cada página ao frame que ela ocupa na memória
    Fila *fila_paginas; // Fila de páginas na memória
} Processo;

Processo *criaProcesso(int id, memory_frame* memoria_principal, Fila* free_frames_principal, memory_frame* memoria_virtual, Fila* free_frames_virtual);
// Cria um novo processo com número de páginas aleatório e acessos a páginas aleatórios

void destroiProcesso(Processo* p);
// Destrutor para o processo

void solicitaPagina(memory_frame* memoria_principal, memory_frame* memoria_virtual);
// Solicita acesso a uma página aleatóriamente

#endif //PROCESSO_H