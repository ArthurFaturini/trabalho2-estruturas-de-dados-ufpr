#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "pilha.h"
#include "expressoes.h"

/* Retorna a prioridade do operador */
int precedencia(char op)
{
    switch (op)
    {
    case '^':
        return 3;
    case '*':
    case '/':
        return 2;
    case '+':
    case '-':
        return 1;
    default:
        return 0;
    }
}

/* Retorna true se o operador for associativo à direita (Exponenciação) */
bool associativoDireita(char op)
{
    return (op == '^');
}

/* Verifica se o caractere é um operador válido */
bool eOperador(char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

/* Inverte uma string e troca os parênteses '(' por ')' e vice-versa */
void inverterETrocarParenteses(const char *src, char *dest)
{
    int len = strlen(src);
    int k = 0;

    for (int i = len - 1; i >= 0; i--)
    {
        if (src[i] == '(')
        {
            dest[k++] = ')';
        }
        else if (src[i] == ')')
        {
            dest[k++] = '(';
        }
        else
        {
            dest[k++] = src[i];
        }
    }
    dest[k] = '\0';
}

/* Inverte uma string simples em memória */
void inverterString(char *str)
{
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++)
    {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

/*
 * Função de conversão Shunting-yard.
 * O parâmetro 'ehParaPrefixa' ajusta a desempilhação para compensar
 * o espelhamento da string ao gerar a Notação Pré-fixa.
 */
void infixaParaPosfixaInternal(const char *infixa, char *posfixa, bool ehParaPrefixa)
{
    Pilha p;
    inicializarPilha(&p);

    int i = 0, k = 0;
    int len = strlen(infixa);

    while (i < len)
    {
        char c = infixa[i];

        // Ignora espaços em branco
        if (isspace(c))
        {
            i++;
            continue;
        }

        // Se for operando (dígito, letra ou ponto decimal de número real)
        if (isalnum(c) || c == '.')
        {
            while (i < len && (isalnum(infixa[i]) || infixa[i] == '.'))
            {
                posfixa[k++] = infixa[i++];
            }
            posfixa[k++] = ' '; // Espaço delimitador
            continue;
        }

        // Parêntese de abertura
        if (c == '(')
        {
            pushChar(&p, c);
        }
        // Parêntese de fecho
        else if (c == ')')
        {
            char topoOp;
            while (!isEmpty(&p) && topChar(&p, &topoOp) && topoOp != '(')
            {
                popChar(&p, &topoOp);
                posfixa[k++] = topoOp;
                posfixa[k++] = ' ';
            }
            popChar(&p, &topoOp); // Remove o '(' da pilha
        }
        // Se for um operador aritmético
        else if (eOperador(c))
        {
            char topoOp;
            while (!isEmpty(&p) && topChar(&p, &topoOp) && eOperador(topoOp))
            {
                int precC = precedencia(c);
                int precTopo = precedencia(topoOp);

                bool desempilhar = false;

                if (ehParaPrefixa)
                {
                    // Na Pré-fixa (string invertida), a associatividade do '^' inverte
                    if (associativoDireita(c))
                    {
                        desempilhar = (precC <= precTopo);
                    }
                    else
                    {
                        desempilhar = (precC < precTopo);
                    }
                }
                else
                {
                    // Pós-fixa padrão
                    if (associativoDireita(c))
                    {
                        desempilhar = (precC < precTopo);
                    }
                    else
                    {
                        desempilhar = (precC <= precTopo);
                    }
                }

                if (desempilhar)
                {
                    popChar(&p, &topoOp);
                    posfixa[k++] = topoOp;
                    posfixa[k++] = ' ';
                }
                else
                {
                    break;
                }
            }
            pushChar(&p, c);
        }
        i++;
    }

    // Desempilha operadores restantes
    char topoOp;
    while (!isEmpty(&p))
    {
        popChar(&p, &topoOp);
        if (topoOp != '(' && topoOp != ')')
        {
            posfixa[k++] = topoOp;
            posfixa[k++] = ' ';
        }
    }

    // Remove espaço em branco residual no final
    if (k > 0 && posfixa[k - 1] == ' ')
    {
        k--;
    }
    posfixa[k] = '\0';

    liberarPilha(&p);
}

/*
 * Interface pública: Converte Infixa para Pós-fixa
 */
void infixaParaPosfixa(const char *infixa, char *posfixa)
{
    infixaParaPosfixaInternal(infixa, posfixa, false);
}

/*
 * Interface pública: Converte Infixa para Pré-fixa
 */
void infixaParaPrefixa(const char *infixa, char *prefixa)
{
    int len = strlen(infixa);
    char *invertida = (char *)malloc((len + 1) * sizeof(char));
    char *posfixaInvertida = (char *)malloc((len * 2 + 1) * sizeof(char));

    if (!invertida || !posfixaInvertida)
    {
        printf("Erro na alocacao de memoria para conversao Pre-fixa.\n");
        free(invertida);
        free(posfixaInvertida);
        return;
    }

    // 1. Inverte a expressão e substitui os parênteses
    inverterETrocarParenteses(infixa, invertida);

    // 2. Aplica Shunting-yard ajustado para a Pré-fixa
    infixaParaPosfixaInternal(invertida, posfixaInvertida, true);

    // 3. Inverte o resultado obtido para encontrar a Pré-fixa final
    inverterString(posfixaInvertida);
    strcpy(prefixa, posfixaInvertida);

    free(invertida);
    free(posfixaInvertida);
}

/*
 * Valida o balanceamento de parênteses e estrutura da expressão
 */
bool validarExpressao(const char *infixa)
{
    Pilha p;
    inicializarPilha(&p);

    int len = strlen(infixa);
    bool ultimoFoiOperador = true;
    int qtdOperandos = 0;

    for (int i = 0; i < len; i++)
    {
        char c = infixa[i];

        if (isspace(c))
            continue;

        if (c == '(')
        {
            pushChar(&p, c);
            ultimoFoiOperador = true;
        }
        else if (c == ')')
        {
            char temp;
            if (!popChar(&p, &temp))
            {
                liberarPilha(&p);
                return false;
            }
            ultimoFoiOperador = false;
        }
        else if (isalnum(c) || c == '.')
        {
            while (i < len && (isalnum(infixa[i]) || infixa[i] == '.'))
            {
                i++;
            }
            i--;
            qtdOperandos++;
            ultimoFoiOperador = false;
        }
        else if (eOperador(c))
        {
            if (c == '-' && ultimoFoiOperador)
            {
                // Tratamento de sinal negativo
            }
            else if (ultimoFoiOperador)
            {
                liberarPilha(&p);
                return false;
            }
            ultimoFoiOperador = true;
        }
        else
        {
            liberarPilha(&p);
            return false;
        }
    }

    bool parentesesOk = isEmpty(&p);
    liberarPilha(&p);

    if (!parentesesOk || ultimoFoiOperador || qtdOperandos == 0)
    {
        return false;
    }

    return true;
}

/*
 * Avalia numericamente a expressão pós-fixa
 */
bool avaliarPosfixa(const char *posfixa, double *resultado)
{
    Pilha p;
    inicializarPilha(&p);

    int i = 0;
    int len = strlen(posfixa);

    while (i < len)
    {
        char c = posfixa[i];

        if (isspace(c))
        {
            i++;
            continue;
        }

        if (isdigit(c) || c == '.' || (c == '-' && isdigit(posfixa[i + 1])))
        {
            char *endptr;
            double val = strtod(&posfixa[i], &endptr);
            pushDouble(&p, val);
            i += (endptr - &posfixa[i]);
            continue;
        }

        if (eOperador(c))
        {
            double op2, op1;

            if (!popDouble(&p, &op2) || !popDouble(&p, &op1))
            {
                printf("\nErro: Expressao malformada na avaliacao.\n");
                liberarPilha(&p);
                return false;
            }

            double res = 0.0;

            switch (c)
            {
            case '+':
                res = op1 + op2;
                break;
            case '-':
                res = op1 - op2;
                break;
            case '*':
                res = op1 * op2;
                break;
            case '/':
                if (op2 == 0.0)
                {
                    printf("\nErro de Execucao: Divisao por zero detectada!\n");
                    liberarPilha(&p);
                    return false;
                }
                res = op1 / op2;
                break;
            case '^':
                res = pow(op1, op2);
                break;
            default:
                liberarPilha(&p);
                return false;
            }

            pushDouble(&p, res);
        }
        i++;
    }

    bool ok = popDouble(&p, resultado);
    if (!isEmpty(&p))
    {
        ok = false;
    }

    liberarPilha(&p);
    return ok;
}