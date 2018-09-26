#ifndef PROCESSO_H
#define PROCESSO_H

typedef struct PCB{
    int PID;
    int PPID;
    int prioridade;
    int status;

} Processo;

#endif //PROCESSO_H