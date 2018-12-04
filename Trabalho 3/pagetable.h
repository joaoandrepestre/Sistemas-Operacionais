#ifndef PAGETABLE_H
#define PAGETABLE_H

typedef enum presente{ausente=0,presente} presente_bit; //Bit para representar se a página está na memória principal ou não
typedef enum modificado{nao_modificado = 0, modificado} modificado_bit; //Bit para representar se a página foi modificada ou não

typedef struct page_table_entry{
    presente_bit P;
    modificado_bit M;
    int frame;
} page_table_entry;


#endif //PAGETABLE_H