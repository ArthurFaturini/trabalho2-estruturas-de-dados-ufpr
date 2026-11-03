/*
 * ===========================================================
 *                           NOMES
 *            ARTHUR FATURINI RANGEL DE PAULA ARAUJO
 *            MARCIA KAMILA DE NAZARE FEITOSA RAMOS
 * ===========================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

/* Inclusoes de plataforma (originalmente em impressora.c): Sleep / usleep */
#ifdef _WIN32
#include <windows.h> /* Sleep */
#else
#include <unistd.h> /* usleep */
#endif

/*
 * ===========================================================
 *                           PILHA.H
 * ===========================================================
 */

/* Struct do nó encadeado */
typedef struct NoPilha
{
    char carac;    /* Guarda operador/caractere se tipo == 0 */
    double numero; /* Guarda valor numérico se tipo == 1 */
    int tipo;      /* 0 para char, 1 para double */
    struct NoPilha *prox;
} NoPilha;

/* Struct de controle da Pilha */
typedef struct
{
    NoPilha *topo;
} Pilha;

/*
 * ===========================================================
 *                         EXPRESSOES.H
 * ===========================================================
 */

/* Nó para a Lista Encadeada de Expressões em Lote */
typedef struct NoExpressao
{
    char expressao[256];
    struct NoExpressao *prox;
} NoExpressao;

/*
 * ===========================================================
 *                            FILA.H
 * ===========================================================
 */

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

/*
 * ===========================================================
 *                         IMPRESSORA.H
 * ===========================================================
 */

/* Quantas solicitacoes saem da fila de entrada a cada 0.5 segundo */
#define TAMANHO_LOTE 5

/* --------------------- PILHA.H --------------------- */
void inicializarPilha(Pilha *p);
bool isEmpty(Pilha *p);
bool pushChar(Pilha *p, char c);
bool pushDouble(Pilha *p, double val);
bool popChar(Pilha *p, char *out);
bool popDouble(Pilha *p, double *out);
bool topChar(Pilha *p, char *out);
bool topDouble(Pilha *p, double *out);
void liberarPilha(Pilha *p);

/* ------------------- EXPRESSOES.H ------------------- */

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

/* Funções para gerenciar a lista de expressões */
void inserirExpressao(NoExpressao **lista, const char *expr);
void liberarListaExpressoes(NoExpressao *lista);

/* Função própria de potenciação (substituta da pow da <math.h>) */
double potencia(double base, int expoente);

/* --------------------- FILA.H ---------------------- */
void inicializarFila(Fila *f);
int filaVazia(Fila *f);
int enfileirar(Fila *f, int prioridade, char ip[]);
int desenfileirar(Fila *f, int *prioridade, char ip[]);
void imprimirFila(Fila *f, char nome[]);
void liberarFila(Fila *f);

/* ------------------- IMPRESSORA.H ------------------- */
int carregarSolicitacoes(char nomeArquivo[], Fila *entrada);
void distribuirLote(Fila *entrada, Fila *fila0, Fila *fila1, Fila *fila2);
void exibirFilas(Fila *entrada, Fila *fila0, Fila *fila1, Fila *fila2);
void simularDistribuicao(Fila *entrada, Fila *fila0, Fila *fila1, Fila *fila2);
void esperarMeioSegundo(void);

/* --------------------- MAIN.C ---------------------- */
void menuPilha(void);
void menuFila(void);
void limparTela(void);
void pausar(void);

/*
 * ===========================================================
 *                           PILHA.C
 * ===========================================================
 */

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
    NoPilha *novo;
    if (p == NULL)
        return false;
    novo = (NoPilha *)malloc(sizeof(NoPilha));
    if (novo == NULL)
        return false;

    novo->carac = c;
    novo->tipo = 0; /* 0 indica char */
    novo->prox = p->topo;
    p->topo = novo;
    return true;
}

bool pushDouble(Pilha *p, double val)
{
    NoPilha *novo;
    if (p == NULL)
        return false;
    novo = (NoPilha *)malloc(sizeof(NoPilha));
    if (novo == NULL)
        return false;

    novo->numero = val;
    novo->tipo = 1; /* 1 indica double */
    novo->prox = p->topo;
    p->topo = novo;
    return true;
}

