#include "fila.h"
#include <stdlib.h>
#include <stdio.h>

//Cria uma nova estrutura de fila
Fila *criaFila()
{
    Fila *novaFila = (Fila *)malloc(sizeof(Fila)); //Aloca espaço de memória para a fila
    novaFila->tamanho = 0;                         //Fila iniciada vazia(tamanho 0)
    novaFila->primeiro = NULL;                     //Primeiro elemento aponta para NULL
    novaFila->ultimo = NULL;                       //Último elemento aponta para NULL
}

//Limpa a memória da fila
void destroiFila(Fila *fila)
{
    Item *primeiro = fila->primeiro; //Pega o primeiro item da fila
    if (primeiro != NULL)
    { //Se o primeiro elemento apontar para NULL, a fila está vazia e não há elementos para serem liberados
        while (primeiro->proximo != NULL)
        {                                       //Enquanto o primeiro não for o último
            primeiro = fila->primeiro;          //Pega o novo primeiro elemento da fila
            fila->primeiro = primeiro->proximo; //Redefine o primeiro elemento da fila
            free(primeiro);                     //Libera a memória do antigo primeiro elemento
        }
        free(fila->primeiro); //Libera o último elemento da fila
    }
    free(fila); //Libera o ponteiro para a fila
}

//Adiciona processo ao fim da fila
void push(Fila *fila, int pag)
{
    Item *novoItem = (Item *)malloc(sizeof(Item)); //Aloca espaço na memória para um novo ítem
    novoItem->pagina = pag;                        //Define a página do ítem como a página passado
    novoItem->proximo = NULL;                      //Define próximo ítem como NULL, já que este ítem será o último da fila
    if (fila->primeiro == NULL)
    {                                  //Se a fila estiver vazia
        fila->primeiro = novoItem;     //O novo ítem será o primeiro da fila
        fila->ultimo = fila->primeiro; //E também será o último
    }
    else
    {                                     //Se a fila não estiver vazia
        fila->ultimo->proximo = novoItem; //Novo ítem entrará depois do último
        fila->ultimo = novoItem;          //O último ítem da fila é agora o novo ítem
    }
    fila->tamanho++; //A fila aumenta de tamanho
}
//Remove primeiro elemento da fila
int pop(Fila *fila)
{
    Item *primeiro = fila->primeiro; //Pega primeiro ítem da fila
    if (primeiro == NULL)
    {              //Se a fila está vazia
        return -1; //Retorna -1
    }
    int pag = primeiro->pagina;         //Pega a página do primeiro ítem
    fila->primeiro = primeiro->proximo; //O primeiro da fila agora deve ser o segundo
    fila->tamanho--;                    //A fila diminui de tamanho
    free(primeiro);                     //Libera memória do primeiro elemento
    return pag;                         //Retorna o processo
}

//Move página pag para o fim da fila
void paraFim(Fila *fila, int pag)
{
    Item *item = fila->primeiro; //Pega primeiro ítem da fila
    Item *ant;
    if (item == NULL)
    { //Se a fila está vazia
        return;
    }

    while (item != NULL)
    {
        if (item->pagina == pag)
            break; //Se o ítem é o correto, interrompe o loop
        ant = item;
        item = item->proximo;
    }

    // Se o ítem já for o último, então já está resolvido
    if (item == fila->ultimo)
        return;

    if (item == fila->primeiro)
    {                                   //Se o ítem correto for o primeiro
        fila->primeiro = item->proximo; // o primeiro é o próximo
        item->proximo = NULL;           //O próximo do correto é nulo
        fila->ultimo->proximo = item;   //O próximo do fim da fila é o correto
        fila->ultimo = item;            // O último da fila é o correto
        return;
    }

    // Se o ítem não for o primeiro
    ant->proximo = item->proximo; //O próximo do anterior é o próximo do correto
    item->proximo = NULL;         //O próximo do correto é nulo
    fila->ultimo->proximo = item; //O próximo do fim da fila é o correto
    fila->ultimo = item;          // O último da fila é o correto
}

//Retorna 1 sse a fila estiver vazia
int filaVazia(Fila *fila)
{
    return fila->primeiro == NULL;
}

//Imprime a fila na tela
void printFila(Fila *fila)
{
    Item *item = fila->primeiro; //Pega primeiro ítem da fila
    if (item != NULL)
    { //Se a fila não está vazia
        while (item->proximo != NULL)
        {                                       //Enquanto o ítem não for o último
            printf("{id: %d}, ", item->pagina); //Imprime o ID da página
            item = item->proximo;               //Pega o próximo ítem
        }
        printf("{id: %d}\n\n", item->pagina); //Imprime a última página
    }
    else
        printf("\n\n"); //Se a fila estiver vazia, imprime linha vazia
}