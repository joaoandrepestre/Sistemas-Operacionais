#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>
#include "memoria.h"
#include "processo.h"
#include "fila.h"

#define MAX_MEMORY_FRAMES 64 //número máximo de frames na memória principal
#define MAX_PROCESSOS 20     //número máximo de processos
#define BUFFER_SIZE 400      // tamanho do buffer para log
#define BUFFER_LINE_SIZE 100 // tamanho da linha do buffer

void gerenciaMemoria(Memoria *mem_principal, Memoria *mem_virtual, Processo **processos, Fila *fila_processos, Processo *p, int pag, char **logger_buffer, int *logger_line);
// Lida com page faults

int swapOutProcessos(Memoria *mem_principal, Memoria *mem_virtual, Processo **processos, Fila *fila_processos, Processo *p);
// Faz swap-out no processo mais antigo para colocar a página nova na memória principal

void swapInProcessos(Memoria *mem_principal, Memoria *mem_virtual, Processo **processos, Fila *fila_processos, Processo *p, char **logger_buffer, int *logger_line);
// Faz swap-in no processo que sofreu swap-out

void printLog(WINDOW *logger, char **logger_buffer, int logger_scroll);
// Imprime o conteúdo do buffer na janela de log

void clearLog(WINDOW *logger, char **logger_buffer, int logger_start, int *logger_line);
// Limpa o conteúdo mais antigo do buffer e move o conteúdo mais novo para o inicio do buffer

int main()
{
    // Variáveis para o simulador
    int i;       // variável de iteração
    int id = 1;  // id sequencial dos processos
    int t = 0;   // instante de tempo da simulação
    int pag;     // página que o processo solicitou
    int fim = 0; // Marca o fim do simulador se igual a 1
    int key;     // Tecla clicada

    // Inicializa semente para valores aleatór
    time_t seed;
    srand((unsigned)time(&seed));

    // Inicializa biblioteca para print organizado
    initscr();
    noecho();
    curs_set(FALSE);

    // Inicializa janela de log
    WINDOW *logger = newwin(LINES - 10, COLS, 10, 0);                     // Janela do log
    scrollok(logger, FALSE);                                              // Permite scroll na janela de log
    keypad(logger, TRUE);                                                 // Permite captura do teclado na janela de log
    char **logger_buffer = (char **)malloc(sizeof(char *) * BUFFER_SIZE); // Buffer do log
    // Buffer do log inicializado com 0
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        logger_buffer[i] = (char *)calloc(BUFFER_LINE_SIZE, sizeof(char));
    }
    int logger_line = 0;   // Linha atual do log
    int logger_start = 0;  // Ponto em que o log começa o tempo atual
    int logger_scroll = 0; // Ponto em que o log começa a impressão

    Memoria *mem_principal = criaMemoria(MAX_MEMORY_FRAMES);                        //Memória principal
    Memoria *mem_virtual = criaMemoria(MAX_MEMORY_FRAMES * MAX_PROCESSOS);          //Memória virtual
    Processo **processos = (Processo **)malloc(sizeof(Processo *) * MAX_PROCESSOS); // processos criados
    Fila *fila_processos = criaFila();                                              //Fila de processos para escolher o processo que sofrerá swap-out

    while (!fim)
    {
        // Mostra a o estado das memórias principal e virtual
        mvprintw(0, 0, "Memória principal:");
        refresh();
        printMemoria(mem_principal, 0, 1);

        // Mostra na tela os comandos
        mvprintw(0, 110, "Comandos:");
        mvprintw(1, 110, "ESC - Stop");
        mvprintw(2, 110, "SPACE - Pause");
        mvprintw(3, 110, "RIGHT ARROW - Skip");
        refresh();

        // Inicio da impressão da janela de log
        mvprintw(9, 0, "Log:");
        refresh();
        sprintf(logger_buffer[logger_line], "T = %d\n", t); // Imprime o tempo atual
        logger_start = logger_line;                         // Define o ponto de inicio do log como o fim do log passado
        logger_scroll = logger_start;                       // Define o inicio do print do log no ponto inicial do log
        logger_line++;                                      // Incrementa a linha atual do log

        // Cria um novo processo
        if (id <= MAX_PROCESSOS) // Se o número máximo de processos ainda não foi atingido
        {
            processos[id - 1] = criaProcesso(id, mem_principal, mem_virtual); // Cria um novo processo, aleatóriamente
            push(fila_processos, id);
            printProcesso(processos[id - 1], logger_buffer, &logger_line); // Imprime  o processo criado no log                                         // Insere o processo na fila
            id++;
        }

        //Para todos os processos criados
        for (i = 0; i < id - 1; i++)
        {
            // Solicita uma página aleatóriamente
            pag = solicitaPagina(processos[i], mem_principal, mem_virtual, logger_buffer, &logger_line);
            if (pag != -1)
            {
                // Se ocorreu page fault, chama o gerenciador de memória para resolver
                gerenciaMemoria(mem_principal, mem_virtual, processos, fila_processos, processos[i], pag, logger_buffer, &logger_line);
            }

            // Manda o processo para o fim da fila
            paraFim(fila_processos, processos[i]->PID);
        }

        // Imprime o log
        printLog(logger, logger_buffer, logger_scroll);

        // Lida com o teclado
        wtimeout(logger, 3000); // Espera 3 segundos ou o input do usuário
        switch (key = wgetch(logger))
        {
        case 27:     // ESC
            fim = 1; // Interrompe o simulador se o usuário apertar ESC
            break;
        case ' ': // Pausa o simulador caso o usuário apertar espaço
            // Executado quando o simulador está pausado
            wclear(stdscr);
            // Mostra a o estado das memórias principal e virtual
            mvprintw(0, 0, "Memória principal:");
            printMemoria(mem_principal, 0, 1);

            // Mostra na tela os comandos
            mvprintw(0, 110, "Comandos:");
            mvprintw(1, 110, "ESC - Stop");
            mvprintw(2, 110, "SPACE - Play");
            mvprintw(3, 110, "UP ARROW - Scroll up");
            mvprintw(4, 110, "DOWN ARROW - Scroll down");

            mvprintw(9, 0, "Log:");
            refresh();

            printLog(logger, logger_buffer, logger_scroll);

            wtimeout(logger, -1); // Espera o input do usuário
            key = wgetch(logger);
            while (key != ' ')
            { // Quando o usuário apertar espaço novamente, retoma a simulação
                switch (key = wgetch(logger))
                {
                case 27:     // ESC
                    fim = 1; // Interrompe o simulador se o usuário apertar ESC
                    break;
                case KEY_UP:                                                       // Seta para cima ou roda do mouse
                    logger_scroll = logger_scroll - 1 < 0 ? 0 : logger_scroll - 1; // Sobe o scroll do log
                    break;
                case KEY_DOWN:       // Seta para baixo ou roda do mouse
                    logger_scroll++; // Desce o scroll do log
                    break;
                }
                if (fim)
                    break;
                // Imprime o log
                printLog(logger, logger_buffer, logger_scroll); // Imprime o log na nova posição do scroll
            }
            break;
        case KEY_RIGHT: // Seta para direita
            break;      // Pula para próxima iteração
        default:        // Qualquer outra tecla
            sleep(1);   // Espera um segundo antes de prosseguir a simulação
            break;
        }

        if (t > 0)                                                       // A partir da segunda iteração
            clearLog(logger, logger_buffer, logger_start, &logger_line); // Limpa o log
        clear();                                                         // Limpa a tela

        t++; // Incrementa o tempo
    }

    // Executado quando o simulador é interrompido
    wclear(logger);                              // Limpa o log
    wprintw(logger, "Simulação interrompida\n"); // Informa o fim da simulação
    wrefresh(logger);
    sleep(1); // Espera 3 segundos

    // Limpeza de memória
    destroiMemoria(mem_principal);
    destroiMemoria(mem_virtual);
    for (i = 0; i < id - 1; i++)
        destroiProcesso(processos[i]);
    free(processos);
    destroiFila(fila_processos);
    free(logger_buffer);

    // Finaliza janela de print organizado
    endwin();
}

