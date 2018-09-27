#include "simulador.h"
#include <stdlib.h>
#include <stdio.h>

//Define variáveis globais para realização de IO
int tempo_disco = 0; //Tempo restante para disco ser liberado. Se 0, então disco está livre para IO
Processo* io_disco; // Ponteiro de referência para Processo ocupando o disco. Se NULL, disco está dlivre para IO
int tempo_fita = 0;//Tempo restante para fita ser liberada. Se 0, então fita está livre para IO
Processo* io_fita;// Ponteiro de referência para Processo ocupando a fita. Se NULL, fita está livre para IO
int tempo_impressora = 0;//Tempo restante para impressora ser liberada. Se 0, então impressora está livre para IO
Processo* io_impressora;// Ponteiro de referência para Processo ocupando a impressora. Se NULL, impressora está livre para IO

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
    if(criar > 100-probabilidade_criar_processo && (*id) < MAX_PROCESSOS){//O código asseguir é executado em probabilidade_criar_processo a cada 100 vezes
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
void executaProcesso(int tempo_atual, Fila* alta_prioridade, Fila* baixa_prioridade, Fila* fila_disco, Fila* fila_fita, Fila* fila_impressora){
    Processo* atual = pop(alta_prioridade); //Pega o primeiro processo da fila de alta prioridade
    if(atual == NULL) atual = pop(baixa_prioridade); //Se a fila de alta prioridade estiver vazia, pega o primeiro da fila de baixa prioridade

    if(atual == NULL){ //Se ambas as filas estiverem vazias
        printf("Não há processos para execução no tempo %d\n", tempo_atual);//Informa o usúario que não há processos a serem executados
        return;//Finaliza a função
    }
    int id = atual->contexto.PID;//Pega o identificador do processo
    printf("Processo %d foi selecionado para execução no tempo %d.\n", id, tempo_atual);//Informa o usuário que o processo foi criado

    if(atual->tempo_inicial_IO >= tempo_atual){//Se o processo precisa fazer IO nesse momento
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

    atual->duracao_restante--;//Executa o processo (decrementa seu tempo de execução restante)
    printf("Processo %d executado por 1 fatia de tempo.\n\n", id);//Informa a execução do processo
    if(atual->duracao_restante > 0) push(baixa_prioridade, atual);//Se o processo não acabou de ser executado, encaminha ele para fila de baixa prioridade
    else{//Se o processo acabou sua execução
        printf("Processo %d finalizado.\n\n", id);//Informa o usúario do fim da execução
        free(atual);//Libera memória do processo atual
    }
}

//Passa processos das filas de IO para execução de IO
void executaIO(Fila* fila_disco, Fila* fila_fita, Fila* fila_impressora, Fila* alta_prioridade, Fila* baixa_prioridade){

    if(tempo_disco==0){//Se o disco não está ocupado
        io_disco = pop(fila_disco);//Pega o primeiro processo da fila de disco
        if(io_disco != NULL){//Se a fila de disco não está vazia
            printf("Processo %d foi selecionado para IO de disco.\n",io_disco->contexto.PID);//Informa que o IO do processo foi iniciado  
            tempo_disco = DISCO;//Define o tempo restante para o fim do IO
        }
    }
    else{//Se o disco está ocupado
        printf("Disco ocupado com processo %d será liberado em %d fatias de tempo.\n", io_disco->contexto.PID, tempo_disco);//Informa o usúario qual processo está ocupando e o tempo restante 
        tempo_disco--;//Executa o IO (decrementa o tempo restante do disco ocupado)
        io_disco->duracao_restante--;//E decrementa a duração restante de execução do processo
        if(tempo_disco==0){//Se o IO foi finalizado
            printf("Processo %d finalizou IO de disco e voltou para fila de baixa prioridade.\n",io_disco->contexto.PID);//Informa o usúario do fim do IO 
            push(baixa_prioridade, io_disco);//Retorna o processo para a fila de baixa prioridade
        }
    }

    if(tempo_fita==0){//Se a fita não está ocupada
        io_fita = pop(fila_fita);//Pega o primeiro processo da fila de fita
        if(io_fita != NULL){//Se a fila de fita não está vazia
            printf("Processo %d foi selecionado para IO de fita.\n",io_fita->contexto.PID);//Informa que o IO do processo foi iniciado
            tempo_fita = FITA;//Define o tempo restante para o fim do IO
        }
    }
    else{//Se a fita está ocupada
        printf("Fita ocupada com processo %d será liberada em %d fatias de tempo.\n", io_fita->contexto.PID, tempo_fita);//Informa o usúario qual processo está ocupando e o tempo restante
        tempo_fita--;//Executa o IO (decrementa o tempo restante da fita ocupada)
        io_fita->duracao_restante--;//E decrementa a duração restante de execução do processo
        if(tempo_fita==0){//Se o IO foi finalizado
            printf("Processo %d finalizou IO de fita e voltou para fila de alta prioridade.\n",io_fita->contexto.PID);//Informa o usúario do fim do IO
            push(alta_prioridade, io_fita);//Retorna o processo para a fila de alta prioridade
        }
    }

    if(tempo_impressora==0){//Se a impressora não está ocupada
        io_impressora = pop(fila_impressora);//Pega o primeiro processo da fila de impressora
        if(io_impressora != NULL){//Se a fila de impressora não está vazia
            printf("Processo %d foi selecionado para IO de impressora.\n",io_impressora->contexto.PID);//Informa que o IO do processo foi iniciado 
            tempo_impressora = IMPRESSORA;//Define o tempo restante para o fim do IO
        }
    }
    else{//Se a impressora está ocupada
        printf("Impressora ocupada com processo %d será liberada em %d fatias de tempo.\n", io_impressora->contexto.PID, tempo_impressora);//Informa o usúario qual processo está ocupando e o tempo restante
        tempo_impressora--;//Executa o IO (decrementa o tempo restante da impressora ocupada)
        io_impressora->duracao_restante--;//E decrementa a duração restante de execução do processo
        if(tempo_impressora==0){//Se o IO foi finalizado
            printf("Processo %d finalizou IO de impressora e voltou para fila de alta prioridade.\n",io_impressora->contexto.PID);//Informa o usúario do fim do IO
            push(alta_prioridade, io_impressora);//Retorna o processo para a fila de alta prioridade
        }
    }
}