#ifndef PILHA_H
#define PILHA_H

#include <stdbool.h>

/* Struct do nó encadeado */
typedef struct NoPilha
{
    char carac;    // Guarda operador/caractere se tipo == 0
    double numero; // Guarda valor numérico se tipo == 1
    int tipo;      // 0 para char, 1 para double
    struct NoPilha *prox;
} NoPilha;

/* Struct de controle da Pilha */
typedef struct
{
    NoPilha *topo;
} Pilha;

/* Protótipos das Funções */
void inicializarPilha(Pilha *p);
bool isEmpty(Pilha *p);
bool pushChar(Pilha *p, char c);
bool pushDouble(Pilha *p, double val);
bool popChar(Pilha *p, char *out);
bool popDouble(Pilha *p, double *out);
bool topChar(Pilha *p, char *out);
bool topDouble(Pilha *p, double *out);
void liberarPilha(Pilha *p);

#endif // PILHA_H