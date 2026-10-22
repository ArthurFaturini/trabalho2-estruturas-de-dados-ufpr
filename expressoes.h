#ifndef EXPRESSOES_H
#define EXPRESSOES_H

#include <stdbool.h>

/* Funções utilitárias de auxílio */
int precedencia(char op);
bool associativoDireita(char op);
bool eOperador(char c);
void inverterETrocarParenteses(const char *src, char *dest);
void inverterString(char *str);

/* Função interna de conversão Shunting-yard */
void infixaParaPosfixaInternal(const char *infixa, char *posfixa, bool ehParaPrefixa);

/* Funções principais de conversão */
void infixaParaPosfixa(const char *infixa, char *posfixa);
void infixaParaPrefixa(const char *infixa, char *prefixa);

/* Funções de validação e avaliação matemática */
bool validarExpressao(const char *infixa);
bool avaliarPosfixa(const char *posfixa, double *resultado);

#endif // EXPRESSOES_H