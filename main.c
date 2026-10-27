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

        if (scanf("%d", &opcao) != 1)
        {
            while (getchar() != '\n')
                ; // Limpa o buffer em caso de entrada invalida
            opcao = -1;
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
    int opcao = 0;
    char infixaBruta[256];
    char infixaLimpa[256];
    char resultadoPosfixa[512];
    char resultadoPrefixa[512];

    /* Matriz/Estrutura para armazenar até 50 expressões em lote */
    char loteInfixa[50][256];
    int totalExpressoes = 0;

    do
    {
        limparTela();
        printf("-------------------------------------------\n");
        printf("      APLICACAO A: EXPRESSOES (PILHA)\n");
        printf("-------------------------------------------\n");
        printf("1. Transformar Infixa em Posfixa\n");
        printf("2. Transformar Infixa em Prefixa\n");
        printf("3. Verificar validade de uma expressao\n");
        printf("4. Calcular resultado de expressoes\n");
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
            ; // Limpa buffer do teclado

        switch (opcao)
        {
        case 1:
            printf("\n--- CONVERTER PARA POSFIXA ---\n");
            printf("Digite uma expressao: ");
            if (fgets(infixaBruta, sizeof(infixaBruta), stdin) != NULL)
            {
                infixaBruta[strcspn(infixaBruta, "\n")] = '\0';
                sanitizarExpressao(infixaBruta, infixaLimpa);

                if (!validarExpressao(infixaLimpa))
                {
                    printf("\nErro: A expressao fornecida eh INVALIDA!\n");
                }
                else
                {
                    infixaParaPosfixa(infixaLimpa, resultadoPosfixa);
                    printf("\nExpressao Entrada : %s\n", infixaBruta);
                    printf("Expressao Processada: %s\n", infixaLimpa);
                    printf("Expressao Posfixa  : %s\n", resultadoPosfixa);
                }
            }
            pausar();
            break;

        case 2:
            printf("\n--- CONVERTER PARA PREFIXA ---\n");
            printf("Digite uma expressao: ");
            if (fgets(infixaBruta, sizeof(infixaBruta), stdin) != NULL)
            {
                infixaBruta[strcspn(infixaBruta, "\n")] = '\0';
                sanitizarExpressao(infixaBruta, infixaLimpa);

                if (!validarExpressao(infixaLimpa))
                {
                    printf("\nErro: A expressao fornecida eh INVALIDA!\n");
                }
                else
                {
                    infixaParaPrefixa(infixaLimpa, resultadoPrefixa);
                    printf("\nExpressao Entrada : %s\n", infixaBruta);
                    printf("Expressao Processada: %s\n", infixaLimpa);
                    printf("Expressao Prefixa  : %s\n", resultadoPrefixa);
                }
            }
            pausar();
            break;

        case 3:
            printf("\n--- VERIFICAR VALIDADE ---\n");
            printf("Digite a expressao para validar: ");
            if (fgets(infixaBruta, sizeof(infixaBruta), stdin) != NULL)
            {
                infixaBruta[strcspn(infixaBruta, "\n")] = '\0';
                sanitizarExpressao(infixaBruta, infixaLimpa);

                if (validarExpressao(infixaLimpa))
                {
                    printf("\nResultado: A expressao '%s' eh VALIDA!\n", infixaBruta);
                }
                else
                {
                    printf("\nResultado: A expressao '%s' eh INVALIDA!\n", infixaBruta);
                }
            }
            pausar();
            break;

        case 4:
            printf("\n--- CALCULAR RESULTADO DE EXPRESSOES (ENTRADA EM LOTE) ---\n");
            printf("Digite quantas expressoes desejar. Para encerrar as entradas, digite 'FIM' ou pressione Enter em linha vazia.\n\n");

            totalExpressoes = 0;
            while (totalExpressoes < 50)
            {
                printf("Expressao #%d: ", totalExpressoes + 1);
                if (fgets(infixaBruta, sizeof(infixaBruta), stdin) == NULL)
                    break;

                infixaBruta[strcspn(infixaBruta, "\n")] = '\0';

                /* Flag de parada: digitar 'FIM', 'fim' ou dar Enter vazio */
                if (strlen(infixaBruta) == 0 || strcasecmp(infixaBruta, "FIM") == 0)
                {
                    break;
                }

                strcpy(loteInfixa[totalExpressoes], infixaBruta);
                totalExpressoes++;
            }

            if (totalExpressoes == 0)
            {
                printf("\nNenhuma expressao foi digitada.\n");
            }
            else
            {
                printf("\n=======================================================\n");
                printf("          RESULTADOS DAS EXPRESSOES PROCESSADAS        \n");
                printf("=======================================================\n");

                for (int i = 0; i < totalExpressoes; i++)
                {
                    sanitizarExpressao(loteInfixa[i], infixaLimpa);

                    printf("\n[%d] Expressao Entrada: %s\n", i + 1, loteInfixa[i]);

                    if (!validarExpressao(infixaLimpa))
                    {
                        printf("    Status: INVALIDA (Nao foi possivel calcular)\n");
                    }
                    else
                    {
                        infixaParaPosfixa(infixaLimpa, resultadoPosfixa);
                        double valFinal = 0.0;
                        if (avaliarPosfixa(resultadoPosfixa, &valFinal))
                        {
                            printf("    Posfixa : %s\n", resultadoPosfixa);
                            printf("    Resultado: %.2f\n", valFinal);
                        }
                    }
                }
                printf("=======================================================\n");
            }
            pausar();
            break;

        case 5:
            printf("\nVoltando ao menu principal...\n");
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