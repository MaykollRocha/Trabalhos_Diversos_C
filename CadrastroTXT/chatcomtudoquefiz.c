#include <stdio.h>
#include <stdlib.h>
#define MAX 7

typedef struct _dados {
    int num;
    struct _dados *prox;
} Dados;

typedef struct _hash {
    struct _dados *infos;
} Hash;

void init_hash(Hash hash[]) {
    for (int i = 0; i < MAX; i++) {
        hash[i].infos = NULL;
    }
}

void limpar_Dados(Dados *dados) {
    if (dados->prox != NULL) {
        limpar_Dados(dados->prox);
        free(dados);
    }
    return;
}

void limpar_hash(Hash hash[]) {
    for (int i = 0; i < MAX; i++) {
        if (hash[i].infos)
            limpar_Dados(hash[i].infos);
    }
}

Dados *insert_dados(int valor, Dados *dados) {
    Dados *aux = (Dados *)malloc(sizeof(Dados));
    aux->num = valor;
    aux->prox = dados;
    return aux;
}

void hash_inserir_valor(Hash hash[], int valor) {
    if (hash[valor % MAX].infos == NULL) {
        hash[valor % MAX].infos = insert_dados(valor, NULL);
    } else {
        hash[valor % MAX].infos = insert_dados(valor, hash[valor % MAX].infos);
    }
    return;
}

void infos_dados(Dados *dados) {
    printf("%i->", dados->num);
    if (dados->prox != NULL) {
        infos_dados(dados->prox);
    }
}

void imprimir_hash(Hash hash[]) {
    for (int i = 0; i < MAX; i++) {
        printf("%i | ", i);
        if (hash[i].infos)
            infos_dados(hash[i].infos);
        printf("NULL\n");
    }
}

Dados *buscar(int valor, Hash hash[]) {
    Dados *aux = hash[valor % MAX].infos;
    while (aux != NULL && aux->num != valor) {
        aux = aux->prox;
    }
    return aux;
}

void remover_valor(int valor, Hash hash[]) {
    Dados *q = hash[valor % MAX].infos;
    Dados *p = buscar(valor, hash);
    if (!p || !q)
        return;
    while (q->prox != p)
        q = q->prox;
    q->prox = p->prox;
    free(p);
    return;
}

void testar_operacoes(Hash hash[], FILE *arq_log) {
    // Inserções
    for (int i = 0; i < 10; i++) {
        hash_inserir_valor(hash, i);
        fprintf(arq_log, "Inserido: %d\n", i);
    }

    // Impressão da tabela hash
    fprintf(arq_log, "\nTabela Hash após as inserções:\n");
    imprimir_hash(hash);

    // Remoção de um valor
    remover_valor(7, hash);
    fprintf(arq_log, "\nRemovido o valor 7.\n");

    // Busca por um valor
    int valor_busca = 11;
    Dados *resultado = buscar(valor_busca, hash);
    if (resultado)
        fprintf(arq_log, "\nO valor %d foi encontrado.\n", valor_busca);
    else
        fprintf(arq_log, "\nO valor %d não foi encontrado.\n", valor_busca);

    // Impressão da tabela hash após as operações
    fprintf(arq_log, "\nTabela Hash após as operações:\n");
    imprimir_hash(hash);
}

int main() {
    Hash hash[MAX];
    FILE *arq_log = fopen("log.txt", "w");
    if (arq_log == NULL) {
        printf("Erro ao abrir o arquivo de log.\n");
        return 1;
    }

    init_hash(hash);
    testar_operacoes(hash, arq_log);
    limpar_hash(hash);

    fclose(arq_log);
    return 0;
}
