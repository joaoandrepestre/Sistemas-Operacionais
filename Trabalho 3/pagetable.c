#include "pagetable.h"
#include <stdlib.h>
#include <stdio.h>

// Cria uma nova estrutura de PageTable
PageTable* criaPageTable(int tamanho)
{
    int i;
    PageTable* page_table = (PageTable*) malloc(sizeof(PageTable));
    page_table->tam = tamanho;
    page_table->paginas = (page_table_entry*) calloc(tamanho, sizeof(page_table_entry));
    for(i=0;i<tamanho;i++) page_table->paginas[i].frame = -1;
    return page_table;
}

// Limpa memória da PageTable
void destroiPageTable(PageTable* page_table)
{
    free(page_table->paginas);
    free(page_table);
}

// Adiciona uma entrada na tabela
void addPageTableEntry(PageTable* page_table, int pag, presente_bit pres, modificado_bit mod, int fr)
{
    page_table_entry pte = {pres, mod, fr};
    page_table->paginas[pag] = pte;
}

// Remove uma entrada da tabela de páginas
void removePageTableEntry(PageTable* page_table, int pag){
    page_table_entry pte = {0,0,-1};
    page_table->paginas[pag] = pte;
}

// Imprime a PageTable na tela
void printPageTable(PageTable* page_table){
    int i;
    printf("Página\tP\tM\tFrame\n");
    for(i=0;i<page_table->tam;i++){
        if(page_table->paginas[i].P){
            printf("%d\t%d\t%d\t%d\n",i,page_table->paginas[i].P,page_table->paginas[i].M,page_table->paginas[i].frame);
        }
    }
    printf("\n\n");
}
