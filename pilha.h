#ifndef PILHA_H
#define PILHA_H

#include <stdbool.h>

/* Enum para diferenciar se o nó guarda um operador/caractere ou um número */
typedef enum
{
    TIPO_CHAR,
    TIPO_DOUBLE
} TipoDado;

/* Conteúdo flexível do nó */
typedef union
{
    char carac;
    double numero;
} Dado;

/* Struct do nó encadeado */
typedef struct NoPilha
{
    Dado dado;
    TipoDado tipo;
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