bool popChar(Pilha *p, char *out)
{
    NoPilha *temp;
    if (isEmpty(p))
        return false;
    temp = p->topo;
    if (out != NULL)
        *out = temp->carac;
    p->topo = temp->prox;
    free(temp);
    return true;
}

bool popDouble(Pilha *p, double *out)
{
    NoPilha *temp;
    if (isEmpty(p))
        return false;
    temp = p->topo;
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
    NoPilha *atual;
    if (p == NULL)
        return;
    atual = p->topo;
    while (atual != NULL)
    {
        NoPilha *temp = atual;
        atual = atual->prox;
        free(temp);
    }
    p->topo = NULL;
}

/*
 * ===========================================================
 *                         EXPRESSOES.C
 * ===========================================================
 */

/* Função própria para substituir o pow() da <math.h> */
double potencia(double base, int expoente)
{
    double resultado = 1.0;
    int expAbs = (expoente < 0) ? -expoente : expoente;

    int i;
    for (i = 0; i < expAbs; i++)
    {
        resultado *= base;
    }

    if (expoente < 0)
    {
        return 1.0 / resultado;
    }

    return resultado;
}

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

    int i;
    for (i = len - 1; i >= 0; i--)
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
    int i;
    for (i = 0; i < len / 2; i++)
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
    int i = 0, k = 0;
    int len;
    bool proximoPodeSerUnario = true; /* Início da string pode ser unário */
    char topoOp;

    inicializarPilha(&p);

    len = strlen(infixa);

    while (i < len)
    {
        char c = infixa[i];

        if (isspace(c))
        {
            i++;
            continue;
        }

        /* TRATAMENTO DE NÚMEROS NEGATIVOS UNÁRIOS: ex (-25 ou -11 no início */
        if ((c == '-' || c == '+') && proximoPodeSerUnario)
        {
            /* Caso 1: Na Pós-fixa normal, o sinal vem ANTES do número (ex: "-5") */
            if (!ehParaPrefixa && (i + 1 < len) && (isdigit(infixa[i + 1]) || infixa[i + 1] == '.'))
            {
                if (c == '-')
                {
                    posfixa[k++] = '-';
                }
                i++;
                while (i < len && (isalnum(infixa[i]) || infixa[i] == '.'))
                {
                    posfixa[k++] = infixa[i++];
                }
                posfixa[k++] = ' ';
                proximoPodeSerUnario = false;
                continue;
            }
        }

        /* Se for operando (dígito, letra ou ponto decimal) */
        if (isalnum(c) || c == '.')
        {
            if (ehParaPrefixa)
            {
                int inicioNum = i;
                int fimNum = i;
                int j;

                while (fimNum < len && (isalnum(infixa[fimNum]) || infixa[fimNum] == '.'))
                {
                    fimNum++;
                }

                /*
                 * Na Pré-fixa (string invertida), só é UNÁRIO se após o número houver um '-'
                 * E logo depois desse '-' houver um operador ou fim de parêntese (ex: "5--5" invertido fica "5- - 5").
                 */
                if (fimNum < len && infixa[fimNum] == '-' && proximoPodeSerUnario)
                {
                    /* Checa se após o '-' existe um operador binário, parêntese ou espaço */
                    int proxIdx = fimNum + 1;
                    while (proxIdx < len && isspace(infixa[proxIdx]))
                        proxIdx++;

                    if (proxIdx >= len || eOperador(infixa[proxIdx]) || infixa[proxIdx] == ')' || infixa[proxIdx] == '(')
                    {
                        for (j = inicioNum; j < fimNum; j++)
                        {
                            posfixa[k++] = infixa[j];
                        }
                        posfixa[k++] = '-'; /* Anexa o '-' após os dígitos para desinverter como '-5' */
                        posfixa[k++] = ' ';
                        i = fimNum + 1;
                        proximoPodeSerUnario = false;
                        continue;
                    }
                }
            }

            while (i < len && (isalnum(infixa[i]) || infixa[i] == '.'))
            {
                posfixa[k++] = infixa[i++];
            }
            posfixa[k++] = ' ';
            proximoPodeSerUnario = false;
            continue;
        }

        /* Parêntese de abertura */
        if (c == '(')
        {
            pushChar(&p, c);
            proximoPodeSerUnario = true; /* Após '(', o próximo símbolo pode ser unário */
        }
        /* Parêntese de fecho */
        else if (c == ')')
        {
            while (!isEmpty(&p) && topChar(&p, &topoOp) && topoOp != '(')
            {
                popChar(&p, &topoOp);
                posfixa[k++] = topoOp;
                posfixa[k++] = ' ';
            }
            popChar(&p, &topoOp);
            proximoPodeSerUnario = false;
        }
        /* Se for operador binário */
        else if (eOperador(c))
        {
            while (!isEmpty(&p) && topChar(&p, &topoOp) && eOperador(topoOp))
            {
                int precC = precedencia(c);
                int precTopo = precedencia(topoOp);

                bool desempilhar = false;

                if (ehParaPrefixa)
                {
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
            proximoPodeSerUnario = true; /* Após um operador binário, pode vir unário */
        }
        i++;
    }

    while (!isEmpty(&p))
    {
        popChar(&p, &topoOp);
        if (topoOp != '(' && topoOp != ')')
        {
            posfixa[k++] = topoOp;
            posfixa[k++] = ' ';
        }
    }

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

    /* 1. Inverte a expressão e substitui os parênteses */
    inverterETrocarParenteses(infixa, invertida);

    /* 2. Aplica Shunting-yard ajustado para a Pré-fixa */
    infixaParaPosfixaInternal(invertida, posfixaInvertida, true);

    /* 3. Inverte o resultado obtido para encontrar a Pré-fixa final */
    inverterString(posfixaInvertida);
    strcpy(prefixa, posfixaInvertida);

    free(invertida);
    free(posfixaInvertida);
}

/*
 * Remove tudo após '=' e normaliza símbolos do edital (÷, ·, {}, [])
 */
void sanitizarExpressao(const char *src, char *dest)
{
    int i = 0, k = 0;
    while (src[i] != '\0')
    {
        if (src[i] == '=')
            break; /* Descarta tudo após o '=' */

        if (src[i] == '{' || src[i] == '[')
        {
            dest[k++] = '(';
        }
        else if (src[i] == '}' || src[i] == ']')
        {
            dest[k++] = ')';
        }
        else if ((unsigned char)src[i] == 0xF7 || src[i] == ':')
        {
            dest[k++] = '/';
        }
        else if (src[i] == '.' && (i == 0 || !isdigit(src[i - 1])) && !isdigit(src[i + 1]))
        {
            dest[k++] = '*'; /* Ponto isolado usado como multiplicação (ex: 3 . 2) */
        }
        else
        {
            dest[k++] = src[i];
        }
        i++;
    }
    dest[k] = '\0';
}

/*
 * Valida parênteses e estrutura da expressão
 */
bool validarExpressao(const char *infixa)
{
    Pilha p;
    int len;
    bool ultimoFoiOperador = true;
    int qtdOperandos = 0;
    bool parentesesOk;
    int i;

    inicializarPilha(&p);

    len = strlen(infixa);

    for (i = 0; i < len; i++)
    {
        char c = infixa[i];

        if (isspace(c))
            continue;

        if (c == '(')
        {
            /* Se o elemento anterior era um operando (ex: "9 (") ou um ')', a sintaxe é inválida! */
            if (!ultimoFoiOperador)
            {
                liberarPilha(&p);
                return false;
            }

            pushChar(&p, c);
            ultimoFoiOperador = true;
        }
        else if (c == ')')
        {
            char temp;

            /* Não pode ter parêntese vazio "()" ou operador logo antes de ')' */
            if (ultimoFoiOperador)
            {
                liberarPilha(&p);
                return false;
            }

            if (!popChar(&p, &temp))
            {
                liberarPilha(&p);
                return false; /* Parêntese fechado sem abertura correspondente */
            }
            ultimoFoiOperador = false;
        }
        else if (isalnum(c) || c == '.')
        {
            /* Se o elemento anterior foi um parêntese de fecho sem operador (ex: ") 9"), é inválido */
            if (!ultimoFoiOperador && i > 0)
            {
                /* Checa se o caractere anterior que não era espaço era ')' */
                int j = i - 1;
                while (j >= 0 && isspace(infixa[j]))
                    j--;
                if (j >= 0 && infixa[j] == ')')
                {
                    liberarPilha(&p);
                    return false;
                }
            }

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
                /* Operador unário permitido */
            }
            else if (ultimoFoiOperador)
            {
                liberarPilha(&p);
                return false; /* Dois operadores binários seguidos */
            }
            else
            {
                ultimoFoiOperador = true;
            }
        }
        else
        {
            liberarPilha(&p);
            return false;
        }
    }

    parentesesOk = isEmpty(&p);
    liberarPilha(&p);

    if (!parentesesOk || ultimoFoiOperador || qtdOperandos == 0)
    {
        return false;
    }

    return true;
}

