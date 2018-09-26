#include "simulador.h"
#include <stdlib.h>
#include <stdio.h>

Fila* alta_prioridade;
Fila* baixa_prioridade;
Fila* impressora;
Fila* fita;
Fila* disco;

int id;

Fila* criaFila(){
    Fila* novaFila = (Fila*) malloc(sizeof(Fila));
    novaFila->tamanho = 0;
    novaFila->primeiro = NULL;
    novaFila->ultimo = NULL;
}

void destroiFila(Fila* fila){//ESSA FUNÇÃO FUNCIONA SEMPRE - JOÃO GOSTO
    Item* primeiro = fila->primeiro;
    if(primeiro != NULL){
        while(primeiro->proximo != NULL){
            primeiro = fila->primeiro;
            fila->primeiro = primeiro->proximo;
            free(primeiro);
        }
    }
    free(fila->primeiro);
    free(fila);
}

void push(Fila* fila, Processo* p){
    Item* novoItem = (Item*) malloc(sizeof(struct item));
    novoItem->proc = p;
    novoItem->proximo = NULL;
    if(fila->primeiro == NULL){
        fila->primeiro = novoItem;
        fila->ultimo = fila->primeiro;
    } else{
        fila->ultimo->proximo = novoItem;
        fila->ultimo = novoItem;
    }
    fila->tamanho++;
}

Processo* pop(Fila* fila){
    Item* primeiro = fila->primeiro;
    if(primeiro == NULL){
        printf("A fila está vazia\n");
        return NULL;
    }
    Processo* p = primeiro->proc;
    fila->primeiro = primeiro->proximo;
    fila->tamanho--;
    free(primeiro);
    return p;
}

void printFila(Fila* fila){
    Item* item = fila->primeiro;
    if(item != NULL){
        while(item->proximo != NULL){
            printf("%d, ", item->proc->contexto.PID);
            item = item->proximo;
        }
        printf("%d\n", item->proc->contexto.PID);
    }
}

Processo* criaProcesso(int process_id, int parent_id, int t_inic, int duracao, IO tipo_io, int t_io){
    Processo* novo_processo = (Processo*) malloc(sizeof(Processo));
    novo_processo->contexto.PID = process_id;
    novo_processo->contexto.PPID = parent_id;
    novo_processo->contexto.prioridade = alta;
    novo_processo->contexto.status = ready;
    novo_processo->tempo_inicial = t_inic;
    novo_processo->duracao_restante = duracao;
    novo_processo->tipo_IO = tipo_io;
    novo_processo->tempo_inicial_IO = t_io;

    return novo_processo;
}

void insereProcesso(int tempo_atual, int probabilidade_criar_processo){
    int duracao_max_pos_io = 10;
    int criar = rand() % 100;
    if(criar > 100-probabilidade_criar_processo){
        IO tipo_io = rand()%4;
        int tempo_io = tempo_atual+rand()%5;
        int duracao_io;
        switch(tipo_io){
            case 1:
                duracao_io=DISCO;
                break;
            case 2:
                duracao_io=FITA;
                break;
            case 3:
                duracao_io=IMPRESSORA;
                break;
            default:
                duracao_io = 0;
                break;
        }
        int duracao_total = tempo_io + duracao_io + rand()%duracao_max_pos_io;
        Processo* novo_processo = criaProcesso(id++,0,tempo_atual, duracao_total, tipo_io, tempo_io);
        push(alta_prioridade, novo_processo);
    }
}

void executaProcesso(int tempo_atual){
    Processo* atual = pop(alta_prioridade);
    
}

void chamaIO();

void esperaIO();