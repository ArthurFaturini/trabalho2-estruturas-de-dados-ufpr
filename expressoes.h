#ifndef EXPRESSOES_H
#define EXPRESSOES_H

#include <stdbool.h>

/* Funções utilitárias */
int precedencia(char op);
bool associativoDireita(char op);
bool eOperador(char c);
void inverterETrocarParenteses(const char *src, char *dest);
void inverterString(char *str);
void sanitizarExpressao(const char *src, char *dest);

/* Funções de conversão */
void infixaParaPosfixaInternal(const char *infixa, char *posfixa, bool ehParaPrefixa);
void infixaParaPosfixa(const char *infixa, char *posfixa);
void infixaParaPrefixa(const char *infixa, char *prefixa);

/* Funções de validação e avaliação */
bool validarExpressao(const char *infixa);
bool avaliarPosfixa(const char *posfixa, double *resultado);

#endif // EXPRESSOES_H