/*
 * Calcula numericamente a expressão pós-fixa usando a Pilha
 */
bool avaliarPosfixa(const char *posfixa, double *resultado)
{
    Pilha p;
    int i = 0;
    int len;
    bool ok;

    inicializarPilha(&p);

    len = strlen(posfixa);

    while (i < len)
    {
        char c = posfixa[i];

        if (isspace(c))
        {
            i++;
            continue;
        }

        /* Se for número (inteiro ou real) */
        if (isdigit(c) || c == '.' || (c == '-' && isdigit(posfixa[i + 1])))
        {
            char *endptr;
            double val = strtod(&posfixa[i], &endptr);
            pushDouble(&p, val);
            i += (endptr - &posfixa[i]);
            continue;
        }

        /* Se for operador */
        if (eOperador(c))
        {
            double op2, op1;
            double res;

            if (!popDouble(&p, &op2) || !popDouble(&p, &op1))
            {
                printf("\nErro: Expressao malformada na avaliacao.\n");
                liberarPilha(&p);
                return false;
            }

            res = 0.0;

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
                res = potencia(op1, (int)op2);
                break;
            default:
                liberarPilha(&p);
                return false;
            }

            pushDouble(&p, res);
        }
        i++;
    }

    ok = popDouble(&p, resultado);
    if (!isEmpty(&p))
    {
        ok = false;
    }

    liberarPilha(&p);
    return ok;
}

