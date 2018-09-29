#include "simulador.h"
#include <stdlib.h>
#include <stdio.h>


//Cria uma nova estrutura de fila
Fila* criaFila(){
    Fila* novaFila = (Fila*) malloc(sizeof(Fila)); //Aloca espaço de memória para a fila
    novaFila->tamanho = 0; //Fila iniciada vazia(tamanho 0)
    novaFila->primeiro = NULL; //Primeiro elemento aponta para NULL
    novaFila->ultimo = NULL; //Último elemento aponta para NULL
}

//Limpa a memória da fila
void destroiFila(Fila* fila){
    Item* primeiro = fila->primeiro; //Pega o primeiro item da fila
    if(primeiro != NULL){//Se o primeiro elemento apontar para NULL, a fila está vazia e não há elementos para serem liberados
        while(primeiro->proximo != NULL){//Enquanto o primeiro não for o último
            primeiro = fila->primeiro; //Pega o novo primeiro elemento da fila
            fila->primeiro = primeiro->proximo; //Redefine o primeiro elemento da fila
            free(primeiro);//Libera a memória do antigo primeiro elemento
        }
        free(fila->primeiro);//Libera o último elemento da fila
    }
    free(fila);//Libera o ponteiro para a fila
}

//Adiciona processo ao fim da fila
void push(Fila* fila, Processo* p){
    Item* novoItem = (Item*) malloc(sizeof(Item)); //Aloca espaço na memória para um novo ítem
    novoItem->proc = p; //Define o Processo do ítem como o processo passado
    novoItem->proximo = NULL; //Define próximo ítem como NULL, já que este ítem será o último da fila
    if(fila->primeiro == NULL){//Se a fila estiver vazia
        fila->primeiro = novoItem;//O novo ítem será o primeiro da fila
        fila->ultimo = fila->primeiro;//E também será o último
    } else{//Se a fila não estiver vazia
        fila->ultimo->proximo = novoItem; //Novo ítem entrará depois do último
        fila->ultimo = novoItem;//O último ítem da fila é agora o novo ítem
    }
    fila->tamanho++;//A fila aumenta de tamanho
}
//Remove primeiro elemento da fila
Processo* pop(Fila* fila){
    Item* primeiro = fila->primeiro; //Pega primeiro ítem da fila
    if(primeiro == NULL){//Se a fila está vazia
        return NULL;//Retorna ponteiro para NULL
    }
    Processo* p = primeiro->proc;//Pega o processo do primeiro ítem
    fila->primeiro = primeiro->proximo;//O primeiro da fila agora deve ser o segundo
    fila->tamanho--;//A fila diminui de tamanho
    free(primeiro);//Libera memória do primeiro elemento
    return p;//Retorna o processo
}

//Retorna 1 sse a fila estiver vazia
int filaVazia(Fila* fila){
    return fila->primeiro==NULL;
}

//Imprime a fila na tela
void printFila(Fila* fila){
    Item* item = fila->primeiro; //Pega primeiro ítem da fila
    if(item != NULL){//Se a fila não está vazia
        while(item->proximo != NULL){//Enquanto o ítem não for o último
            printf("(%d, %d tempos), ", item->proc->contexto.PID, item->proc->duracao_restante);//Imprime o PID e o tempo restante do processo
            item = item->proximo;//Pega o próximo ítem
        }
        printf("(%d,%d tempos)\n", item->proc->contexto.PID, item->proc->duracao_restante);//Imprime o último processo
    }else printf("\n");//Se a fila estiver vazia, imprime linha vazia
}

//Inicializa um novo processo
Processo* criaProcesso(int process_id, int parent_id, int t_inic, int duracao, IO tipo_io, int t_io){
    Processo* novo_processo = (Processo*) malloc(sizeof(Processo));//Aloca memória para novo processo
    //Inicializa as variáveis do processo
    novo_processo->contexto.PID = process_id;
    novo_processo->contexto.PPID = parent_id;
    novo_processo->contexto.prioridade = alta;
    novo_processo->contexto.status = ready;
    novo_processo->tempo_inicial = t_inic;
    novo_processo->duracao_restante = duracao;
    novo_processo->tipo_IO = tipo_io;
    novo_processo->tempo_inicial_IO = t_io;

    return novo_processo;//Retorna o processo
}

