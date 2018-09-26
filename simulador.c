#include "simulador.h"
#include <stdlib.h>

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
    Processo* p = primeiro->proc;
    fila->primeiro = primeiro->proximo;
    fila->tamanho--;
    free(primeiro);
    return p;
}

void criaProcessos();

void executaProcesso();

void chamaIO();

void esperaIO();