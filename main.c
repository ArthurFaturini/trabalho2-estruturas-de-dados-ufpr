#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pilha.h"
#include "expressoes.h"

void menuPilha(void);
void menuFila(void);
void limparTela(void);
void pausar(void);

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
                ; // Limpa o \n para nao vazar para a menuPilha()
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

    limparTela();
    printf("=======================================================\n");
    printf("     ENTRADA DE EXPRESSOES (MODULO PILHA)             \n");
    printf("=======================================================\n");
    printf("Digite as expressoes que deseja processar.\n");
    printf("Para encerrar e ir para o menu, digite 'FIM' ou aperte Enter.\n\n");

    int contador = 1;
    while (1)
    {
        printf("Expressao #%d: ", contador);
        if (fgets(bufferEntrada, sizeof(bufferEntrada), stdin) == NULL)
            break;

        bufferEntrada[strcspn(bufferEntrada, "\n")] = '\0';

        /* Flag de parada */
        if (strlen(bufferEntrada) == 0 || strcasecmp(bufferEntrada, "FIM") == 0)
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
            ; // Limpa buffer

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
                    sanitizarExpressao(curr->expressao, infixaLimpa);
                    bool valida = validarExpressao(infixaLimpa);
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
                        infixaParaPosfixa(infixaLimpa, resultadoPosfixa);
                        double valFinal = 0.0;
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

        switch (opcao)
        {
        case 1:
            printf("\n[Em desenvolvimento] Carregando 'solicitacoes.txt'...\n");
            // TODO: Chamar funcao de leitura de TXT
            pausar();
            break;
        case 2:
            printf("\n[Em desenvolvimento] Processando lotes de 5 solicitacoes...\n");
            // TODO: Chamar funcao de simulacao de prioridades
            pausar();
            break;
        case 3:
            printf("\n[Em desenvolvimento] Exibindo estado das filas...\n");
            // TODO: Chamar funcao de impressao de tela
            pausar();
            break;
        case 4:
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
        ; // Limpa buffer pendente
    // getchar(); // Aguarda a tecla Enter
}