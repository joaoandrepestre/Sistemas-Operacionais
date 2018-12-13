#include "pagetable.h"
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>

// Cria uma nova estrutura de PageTable
PageTable *criaPageTable(int tamanho)
{
    int i;
    PageTable *page_table = (PageTable *)malloc(sizeof(PageTable));
    page_table->tam = tamanho;
    page_table->paginas = (page_table_entry *)calloc(tamanho, sizeof(page_table_entry));
    for (i = 0; i < tamanho; i++)
        page_table->paginas[i].frame = -1;
    return page_table;
}

// Limpa memória da PageTable
void destroiPageTable(PageTable *page_table)
{
    free(page_table->paginas);
    free(page_table);
}

// Adiciona uma entrada na tabela
void addPageTableEntry(PageTable *page_table, int pag, presente_bit pres, swaped_out swap, int fr)
{
    page_table_entry pte = {pres, swap, fr};
    page_table->paginas[pag] = pte;
}

// Remove uma entrada da tabela de páginas
void removePageTableEntry(PageTable *page_table, int pag)
{
    page_table_entry pte = {0, 0, -1};
    page_table->paginas[pag] = pte;
}

// Imprime a PageTable na tela
void printPageTable(PageTable *page_table, WINDOW* win, int* x, int* y)
{
    int i;
    mvwprintw(win,*y,*x,"Página");
    mvwprintw(win,*y,(*x)+10,"P");
    mvwprintw(win,*y,(*x)+19,"S");
    mvwprintw(win,*y,(*x)+24,"Frame");
    for (i = 0; i < page_table->tam; i++)
    {
        if (page_table->paginas[i].S || page_table->paginas[i].P)
        {
            (*y)++;
            mvwprintw(win,*y,*x,"%d", i);
            mvwprintw(win,*y,(*x)+9,"%d", page_table->paginas[i].P);
            mvwprintw(win,*y,(*x)+19,"%d", page_table->paginas[i].S);
            mvwprintw(win,*y,(*x)+24,"%d", page_table->paginas[i].frame);

        }
    }
    (*y)++;
    refresh();
}
