#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    int codigo;
    char *nome;
    int quantidade;
    float preco;
    char estado[2];
}produto;

int compararNomes(const void *a, const void *b) {
    const produto *produtoA = (const produto *)a;
    const produto *produtoB = (const produto *)b;
    return strcmp(produtoA->nome, produtoB->nome);
}

void gerarRelatorio(char *nomeArquivo, produto *listaProdutos, int quantidade) {
    // Ordena a lista de produtos por nome em ordem alfabética
    qsort(listaProdutos, quantidade, sizeof(produto), compararNomes);

    // Cria e abre o arquivo para escrita
    FILE *relatorio = fopen(nomeArquivo, "w");
    
    if (relatorio == NULL) {
        printf("Erro ao criar o arquivo de relatório.\n");
        exit(1);
    }

    for (int i = 0; i < quantidade; i++) {
        fprintf(relatorio, "%d\n%s%d\n%0.2f\n%s",
                listaProdutos[i].codigo,
                listaProdutos[i].nome,
                listaProdutos[i].quantidade,
                listaProdutos[i].preco,
                listaProdutos[i].estado);
    }

    // Fecha o arquivo
    fclose(relatorio);
}

void imprimirProdutoPorCodigo(produto *listaProdutos, int quantidade, int codigo) {
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

void imprimirProdutoMenorQuantidade(produto *listaProdutos, int quantidade) {
    if (quantidade == 0) {
        printf("Lista de produtos vazia.\n");
        return;
    }

    // Inicializa com a primeira quantidade encontrada
    int menorQuantidade = listaProdutos[0].quantidade;
    int indiceMenorQuantidade = 0;

    // Percorre a lista para encontrar o produto com menor quantidade
    for (int i = 1; i < quantidade; i++) {
        if (listaProdutos[i].quantidade < menorQuantidade) {
            menorQuantidade = listaProdutos[i].quantidade;
            indiceMenorQuantidade = i;
        }
    }

    // Imprime os dados do produto com menor quantidade
    printf("%d\n", listaProdutos[indiceMenorQuantidade].codigo);
    printf("%s", listaProdutos[indiceMenorQuantidade].nome);
    printf("%d\n", listaProdutos[indiceMenorQuantidade].quantidade);
    printf("%.2f\n", listaProdutos[indiceMenorQuantidade].preco);
    printf("%s", listaProdutos[indiceMenorQuantidade].estado);
}

void imprimirProdutosPorEstado(produto *listaProdutos, int quantidade, char *estado) {
    // Percorre a lista de produtos para imprimir os dados dos produtos do estado fornecido
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

void encontrarMenorQuantidadePorEstado(produto *listaProdutos, int quantidade, char *estado) {
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
        // Imprime os dados do produto com menor quantidade no estado
        printf("%d\n", listaProdutos[indiceMenorQuantidade].codigo);
        printf("%s", listaProdutos[indiceMenorQuantidade].nome);
        printf("%d\n", listaProdutos[indiceMenorQuantidade].quantidade);
        printf("%.2f\n", listaProdutos[indiceMenorQuantidade].preco);
        printf("%s", listaProdutos[indiceMenorQuantidade].estado);
    } else {
        printf("Nenhum produto encontrado para o estado %s.", estado);
    }
}

int calcularQuantidadeTotal(produto *listaProdutos, int quantidade) {
    int quantidadeTotal = 0;

    for (int i = 0; i < quantidade; i++) {
        quantidadeTotal += listaProdutos[i].quantidade;
    }

    return quantidadeTotal;
}

int main(int argc,char *argv[])
{
    //Abre o arquivo
    char linha[100]; 
    FILE *arquivo = fopen(argv[1], "r");
    //Se não abrir da erro
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }
    
    //A primeira linha tem a quantidade de itens
    fgets(linha, sizeof(linha), arquivo);
    int Q;
    Q = atoi(linha);
    //Cria um vetor de prutodos da quantidade
    produto lista_produt[Q];
    
    int infos = 0,param = 0;
    
    //Descarrega o arquivo
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (infos == 0) lista_produt[param].codigo = atoi(linha);

        if (infos == 1) {
            lista_produt[param].nome = (char *)malloc(strlen(linha) + 1);
            strcpy(lista_produt[param].nome , linha);
        }

        if (infos == 2) lista_produt[param].quantidade = atoi(linha);

        if (infos == 3) lista_produt[param].preco = atof(linha);

        if (infos == 4){
            strcpy(lista_produt[param].estado, linha);
            infos  = -1;
            param ++;
        } 
        infos ++;
    }
    //Fecha
    fclose(arquivo);
    
    //Começa o processamento
    switch (atoi(argv[2]))
    {
    case 1: 
        if (argc == 4) gerarRelatorio(argv[3], lista_produt, Q);
        break;

    case 2:
         if (argc == 4) imprimirProdutoPorCodigo(lista_produt, Q, atoi(argv[3]));
        break;

    case 3:
        imprimirProdutoMenorQuantidade(lista_produt, Q);
        break;

    case 4:
        if (argc == 4) imprimirProdutosPorEstado(lista_produt, Q, strcat(argv[3],"\n"));
        break;
    case 5:
         if (argc == 4) encontrarMenorQuantidadePorEstado(lista_produt, Q, strcat(argv[3],"\n"));
        break;
    case 6:
        printf("%i",calcularQuantidadeTotal(lista_produt,Q));
        break;

    default:
        break;
    }
    
    return 1;
}