void inserirExpressao(NoExpressao **lista, const char *expr)
{
    NoExpressao *novo = (NoExpressao *)malloc(sizeof(NoExpressao));
    if (novo == NULL)
        return;

    strcpy(novo->expressao, expr);
    novo->prox = NULL;

    if (*lista == NULL)
    {
        *lista = novo;
    }
    else
    {
        NoExpressao *atual = *lista;
        while (atual->prox != NULL)
        {
            atual = atual->prox;
        }
        atual->prox = novo;
    }
}

void liberarListaExpressoes(NoExpressao *lista)
{
    NoExpressao *atual = lista;
    while (atual != NULL)
    {
        NoExpressao *temp = atual;
        atual = atual->prox;
        free(temp);
    }
}

/*
 * ===========================================================
 *                            FILA.C
 * ===========================================================
 */

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
        f->inicio = novo; /* fila vazia: o novo e o primeiro */
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

/*
 * ===========================================================
 *                         IMPRESSORA.C
 * ===========================================================
 */

/* Le o arquivo TXT e coloca cada solicitacao na fila de entrada.
 * Formato: "1, 192.168.0.1; 2, 192.168.0.2; ... 3, 192.168.0.50."
 * Retorna quantas solicitacoes foram carregadas, ou -1 se o arquivo nao abriu. */
int carregarSolicitacoes(char nomeArquivo[], Fila *entrada)
{
    FILE *arquivo;
    int prioridade;
    char ip[TAM_IP];
    int total = 0;
    int tamanho;
    int lidos;

    arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL)
        return -1;

    /* Cada leitura pega um par "prioridade, ip" e o ';' que vem depois.
     * %15[0-9.] le somente digitos e pontos (no maximo 15 caracteres).
     * fscanf retorna quantos campos leu, ou EOF no fim do arquivo. */
    while ((lidos = fscanf(arquivo, " %d , %15[0-9.] ;", &prioridade, ip)) != EOF)
    {
        if (lidos != 2)
        {
            /* Trecho mal escrito: pula ate o proximo ';' e continua lendo */
            printf("Ignorado: trecho invalido no arquivo\n");
            fscanf(arquivo, "%*[^;]");
            fscanf(arquivo, ";");
            continue;
        }

        /* O ultimo IP do arquivo termina com '.', que nao faz parte dele */
        tamanho = strlen(ip);
        if (tamanho > 0 && ip[tamanho - 1] == '.')
            ip[tamanho - 1] = '\0';

        if (prioridade < 1 || prioridade > 3)
        {
            printf("Ignorada: %s tem prioridade invalida (%d)\n", ip, prioridade);
            continue;
        }

        if (!enfileirar(entrada, prioridade, ip))
        {
            printf("Erro: memoria insuficiente ao carregar %s\n", ip);
            break;
        }
        total++;
    }

    fclose(arquivo);
    return total;
}

