#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STRING 64

void cadastrarPessoa(FILE *arquivo);
void pesquisarPessoa(FILE *arquivo);
void removerPessoa(FILE *arquivo);
void listarPessoas(FILE *arquivo);

int main() {
    FILE *arquivo;
    int opcao;

    arquivo = fopen("cadastro.txt", "a+");  // Abre o arquivo para leitura e escrita (append)

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    do {
        printf("-------------------\n");
        printf("        Menu       \n");
        printf("1. Inserir pessoa  \n");
        printf("2. Pesquisar pessoa\n");
        printf("3. Remover pessoa  \n");
        printf("4. Listar pessoas  \n");
        printf("5. Sair            \n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        printf("-------------------\n");
        switch (opcao) {
            case 1:
                cadastrarPessoa(arquivo);
                break;
            case 2:
                pesquisarPessoa(arquivo);
                break;
            case 3:
                removerPessoa(arquivo);
                break;
            case 4:
                listarPessoas(arquivo);
                break;
            case 5:
                printf("Saindo do programa.\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 5);

    fclose(arquivo);

    return 1;//Retorno Bem sucessedido
}

void cadastrarPessoa(FILE *arquivo) {
    char nome[MAX_STRING], telefone[MAX_STRING], cidade[MAX_STRING];
    int idade;

    printf("Digite o nome: ");
    scanf(" %[^\n]s", nome);
    printf("Digite o telefone: ");
    scanf(" %[^\n]s", telefone);
    printf("Digite a idade: ");
    scanf("%d", &idade);
    printf("Digite a cidade: ");
    scanf(" %[^\n]s", cidade);

    fprintf(arquivo, "%s | %s | %d | %s\n", nome, telefone, idade, cidade);

    printf("Pessoa cadastrada com sucesso.\n");
}

void pesquisarPessoa(FILE *arquivo) {
    char nomePesquisa[MAX_STRING];
    printf("Digite o nome da pessoa que deseja pesquisar: ");
    scanf(" %[^\n]s", nomePesquisa);

    char linha[MAX_STRING * 4];  // Uma linha no arquivo tem, no máximo, 4 strings
    fseek(arquivo, 0, SEEK_SET);  // Volta para o início do arquivo

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (strstr(linha, nomePesquisa) != NULL) {
            printf("Pessoa encontrada:\n%s", linha);
            return;
        }
    }

    printf("Pessoa nao encontrada.\n");
}

void removerPessoa(FILE *arquivo) {
    char nomeRemocao[MAX_STRING];
    printf("Digite o nome da pessoa que deseja remover: ");
    scanf(" %[^\n]s", nomeRemocao);

    FILE *temporario = fopen("temporario.txt", "w");
    if (temporario == NULL) {
        printf("Erro ao abrir arquivo temporario.\n");
        exit(1);
    }

    char linha[MAX_STRING * 4];
    fseek(arquivo, 0, SEEK_SET);

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (strstr(linha, nomeRemocao) == NULL) {
            fprintf(temporario, "%s", linha);
        }else{
            printf("Dados excluidos\n %s",linha);
        }
    }

    fclose(temporario);
    fclose(arquivo);

    remove("cadastro.txt");
    rename("temporario.txt", "cadastro.txt");

    arquivo = fopen("cadastro.txt", "a+");
    printf("Pessoa removida com sucesso.\n");
}

void listarPessoas(FILE *arquivo) {
    char linha[MAX_STRING * 4];
    fseek(arquivo, 0, SEEK_SET);

    printf("\nLista de pessoas cadastradas:\n");
    printf("NOME | TELEFONE | IDADE | CIDADE\n");
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        printf("%s", linha);
    }

}
