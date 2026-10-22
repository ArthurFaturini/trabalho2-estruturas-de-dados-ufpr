#include <stdio.h>
#include <stdlib.h>
#include "pilha.h"

/* Inicializa a pilha apontando o topo para NULL */
void inicializarPilha(Pilha *p)
{
    if (p != NULL)
    {
        p->topo = NULL;
    }
}

/* Retorna true se a pilha estiver vazia */
bool isEmpty(Pilha *p)
{
    if (p == NULL)
        return true;
    return (p->topo == NULL);
}

/* Empilha um caractere (operador ou parêntese) */
bool pushChar(Pilha *p, char c)
{
    if (p == NULL)
        return false;

    NoPilha *novo = (NoPilha *)malloc(sizeof(NoPilha));
    if (novo == NULL)
    {
        printf("Erro: Falha na alocacao de memoria (pushChar).\n");
        return false;
    }

    novo->dado.carac = c;
    novo->tipo = TIPO_CHAR;
    novo->prox = p->topo;
    p->topo = novo;

    return true;
}

/* Empilha um número real (operando ou resultado) */
bool pushDouble(Pilha *p, double val)
{
    if (p == NULL)
        return false;

    NoPilha *novo = (NoPilha *)malloc(sizeof(NoPilha));
    if (novo == NULL)
    {
        printf("Erro: Falha na alocacao de memoria (pushDouble).\n");
        return false;
    }

    novo->dado.numero = val;
    novo->tipo = TIPO_DOUBLE;
    novo->prox = p->topo;
    p->topo = novo;

    return true;
}

/* Desempilha um caractere */
bool popChar(Pilha *p, char *out)
{
    if (isEmpty(p))
        return false;

    NoPilha *temp = p->topo;
    if (out != NULL)
    {
        *out = temp->dado.carac;
    }

    p->topo = temp->prox;
    free(temp);

    return true;
}

/* Desempilha um número real */
bool popDouble(Pilha *p, double *out)
{
    if (isEmpty(p))
        return false;

    NoPilha *temp = p->topo;
    if (out != NULL)
    {
        *out = temp->dado.numero;
    }

    p->topo = temp->prox;
    free(temp);

    return true;
}

/* Consulta o elemento do topo sem desempilhar (para char) */
bool topChar(Pilha *p, char *out)
{
    if (isEmpty(p) || out == NULL)
        return false;
    *out = p->topo->dado.carac;
    return true;
}

/* Consulta o elemento do topo sem desempilhar (para double) */
bool topDouble(Pilha *p, double *out)
{
    if (isEmpty(p) || out == NULL)
        return false;
    *out = p->topo->dado.numero;
    return true;
}

/* Desaloca todos os nós da pilha para evitar Memory Leaks */
void liberarPilha(Pilha *p)
{
    if (p == NULL)
        return;

    NoPilha *atual = p->topo;
    while (atual != NULL)
    {
        NoPilha *temp = atual;
        atual = atual->prox;
        free(temp);
    }

    p->topo = NULL;
}