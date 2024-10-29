#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int codigo;
    char *nome;
    int quantidade;
    float preco;
    char estado[3]; // Aumentei o tamanho para acomodar o '\n' e o '\0'
} Produto;

int lerDadosArquivo(char *nomeArquivo, Produto **listaProdutos) {
    FILE *arquivo = fopen(nomeArquivo, "r");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    int quantidade;
    fscanf(arquivo, "%d\n", &quantidade);

    *listaProdutos = (Produto *)malloc(quantidade * sizeof(Produto));

    for (int i = 0; i < quantidade; i++) {
        fscanf(arquivo, "%d\n", &(*listaProdutos)[i].codigo);

        char buffer[100];
        fgets(buffer, sizeof(buffer), arquivo);
        (*listaProdutos)[i].nome = strdup(buffer);

        fscanf(arquivo, "%d\n%f\n%s", &(*listaProdutos)[i].quantidade,
               &(*listaProdutos)[i].preco, (*listaProdutos)[i].estado);
    }

    fclose(arquivo);
    return quantidade;
}

void liberarMemoria(Produto *listaProdutos, int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        free(listaProdutos[i].nome);
    }
    free(listaProdutos);
}

int compararNomes(const void *a, const void *b) {
    const Produto *produtoA = (const Produto *)a;
    const Produto *produtoB = (const Produto *)b;
    return strcmp(produtoA->nome, produtoB->nome);
}

void gerarRelatorio(char *nomeArquivo, Produto *listaProdutos, int quantidade) {
    qsort(listaProdutos, quantidade, sizeof(Produto), compararNomes);

    FILE *relatorio = fopen(nomeArquivo, "w");

    if (relatorio == NULL) {
        printf("Erro ao criar o arquivo de relatório.\n");
        exit(1);
    }

    fprintf(relatorio, "%d\n", quantidade);

    for (int i = 0; i < quantidade; i++) {
            printf(relatorio, "%d\n%s%d\n%.2f\n%s",
                listaProdutos[i].codigo,
                listaProdutos[i].nome,
                listaProdutos[i].quantidade,
                listaProdutos[i].preco,
                listaProdutos[i].estado);
    }

    fclose(relatorio);
}

void imprimirProdutoPorCodigo(Produto *listaProdutos, int quantidade, int codigo) {
    for (int i = 0; i < quantidade; i++) {
        if (listaProdutos[i].codigo == codigo) {
            printf("%d\n", listaProdutos[i].codigo);
            printf("%s", listaProdutos[i].nome);
            printf("%d\n", listaProdutos[i].quantidade);
            printf("%.2f\n", listaProdutos[i].preco);
            printf("%s", listaProdutos[i].estado);
            return;
        }
    }

    printf("Produto com código %d não encontrado.\n", codigo);
}

void imprimirProdutoMenorQuantidade(Produto *listaProdutos, int quantidade) {
    if (quantidade == 0) {
        printf("Lista de produtos vazia.\n");
        return;
    }

    int menorQuantidade = listaProdutos[0].quantidade;
    int indiceMenorQuantidade = 0;

    for (int i = 1; i < quantidade; i++) {
        if (listaProdutos[i].quantidade < menorQuantidade) {
            menorQuantidade = listaProdutos[i].quantidade;
            indiceMenorQuantidade = i;
        }
    }

    printf("%d\n", listaProdutos[indiceMenorQuantidade].codigo);
    printf("%s", listaProdutos[indiceMenorQuantidade].nome);
    printf("%d\n", listaProdutos[indiceMenorQuantidade].quantidade);
    printf("%.2f\n", listaProdutos[indiceMenorQuantidade].preco);
    printf("%s", listaProdutos[indiceMenorQuantidade].estado);
}

void imprimirProdutosPorEstado(Produto *listaProdutos, int quantidade, char *estado) {
    for (int i = 0; i < quantidade; i++) {
        if (strcmp(listaProdutos[i].estado, estado) == 0) {
            printf("%d\n", listaProdutos[i].codigo);
            printf("%s", listaProdutos[i].nome);
            printf("%d\n", listaProdutos[i].quantidade);
            printf("%.2f\n", listaProdutos[i].preco);
            printf("%s", listaProdutos[i].estado);
        }
    }
}

void encontrarMenorQuantidadePorEstado(Produto *listaProdutos, int quantidade, char *estado) {
    int menorQuantidade = -1;
    int indiceMenorQuantidade = -1;

    for (int i = 0; i < quantidade; i++) {
        if (strcmp(listaProdutos[i].estado, estado) == 0) {
            if (menorQuantidade == -1 || listaProdutos[i].quantidade < menorQuantidade) {
                menorQuantidade = listaProdutos[i].quantidade;
                indiceMenorQuantidade = i;
            }
        }
    }

    if (indiceMenorQuantidade != -1) {
        printf("%d\n", listaProdutos[indiceMenorQuantidade].codigo);
        printf("%s", listaProdutos[indiceMenorQuantidade].nome);
        printf("%d\n", listaProdutos[indiceMenorQuantidade].quantidade);
        printf("%.2f\n", listaProdutos[indiceMenorQuantidade].preco);
        printf("%s", listaProdutos[indiceMenorQuantidade].estado);
    } else {
        printf("Nenhum produto encontrado para o estado %s.", estado);
    }
}

int calcularQuantidadeTotal(Produto *listaProdutos, int quantidade) {
    int quantidadeTotal = 0;

    for (int i = 0; i < quantidade; i++) {
        quantidadeTotal += listaProdutos[i].quantidade;
    }

    return quantidadeTotal;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso: %s nome_do_arquivo.txt opcao [parametro]\n", argv[0]);
        return 1;
    }

    Produto *listaProdutos;
    int quantidadeProdutos = lerDadosArquivo(argv[1], &listaProdutos);

    int opcao = atoi(argv[2]);

    switch (opcao) {
    case 1:
        if (argc != 4) {
            printf("Uso: %s nome_do_arquivo.txt 1 nome_do_relatorio.txt\n", argv[0]);
            return 1;
        }
        gerarRelatorio(argv[3], listaProdutos, quantidadeProdutos);
        break;

    case 2:
        if (argc != 4) {
            printf("Uso: %s nome_do_arquivo.txt 2 codigo_do_produto\n", argv[0]);
            return 1;
        }
        imprimirProdutoPorCodigo(listaProdutos, quantidadeProdutos, atoi(argv[3]));
        break;

    case 3:
        imprimirProdutoMenorQuantidade(listaProdutos, quantidadeProdutos);
        break;

    case 4:
        if (argc != 4) {
            printf("Uso: %s nome_do_arquivo.txt 4 estado\n", argv[0]);
            return 1;
        }
        imprimirProdutosPorEstado(listaProdutos, quantidadeProdutos, argv[3]);
        break;

    case 5:
        if (argc != 4) {
            printf("Uso: %s nome_do_arquivo.txt 5 estado\n", argv[0]);
            return 1;
        }
        encontrarMenorQuantidadePorEstado(listaProdutos, quantidadeProdutos, argv[3]);
        break;

    case 6:
        printf("%i\n", calcularQuantidadeTotal(listaProdutos, quantidadeProdutos));
        break;

    default:
        printf("Opção inválida.\n");
        break;
    }

    liberarMemoria(listaProdutos, quantidadeProdutos);

    return 0;
}