//Cria ou não um novo processo no intervalo de tempo atual com uma certa probabilidade 
void insereProcesso(int tempo_atual, int* id, Fila* alta_prioridade, int probabilidade_criar_processo){
    int duracao_max_pos_io = 10;//Define número máximo de fatias de tempo que o processo pode ocupar após o fim do IO
    IO tipo_io; //Declara o tipo de IO 
    int tempo_io; //Declara o tempo de inicio do IO
    int duracao_io; //Declara duração de IO
    int duracao_total; //Declara duração total

    int criar = rand() % 100; //Gera um número aleatório entre 0 e 100
    if(criar > 100-probabilidade_criar_processo && (*id) <= MAX_PROCESSOS){//O código asseguir é executado em probabilidade_criar_processo a cada 100 vezes
        tipo_io = rand()%4;//Gera um número aleatório entre 0 e 4. Tipo de IO mapeado na tabela em simulador.h
        tempo_io = tempo_atual+rand()%5;//IO pode iniciar entre o início da execução e o 5 fatias de tempo após o início
        switch(tipo_io){//Mapeia tempos de IO segundo tipos. Constantes definidas em simulador.h
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
        duracao_total = tempo_io + duracao_io + rand()%duracao_max_pos_io;//Define duração máxima aleatória.
        Processo* novo_processo = criaProcesso(*id,0,tempo_atual, duracao_total, tipo_io, tempo_io);//Cria o novo processo com os parâmetros definidos acima.
        push(alta_prioridade, novo_processo);//Insere o novo processo na fila de alta prioridade

        //Imprime informações sobre o processo criado para o usuário
        printf("Processo %d chegou no tempo %d e entrou  na fila de alta prioridade.\n", novo_processo->contexto.PID, tempo_atual);
        printf("Processo %d: \n\t"
                    "Prioridade: %d\n\t"
                    "Status: %d\n\t" 
                    "Duração: %d\n\t"
                    "Faz IO do tipo %d a partir do tempo %d\n\n",
                    novo_processo->contexto.PID, 
                    novo_processo->contexto.prioridade, 
                    novo_processo->contexto.status,
                    novo_processo->duracao_restante,
                    novo_processo->tipo_IO,
                    novo_processo->tempo_inicial_IO);

        (*id)++;//Incrementa o identificador de processos sequencial
    }

}

//Executar processos das filas
void executaProcesso(int tempo_atual, Processo** executando, Fila* alta_prioridade, Fila* baixa_prioridade, Fila* fila_disco, Fila* fila_fita, Fila* fila_impressora){
    Processo* atual = pop(alta_prioridade); //Pega o primeiro processo da fila de alta prioridade
    if(atual == NULL) atual = pop(baixa_prioridade); //Se a fila de alta prioridade estiver vazia, pega o primeiro da fila de baixa prioridade

    if(atual == NULL){ //Se ambas as filas estiverem vazias
        printf("Não há processos para execução no tempo %d\n", tempo_atual);//Informa o usúario que não há processos a serem executados
        return;//Finaliza a função
    }
    int id = atual->contexto.PID;//Pega o identificador do processo
    int prioridade_atual = atual->contexto.prioridade;
    printf("Processo %d foi selecionado para execução no tempo %d.\n", id, tempo_atual);//Informa o usuário que o processo foi criado

    if(atual->tempo_inicial_IO <= tempo_atual){//Se o processo precisa fazer IO nesse momento
        atual->contexto.status = blocked;//Redefine status
        atual->contexto.prioridade = io;//Redefine prioridade
        switch(atual->tipo_IO){//Encaminha o processo para a fila de IO apropriada
            case 1:
                push(fila_disco, atual);
                printf("Processo %d enviado para fila de disco.\n",id);
                return;//Finaliza a função
            case 2:
                push(fila_fita, atual);
                printf("Processo %d enviado para fila de fita.\n",id);
                return;//Finaliza a função
            case 3:
                push(fila_impressora, atual);
                printf("Processo %d enviado para fila de impressora.\n",id);
                return;//Finaliza a função
        }
    }

    atual->contexto.status = running;//Redefine status
    atual->contexto.prioridade = prioridade_atual;//Redefine prioridade
    *executando = atual;
    (*executando)->duracao_restante--;//Executa o processo (decrementa seu tempo de execução restante)
}

//Retorna os processos executados para a fila de baixa prioridade
void terminaExecucaoProcesso(Processo** executando, Fila* baixa_prioridade){
    if(*executando != NULL){//Se existe um processo roddando
        int id = (*executando)->contexto.PID;
        printf("Processo %d executado por 1 fatia de tempo.\n\n", id);//Informa a execução do processo
        if((*executando)->duracao_restante > 0){ //Se o processo não acabou de ser executado
            (*executando)->contexto.status = ready;//Redefine status
            (*executando)->contexto.prioridade = baixa;//Redefine prioridade
            push(baixa_prioridade, *executando);//Encaminha ele para fila de baixa prioridade
            *executando = NULL;//Define que não há processo rodando
        }
        else{//Se o processo acabou sua execução
            printf("Processo %d finalizado.\n\n", id);//Informa o usúario do fim da execução
            free(*executando);//Libera memória do processo atual
        }
    }
}

//Passa processos das filas de IO para execução de IO
void executaIO(IO tipo_io, int* tempo_restante_io, Processo** processo_io, Fila* fila_io){

    if(*tempo_restante_io==0){//Se o dispositivo não está ocupado
        *processo_io = pop(fila_io);//Pega o primeiro processo da fila do dispositivo
        if(*processo_io != NULL){//Se a fila do dispositivo não está vazia
            printf("Processo %d foi selecionado para IO de ",(*processo_io)->contexto.PID);//Informa que o IO do processo foi iniciado  
            switch(tipo_io){//Define o tempo restante para o fim do IO
                case 1:
                    *tempo_restante_io = DISCO;
                    printf("disco.\n");
                    break;
                case 2:
                    *tempo_restante_io = FITA;
                    printf("fita.\n");
                    break;
                case 3:
                    *tempo_restante_io = IMPRESSORA;
                    printf("impressora.\n");
                    break;
            }
        }
    }
    else{//Se o dispositivo está ocupado
        switch(tipo_io){//Informa o tipo de IO ocupado
                case 1:
                    printf("Disco ");
                    break;
                case 2:
                    printf("Fita ");
                    break;
                case 3:
                    printf("Impressora ");
                    break;
            }
        printf("tratando o processo %d estará livre em %d fatias de tempo.\n", (*processo_io)->contexto.PID, *tempo_restante_io);//Informa o usúario qual processo está ocupando e o tempo restante 
        (*tempo_restante_io)--;//Executa o IO (decrementa o tempo restante do dispositivo ocupado)
        (*processo_io)->duracao_restante--;//E decrementa a duração restante de execução do processo
    }
}

//Retorna os processos que executaram IO para as filas de execução
void terminaExecucaoIO(IO tipo_io, int* tempo_restante_io, Processo** processo_io, Fila* fila_retorno){

    if(*tempo_restante_io==0 && *processo_io!=NULL){//Se existe IO em execução e o IO está finalizado 
            printf("Processo %d finalizou IO de ",(*processo_io)->contexto.PID);//Informa o usúario do fim do IO 
            switch(tipo_io){//Informa o tipo de IO finalizado
                case 1:
                    (*processo_io)->contexto.prioridade = baixa;//Redefine prioridade
                    printf("disco ");
                    break;
                case 2:
                    (*processo_io)->contexto.prioridade = alta;//Redefine prioridade
                    printf("fita ");
                    break;
                case 3:
                    (*processo_io)->contexto.prioridade = alta;//Redefine prioridade
                    printf("impressora ");
                    break;
            }
            printf("e voltou para fila de execução.\n");
            (*processo_io)->contexto.status = ready;//Redefine status
            (*processo_io)->tipo_IO=0;//Define que não há mais IO para ser executado neste processo
            push(fila_retorno, *processo_io);//Retorna o processo para a fila de execução
            *processo_io = NULL;
        }

}

//Retorna 1 sse a simulação pode ser terminada
int fim_simulacao(int tempo_disco, int tempo_fita, int tempo_impressora, int id, Fila* alta_prioridade, Fila* baixa_prioridade, Fila* fila_disco, Fila* fila_fita, Fila* fila_impressora){
    //Se todas as filas estão vazias
    int filas_vazias = filaVazia(alta_prioridade) && filaVazia(baixa_prioridade) && filaVazia(fila_disco) && filaVazia(fila_fita) && filaVazia(fila_impressora);
    //Se o máximo de processos foi criado
    int max_processos_criados = id>=MAX_PROCESSOS;
    //Se não há io acontecendo
    int io_livre = tempo_disco==0 && tempo_fita==0 && tempo_impressora==0;
    //Então a simulação pode ser terminada
    return filas_vazias && max_processos_criados && io_livre;
}
