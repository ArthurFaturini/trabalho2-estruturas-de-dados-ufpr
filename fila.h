#ifndef FILA_H
#define FILA_H

/* Maior IP possivel: "255.255.255.255" = 15 caracteres + '\0' */
#define TAM_IP 16

/* Um no da fila: uma solicitacao de impressao */
typedef struct NoFila
{
    int prioridade;      /* 1 = maxima, 2 = normal, 3 = baixa */
    char ip[TAM_IP];     /* identificacao do computador */
    struct NoFila *prox; /* endereco do proximo no */
} NoFila;

/* Controle da fila: guarda o primeiro e o ultimo no */
typedef struct
{
    NoFila *inicio;
    NoFila *fim;
    int quantidade;
} Fila;

void inicializarFila(Fila *f);
int filaVazia(Fila *f);
int enfileirar(Fila *f, int prioridade, char ip[]);
int desenfileirar(Fila *f, int *prioridade, char ip[]);
void imprimirFila(Fila *f, char nome[]);
void liberarFila(Fila *f);

#endif
