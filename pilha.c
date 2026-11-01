#include <stdio.h>
#include <stdlib.h>
#include "pilha.h"

void inicializarPilha(Pilha *p)
{
    if (p != NULL)
    {
        p->topo = NULL;
    }
}

bool isEmpty(Pilha *p)
{
    if (p == NULL)
        return true;
    return (p->topo == NULL);
}

bool pushChar(Pilha *p, char c)
{
    if (p == NULL)
        return false;
    NoPilha *novo = (NoPilha *)malloc(sizeof(NoPilha));
    if (novo == NULL)
        return false;

    novo->carac = c;
    novo->tipo = 0; // 0 indica char
    novo->prox = p->topo;
    p->topo = novo;
    return true;
}

bool pushDouble(Pilha *p, double val)
{
    if (p == NULL)
        return false;
    NoPilha *novo = (NoPilha *)malloc(sizeof(NoPilha));
    if (novo == NULL)
        return false;

    novo->numero = val;
    novo->tipo = 1; // 1 indica double
    novo->prox = p->topo;
    p->topo = novo;
    return true;
}

bool popChar(Pilha *p, char *out)
{
    if (isEmpty(p))
        return false;
    NoPilha *temp = p->topo;
    if (out != NULL)
        *out = temp->carac;
    p->topo = temp->prox;
    free(temp);
    return true;
}

bool popDouble(Pilha *p, double *out)
{
    if (isEmpty(p))
        return false;
    NoPilha *temp = p->topo;
    if (out != NULL)
        *out = temp->numero;
    p->topo = temp->prox;
    free(temp);
    return true;
}

bool topChar(Pilha *p, char *out)
{
    if (isEmpty(p) || out == NULL)
        return false;
    *out = p->topo->carac;
    return true;
}

bool topDouble(Pilha *p, double *out)
{
    if (isEmpty(p) || out == NULL)
        return false;
    *out = p->topo->numero;
    return true;
}

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