/* Tira ate 5 solicitacoes da fila de entrada e coloca cada uma
 * na fila da sua prioridade: 1 -> Fila 0, 2 -> Fila 1, 3 -> Fila 2 */
void distribuirLote(Fila *entrada, Fila *fila0, Fila *fila1, Fila *fila2)
{
    int i;
    int prioridade;
    char ip[TAM_IP];

    for (i = 0; i < TAMANHO_LOTE; i++)
    {
        if (!desenfileirar(entrada, &prioridade, ip))
            break; /* a fila de entrada acabou antes de completar 5 */

        if (prioridade == 1)
            enfileirar(fila0, prioridade, ip);
        else if (prioridade == 2)
            enfileirar(fila1, prioridade, ip);
        else
            enfileirar(fila2, prioridade, ip);

        printf("  %-15s (prioridade %d) -> Fila %d\n", ip, prioridade, prioridade - 1);
    }
}

/* Mostra a situacao das quatro filas */
void exibirFilas(Fila *entrada, Fila *fila0, Fila *fila1, Fila *fila2)
{
    printf("\n");
    imprimirFila(entrada, "Fila de Entrada");
    imprimirFila(fila0, "Fila 0 - maxima");
    imprimirFila(fila1, "Fila 1 - normal");
    imprimirFila(fila2, "Fila 2 - baixa ");
}

/* Distribui lotes de 5 a cada 0.5 segundo ate esvaziar a fila de entrada,
 * mostrando a situacao das filas depois de cada lote */
void simularDistribuicao(Fila *entrada, Fila *fila0, Fila *fila1, Fila *fila2)
{
    int lote = 1;

    while (!filaVazia(entrada))
    {
        printf("\n========== LOTE %d ==========\n", lote);
        distribuirLote(entrada, fila0, fila1, fila2);
        exibirFilas(entrada, fila0, fila1, fila2);
        esperarMeioSegundo();
        lote++;
    }

    printf("\nDistribuicao concluida! A fila de entrada esta vazia.\n");
}

/* Pausa o programa por 0.5 segundo.
 * Windows usa Sleep (milissegundos); macOS/Linux usa usleep (microssegundos). */
void esperarMeioSegundo(void)
{
    fflush(stdout); /* mostra na tela o que ja foi impresso antes de pausar */
#ifdef _WIN32
    Sleep(500);
#else
    usleep(500000);
#endif
}

/*
 * ===========================================================
 *                            MAIN.C
 * ===========================================================
 */

