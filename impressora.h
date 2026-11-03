#ifndef IMPRESSORA_H
#define IMPRESSORA_H

#include "fila.h"

/* Quantas solicitacoes saem da fila de entrada a cada 0.5 segundo */
#define TAMANHO_LOTE 5

int carregarSolicitacoes(char nomeArquivo[], Fila *entrada);
void distribuirLote(Fila *entrada, Fila *fila0, Fila *fila1, Fila *fila2);
void exibirFilas(Fila *entrada, Fila *fila0, Fila *fila1, Fila *fila2);
void simularDistribuicao(Fila *entrada, Fila *fila0, Fila *fila1, Fila *fila2);
void esperarMeioSegundo(void);

#endif
