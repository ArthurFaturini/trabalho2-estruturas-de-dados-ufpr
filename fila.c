#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fila.h"

/* Deixa a fila vazia, sem nenhum no */
void inicializarFila(Fila *f)
{
    f->inicio = NULL;
    f->fim = NULL;
    f->quantidade = 0;
}

/* Retorna 1 se a fila estiver vazia e 0 caso contrario */
int filaVazia(Fila *f)
{
    return f->inicio == NULL;
}

/* Insere uma solicitacao no FIM da fila.
 * Retorna 1 se deu certo e 0 se faltou memoria. */
int enfileirar(Fila *f, int prioridade, char ip[])
{
    NoFila *novo = (NoFila *)malloc(sizeof(NoFila));

    if (novo == NULL)
        return 0;

    novo->prioridade = prioridade;
    strcpy(novo->ip, ip);
    novo->prox = NULL;

    if (filaVazia(f))
        f->inicio = novo;    /* fila vazia: o novo e o primeiro */
    else
        f->fim->prox = novo; /* senao: liga o antigo ultimo ao novo */

    f->fim = novo;
    f->quantidade++;
    return 1;
}

/* Remove a solicitacao do INICIO da fila e devolve seus dados
 * pelos parametros. Retorna 1 se removeu e 0 se a fila estava vazia. */
int desenfileirar(Fila *f, int *prioridade, char ip[])
{
    NoFila *removido;

    if (filaVazia(f))
        return 0;

    removido = f->inicio;
    *prioridade = removido->prioridade;
    strcpy(ip, removido->ip);

    f->inicio = removido->prox;
    if (f->inicio == NULL) /* a fila ficou vazia */
        f->fim = NULL;

    free(removido);
    f->quantidade--;
    return 1;
}

/* Mostra todos os IPs da fila, do inicio ao fim, sem remover nada */
void imprimirFila(Fila *f, char nome[])
{
    NoFila *atual = f->inicio;

    printf("%s (%d): ", nome, f->quantidade);

    if (filaVazia(f))
        printf("vazia");

    while (atual != NULL)
    {
        printf("%s ", atual->ip);
        atual = atual->prox;
    }
    printf("\n");
}

/* Remove todos os nos, liberando a memoria */
void liberarFila(Fila *f)
{
    int prioridade;
    char ip[TAM_IP];

    while (desenfileirar(f, &prioridade, ip))
        ; /* desenfileirar ja faz o free de cada no */
}