int main(void)
{
    int opcao = 0;

    do
    {
        limparTela();
        printf("===========================================\n");
        printf("   TRABALHO PRATICO II - ESTRUTURAS DE DADOS\n");
        printf("===========================================\n");
        printf("1. Aplicacao A - Pilha (Expressoes Aritmeticas)\n");
        printf("2. Aplicacao B - Fila (Gerenciamento de Impressao)\n");
        printf("0. Sair\n");
        printf("===========================================\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) == 1)
        {
            while (getchar() != '\n')
                ; /* Limpa o \n para nao vazar para a menuPilha() */
        }

        switch (opcao)
        {
        case 1:
            menuPilha();
            break;
        case 2:
            menuFila();
            break;
        case 0:
            printf("\nEncerrando o programa... Ate logo!\n");
            break;
        default:
            printf("\nOpcao invalida! Tente novamente.\n");
            pausar();
            break;
        }
    } while (opcao != 0);

    return 0;
}

/* =======================================================
 * APLICACAO A: PILHA (Expressões Aritméticas)
 * Requisito: Menu obrigatório com as 5 opções do edital.
 * ======================================================= */
void menuPilha(void)
{
    NoExpressao *listaExpressoes = NULL;
    char bufferEntrada[256];
    char infixaLimpa[256];
    char resultadoPosfixa[512];
    char resultadoPrefixa[512];
    int opcao = 0;
    int contador = 1;

    limparTela();
    printf("=======================================================\n");
    printf("     ENTRADA DE EXPRESSOES (MODULO PILHA)             \n");
    printf("=======================================================\n");
    printf("Digite as expressoes que deseja processar.\n");
    printf("Para encerrar e ir para o menu, digite 'FIM' ou aperte Enter.\n\n");

    while (1)
    {
        printf("Expressao #%d: ", contador);
        if (fgets(bufferEntrada, sizeof(bufferEntrada), stdin) == NULL)
            break;

        bufferEntrada[strcspn(bufferEntrada, "\n")] = '\0';

        /* Flag de parada */
        if (strlen(bufferEntrada) == 0 ||
            strcmp(bufferEntrada, "FIM") == 0 ||
            strcmp(bufferEntrada, "fim") == 0 ||
            strcmp(bufferEntrada, "Fim") == 0)
        {
            break;
        }

        inserirExpressao(&listaExpressoes, bufferEntrada);
        contador++;
    }

    /* Se o usuário não digitou nenhuma expressão, avisa e retorna */
    if (listaExpressoes == NULL)
    {
        printf("\nNenhuma expressao inserida. Retornando...\n");
        pausar();
        return;
    }

    /* LOOP DO MENU PRINCIPAL DE OPERACOES */
    do
    {
        limparTela();
        printf("-------------------------------------------\n");
        printf("      APLICACAO A: EXPRESSOES (PILHA)\n");
        printf("-------------------------------------------\n");
        printf("1. Transformar Infixa em Posfixa (Todas)\n");
        printf("2. Transformar Infixa em Prefixa (Todas)\n");
        printf("3. Verificar validade de expressoes (Todas)\n");
        printf("4. Calcular resultado de expressoes (Todas)\n");
        printf("5. Voltar ao Menu Principal\n");
        printf("-------------------------------------------\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1)
        {
            while (getchar() != '\n')
                ;
            opcao = -1;
        }

        while (getchar() != '\n')
            ; /* Limpa buffer */

        switch (opcao)
        {
        case 1:
            printf("\n--- CONVERSAO PARA POSFIXA ---\n");
            {
                NoExpressao *curr = listaExpressoes;
                int idx = 1;
                while (curr != NULL)
                {
                    sanitizarExpressao(curr->expressao, infixaLimpa);
                    printf("\n[%d] Entrada : %s\n", idx++, curr->expressao);

                    if (!validarExpressao(infixaLimpa))
                    {
                        printf("    Status  : INVALIDA!\n");
                    }
                    else
                    {
                        infixaParaPosfixa(infixaLimpa, resultadoPosfixa);
                        printf("    Posfixa : %s\n", resultadoPosfixa);
                    }
                    curr = curr->prox;
                }
            }
            pausar();
            break;

        case 2:
            printf("\n--- CONVERSAO PARA PREFIXA ---\n");
            {
                NoExpressao *curr = listaExpressoes;
                int idx = 1;
                while (curr != NULL)
                {
                    sanitizarExpressao(curr->expressao, infixaLimpa);
                    printf("\n[%d] Entrada : %s\n", idx++, curr->expressao);

                    if (!validarExpressao(infixaLimpa))
                    {
                        printf("    Status  : INVALIDA!\n");
                    }
                    else
                    {
                        infixaParaPrefixa(infixaLimpa, resultadoPrefixa);
                        printf("    Prefixa : %s\n", resultadoPrefixa);
                    }
                    curr = curr->prox;
                }
            }
            pausar();
            break;

        case 3:
            printf("\n--- VERIFICACAO DE VALIDADE ---\n");
            {
                NoExpressao *curr = listaExpressoes;
                int idx = 1;
                while (curr != NULL)
                {
                    bool valida;
                    sanitizarExpressao(curr->expressao, infixaLimpa);
                    valida = validarExpressao(infixaLimpa);
                    printf("[%d] Expressao: %-30s -> %s\n",
                           idx++, curr->expressao, valida ? "VALIDA" : "INVALIDA");
                    curr = curr->prox;
                }
            }
            pausar();
            break;

        case 4:
            printf("\n--- AVALIACAO E CALCULO MATEMATICO ---\n");
            {
                NoExpressao *curr = listaExpressoes;
                int idx = 1;
                while (curr != NULL)
                {
                    sanitizarExpressao(curr->expressao, infixaLimpa);
                    printf("\n[%d] Expressao: %s\n", idx++, curr->expressao);

                    if (!validarExpressao(infixaLimpa))
                    {
                        printf("    Erro: Expressao sintaticamente invalida.\n");
                    }
                    else
                    {
                        double valFinal = 0.0;
                        infixaParaPosfixa(infixaLimpa, resultadoPosfixa);
                        if (avaliarPosfixa(resultadoPosfixa, &valFinal))
                        {
                            printf("    Posfixa  : %s\n", resultadoPosfixa);
                            printf("    Resultado: %.2f\n", valFinal);
                        }
                    }
                    curr = curr->prox;
                }
            }
            pausar();
            break;

        case 5:
            printf("\nLiberando lista de expressoes e voltando...\n");
            liberarListaExpressoes(listaExpressoes);
            break;

        default:
            printf("\nOpcao invalida!\n");
            pausar();
            break;
        }
    } while (opcao != 5);
}

/* =======================================================
 * APLICACAO B: FILA (Gerenciamento de Impressões)
 * Requisito: Carregar TXT, distribuir com delay e exibir filas.
 * ======================================================= */
void menuFila(void)
{
    int opcao = 0;
    int carregadas;
    Fila entrada, fila0, fila1, fila2;

    inicializarFila(&entrada);
    inicializarFila(&fila0);
    inicializarFila(&fila1);
    inicializarFila(&fila2);

    do
    {
        limparTela();
        printf("-------------------------------------------\n");
        printf("    APLICACAO B: IMPRESSORA (FILA)\n");
        printf("-------------------------------------------\n");
        printf("1. Carregar solicitacoes do arquivo TXT\n");
        printf("2. Iniciar distribuicao e simulacao (delay 0.5s)\n");
        printf("3. Exibir situacao das filas\n");
        printf("4. Voltar ao Menu Principal\n");
        printf("-------------------------------------------\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1)
        {
            while (getchar() != '\n')
                ;
            opcao = -1;
        }
        while (getchar() != '\n')
            ;

        switch (opcao)
        {
        case 1:
            /* Esvazia as filas antes, para nao duplicar se carregar de novo */
            liberarFila(&entrada);
            liberarFila(&fila0);
            liberarFila(&fila1);
            liberarFila(&fila2);

            carregadas = carregarSolicitacoes("solicitacoes.txt", &entrada);
            if (carregadas < 0)
                printf("\nErro: nao foi possivel abrir 'solicitacoes.txt'.\n");
            else
                printf("\n%d solicitacoes carregadas na Fila de Entrada.\n", carregadas);
            pausar();
            break;
        case 2:
            if (filaVazia(&entrada))
                printf("\nA Fila de Entrada esta vazia. Use a opcao 1 primeiro.\n");
            else
                simularDistribuicao(&entrada, &fila0, &fila1, &fila2);
            pausar();
            break;
        case 3:
            exibirFilas(&entrada, &fila0, &fila1, &fila2);
            pausar();
            break;
        case 4:
            liberarFila(&entrada);
            liberarFila(&fila0);
            liberarFila(&fila1);
            liberarFila(&fila2);
            printf("\nVoltando ao menu principal...\n");
            break;
        default:
            printf("\nOpcao invalida!\n");
            pausar();
            break;
        }
    } while (opcao != 4);
}

/* Função utilitária para limpar a tela em Windows ou Linux/macOS */
void limparTela(void)
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/* Função utilitária para pausar a execução até o usuário pressionar Enter */
void pausar(void)
{
    printf("\nPressione Enter para continuar...");
    while (getchar() != '\n')
        ; /* Limpa buffer pendente */
    /* getchar(); // Aguarda a tecla Enter */
}