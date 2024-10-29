#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compartimento_hash.c"
#define MAX_NOME 50

typedef struct {
    int cod_cli;
    char nome_cli[MAX_NOME];
    int prox; // Ponteiro para o próximo elemento na lista encadeada
} Registro;


int main() {
    // Executa o teste "Busca chave inexistente"
    printf("Executando teste: Busca chave inexistente\n");
    ListaCompartimentos *tab_hash;
    ListaClientes *tab_cliente;

    tab_hash = cria_compartimentos(7,
        compartimento_hash(0),
        compartimento_hash(-1),
        compartimento_hash(4),
        compartimento_hash(1),
        compartimento_hash(-1),
        compartimento_hash(2),
        compartimento_hash(-1));
    salva_compartimentos(NOME_ARQUIVO_HASH, tab_hash);

    tab_cliente = cria_clientes(6,
        cliente(49, "Joao", -1, OCUPADO),
        cliente(59, "Maria", 3, OCUPADO),
        cliente(103, "Pedro", -1, OCUPADO),
        cliente(3, "Janio", 5, OCUPADO),
        cliente(51, "Ana", -1, OCUPADO),
        cliente(87, "Mauricio", -1, OCUPADO));
    salva_clientes(NOME_ARQUIVO_DADOS, tab_cliente);

    int end = busca(10, NOME_ARQUIVO_HASH, NOME_ARQUIVO_DADOS, M);

    if (end == -1) {
        printf("Chave 10 nao encontrada.\n");
    } else {
        printf("Erro: Chave 10 encontrada em %d.\n", end);
    }

    libera_compartimentos(tab_hash);
    libera_clientes(tab_cliente);

    return 0;
}
