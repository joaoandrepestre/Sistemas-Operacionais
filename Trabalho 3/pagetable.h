#ifndef PAGETABLE_H
#define PAGETABLE_H

#include <ncurses.h>

typedef enum presente
{
    ausente = 0,
    presente
} presente_bit; //Bit para representar se a página está na memória principal ou não
typedef enum swaped_out
{
    nao_swaped = 0,
    swaped
} swaped_out; //Bit para representar se a página foi removida da memória por swap-out

typedef struct page_table_entry
{
    presente_bit P;
    swaped_out S;
    int frame;
} page_table_entry;

typedef struct page_table
{
    int tam;
    page_table_entry *paginas;
} PageTable;

PageTable *criaPageTable(int tamanho);
// Cria uma nova estrutura de PageTable

void destroiPageTable(PageTable *page_table);
// Limpa memória da PageTable

void addPageTableEntry(PageTable *page_table, int pag, presente_bit pres, swaped_out swap, int fr);
// Adiciona uma entrada na tabela

void removePageTableEntry(PageTable *page_table, int pag);
// Remove uma entrada da tabela de páginas

void printPageTable(PageTable *page_table, WINDOW* win, int* x, int* y);
// Imprime a PageTable na tela

#endif //PAGETABLE_H