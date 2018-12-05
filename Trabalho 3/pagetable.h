#ifndef PAGETABLE_H
#define PAGETABLE_H

typedef enum presente{ausente=0,presente} presente_bit; //Bit para representar se a página está na memória principal ou não
typedef enum modificado{nao_modificado = 0, modificado} modificado_bit; //Bit para representar se a página foi modificada ou não

typedef struct page_table_entry{
    presente_bit P;
    modificado_bit M;
    int frame;
} page_table_entry;

typedef struct page_table{
    int tam;
    page_table_entry* paginas;
}PageTable;

PageTable* criaPageTable(int tamanho);
// Cria uma nova estrutura de PageTable

void destroiPageTable(PageTable* page_table);
// Limpa memória da PageTable

void addPageTableEntry(PageTable* page_table, int pag, presente_bit pres, modificado_bit mod, int fr);
// Adiciona uma entrada na tabela

void removePageTableEntry(PageTable* page_table, int pag);
// Remove uma entrada da tabela de páginas

void printPageTable(PageTable* page_table);
// Imprime a PageTable na tela

#endif //PAGETABLE_H