#include <stdio.h>
#include <string.h>
#include "impressora.h"

#ifdef _WIN32
#include <windows.h> /* Sleep */
#else
#include <unistd.h> /* usleep */
#endif

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
