#include <stdio.h>
#include <stdlib.h>
#include "memoria.h"
#include "processo.h"
#include "fila.h"
#include <time.h>

#define MAX_MEMORY_FRAMES 64 //número máximo de frames na memória principal

int main(){
    /* time_t seed;
    srand((unsigned)time(&seed)); */
    Memoria* mem_principal = criaMemoria(MAX_MEMORY_FRAMES); //Memória principal
    Memoria* mem_virtual = criaMemoria(MAX_MEMORY_FRAMES*2); //Memória virtual
    Processo** processos = (Processo*) malloc(sizeof(Processo*)*1000);

    printMemoria(mem_principal);
    printMemoria(mem_virtual);
}