// Lida com page faults
void gerenciaMemoria(Memoria *mem_principal, Memoria *mem_virtual, Processo **processos, Fila *fila_processos, Processo *p, int pag, char **logger_buffer, int *logger_line)
{
    int frame;
    int proc_swap;

    // Checa se a página pedida sofreu swap-out
    if (p->page_table->paginas[pag].S)
    {
        // SWAP-IN DO PROCESSO
        sprintf(logger_buffer[*logger_line], "A página %d sofreu swap-out, deve ser feito swap-in do processo %d.\n", pag, p->PID);
        (*logger_line) ++;
        swapInProcessos(mem_principal, mem_virtual, processos, fila_processos, p, logger_buffer, logger_line);
        return;
    }

    // Checa se está dentro do working set limit
    if (p->fila_paginas->tamanho < WSL)
    {
        // Checa se há espaço na memória principal
        if (mem_principal->tam_free_frames > 0)
        {
            // Adiciona o frame na memória principal
            frame = addPageToMemory(p, mem_principal, pag, presente, nao_swaped);
            sprintf(logger_buffer[*logger_line], "O gerenciador de memória inseriu a página %d do processo %d no frame %d da memória principal.\n\n", pag, p->PID, frame);
            (*logger_line) += 2;
            return;
        }
        // Se não houver espaço na memória principal
        //SWAP-OUT DE OUTRO PROCESSO
        sprintf(logger_buffer[*logger_line], "Não há espaço na memória principal, outro processo deve sofrer swap-out.\n");
        (*logger_line)++;
        proc_swap = swapOutProcessos(mem_principal, mem_virtual, processos, fila_processos, p);
        sprintf(logger_buffer[*logger_line], "O processo %d sofreu swap-out.\n", proc_swap);
        (*logger_line)++;
        frame = addPageToMemory(p, mem_principal, pag, presente, nao_swaped);
        sprintf(logger_buffer[*logger_line], "O gerenciador de memória inseriu a página %d do processo %d no frame %d da memória principal.\n\n", pag, p->PID, frame);
        (*logger_line) += 2;
        return;
    }

    // Se atingiu o WSL, fazer LRU
    sprintf(logger_buffer[*logger_line], "WORKING SET LIMIT do processo %d atingido.\n", p->PID);
    (*logger_line)++;
    frame = swapPagesLRU(p, mem_principal, mem_virtual, pag);
    sprintf(logger_buffer[*logger_line], "O gerenciador de memória fez LRU no processo e inseriu a página %d no frame %d da memória principal.\n\n", pag, frame);
    (*logger_line) += 2;
}

