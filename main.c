#include <stdio.h>
#include <stdlib.h>
#include "pilha.h"

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

        switch (opcao)
        {
        case 1:
            printf("\n[Em desenvolvimento] Infixa -> Posfixa\n");
            // TODO: Chamar funcao do modulo de pilha/expressoes
            pausar();
            break;
        case 2:
            printf("\n[Em desenvolvimento] Infixa -> Prefixa\n");
            // TODO: Chamar funcao do modulo de pilha/expressoes
            pausar();
            break;
        case 3:
            printf("\n[Em desenvolvimento] Validação de Expressao\n");
            // TODO: Chamar funcao do modulo de pilha/expressoes
            pausar();
            break;
        case 4:
            printf("\n[Em desenvolvimento] Avaliacao de Expressao\n");
            // TODO: Chamar funcao do modulo de pilha/expressoes
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
        ;      // Limpa buffer pendente
    getchar(); // Aguarda a tecla Enter
}