int swapOutProcessos(Memoria *mem_principal, Memoria *mem_virtual, Processo **processos, Fila *fila_processos, Processo *p)
{
    int i, j, lixo;
    int proc_remov;

    // Pega processo mais antigo diferente de p e que não sofreu swap-out
    proc_remov = pop(fila_processos);
    // Enquanto o processo mais antigo for igual a p ou já tiver sofrido swap-out
    while (proc_remov == p->PID || processos[proc_remov - 1]->S)
    {
        // Põe ele no fim da fila
        push(fila_processos, proc_remov);
        // Pega o próximo da fila
        proc_remov = pop(fila_processos);
    }

    // Define bit S como swaped
    processos[proc_remov - 1]->S = swaped;

    // Encontra todas as páginas desse processo que estão na memória principal
    j = 0;
    for (i = 0; i < processos[proc_remov - 1]->page_table->tam; i++)
    {
        if (j == WSL)
            break;
        if (processos[proc_remov - 1]->page_table->paginas[i].P)
        {
            // Remove da memória principal
            removeMemoryFrame(mem_principal, processos[proc_remov - 1]->page_table->paginas[i].frame);
            // Adiciona a memória virtual
            addPageToMemory(processos[proc_remov - 1], mem_virtual, i, ausente, swaped);
            // Remove uma página da fila de páginas
            pop(processos[proc_remov - 1]->fila_paginas);
            j++;
        }
    }
    // Insere o processo removido na fila de processos novamente
    push(fila_processos, proc_remov);

    return proc_remov;
}

// Faz swap-in no processo que sofreu swap-out
void swapInProcessos(Memoria *mem_principal, Memoria *mem_virtual, Processo **processos, Fila *fila_processos, Processo *p, char **logger_buffer, int *logger_line)
{
    int i, j, frame;

    int *pag_recoloc = (int *)malloc(sizeof(int) * WSL);
    for (i = 0; i < WSL; i++)
        pag_recoloc[i] = -1;

    // Encontrar todas as páginas do processo que sofreram swap-out
    j = 0;
    for (i = 0; i < p->page_table->tam; i++)
    {
        // Se já foram encontradas WSL páginas, para a busca
        if (j == WSL)
            break;
        // Se a página sofreu swap-out
        if (p->page_table->paginas[i].S)
        {
            p->page_table->paginas[i].S = nao_swaped;
            gerenciaMemoria(mem_principal, mem_virtual, processos, fila_processos, p, i, logger_buffer, logger_line);
            pag_recoloc[j] = i; // Coloca a página no grupo de páginas que sofreram swap-out
            j++;                // Incrementao contador de páginas encontradas
        }
    }

    // Redefine bit S como nao_swaped
    p->S = nao_swaped;
}

// Imprime o conteúdo do buffer na janela de log
void printLog(WINDOW *logger, char **logger_buffer, int logger_scroll)
{
    int i;
    wclear(logger); // Limpa a tela
    for (i = 0; i < LINES - 10; i++)
    {                                                      // Para todas as linhas que cabem na janela
        wprintw(logger, logger_buffer[logger_scroll + i]); // Imprime a linha atual
        wrefresh(logger);
    }
}

// Limpa o conteúdo mais antigo do buffer e move o conteúdo mais novo para o inicio do buffer
void clearLog(WINDOW *logger, char **logger_buffer, int logger_start, int *logger_line)
{
    int i, j;
    int fim = (*logger_line) - logger_start;

    // Apaga o conteúdo mais antigo do buffer
    for (i = 0; i < logger_start; i++)
    {
        for (j = 0; j < BUFFER_LINE_SIZE; j++)
        {
            logger_buffer[i][j] = 0;
        }
    }

    // Copia o conteudo mais novo para o início do buffer
    for (i = 0; i < fim; i++)
    {
        for (j = 0; j < BUFFER_LINE_SIZE; j++)
        {
            logger_buffer[i][j] = logger_buffer[logger_start + i][j];
        }
    }

    // Apaga o fim do buffer
    for (i = fim; i < BUFFER_SIZE; i++)
    {
        for (j = 0; j < BUFFER_LINE_SIZE; j++)
        {
            logger_buffer[i][j] = 0;
        }
    }

    // Redefine a linha atual como o fim do conteúdo movido
    (*logger_line) = fim;

    // Limpa a tela
    wclear(logger);
}
