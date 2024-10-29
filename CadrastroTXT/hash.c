#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define MAX 7


typedef struct Funcionario{
   int matricula;
   char setor[50];
   char nome[50];
   struct Funcionario *prox;
} TFunc;

// Estrutura da tabela hash
typedef struct _hashfunc {
    struct Funcionario *infos;
}HashFunc;

// Função para inicializar a tabela hash
void init_hash(HashFunc hash[]) {
    for (int i = 0; i < MAX; i++) {
        hash[i].infos = NULL;
    }
}

// Função para limpar os dados de uma lista ligada de Funcionario
void limpar_Funcionarios(TFunc *funcionario) {
    if (funcionario->prox != NULL) {
        limpar_Funcionarios(funcionario->prox);
        free(funcionario);
    }
}

// Função para limpar toda a tabela hash
void limpar_hash(HashFunc hash[]) {
    for (int i = 0; i < MAX; i++) {
        if (hash[i].infos)
            limpar_Funcionarios(hash[i].infos);
    }
}

// Função para buscar um Funcionario na tabela hash
TFunc *buscar(int matricula, HashFunc hash[]) {
    TFunc *aux = hash[matricula % MAX].infos;
    while (aux != NULL && aux->matricula != matricula) {
        aux = aux->prox;
    }
    return aux;
}

// Função para inserir um novo Funcionario na tabela hash
void hash_inserir_funcionario(int matricula, char* nome, char* setor, HashFunc hash[]) {

    TFunc *novo_funcionario = (TFunc *)malloc(sizeof(TFunc));
    novo_funcionario->matricula = matricula;
    strcpy(novo_funcionario->nome, nome);
    strcpy(novo_funcionario->setor, setor);
    novo_funcionario->prox = hash[matricula % MAX].infos;
    hash[matricula % MAX].infos = novo_funcionario;
}

// Função para imprimir todos os Funcionarios presentes na tabela hash
void imprimir_hash(HashFunc hash[], FILE *arq_log) {
    fprintf(arq_log, "-----TABELA HASH COMPLETA----\n");
    for (int i = 0; i < MAX; i++) {
        fprintf(arq_log, "%i | ", i);
        TFunc *funcionario = hash[i].infos;
        while (funcionario != NULL) {
            fprintf(arq_log, "Matricula: %i, Nome: %s, Setor: %s -> ", funcionario->matricula, funcionario->nome, funcionario->setor);
            funcionario = funcionario->prox;
        }
        fprintf(arq_log, "NULL\n");
    }
    fprintf(arq_log, "-----------------------------\n");
}

// Função para remover um Funcionario da tabela hash
void remover_funcionario(int matricula, HashFunc hash[], FILE *arq_log) {
    TFunc *q = hash[matricula % MAX].infos;
    if (!q) {
        fprintf(arq_log, "Não existe Funcionario com matricula %i para ser excluído.\n", matricula);
        return;
    }

    if (q->matricula == matricula) {
        hash[matricula % MAX].infos = q->prox;
        free(q);
        fprintf(arq_log, "Funcionario com matricula %i retirado da tabela.\n", matricula);
        return;
    }

    TFunc *p = buscar(matricula, hash);
    if (!p) {
        fprintf(arq_log, "Não existe Funcionario com matricula %i para ser excluído.\n", matricula);
        return;
    }

    while (q->prox != p)
        q = q->prox;
    q->prox = p->prox;
    free(p);
    fprintf(arq_log, "Funcionario com matricula %i retirado da tabela.\n", matricula);
}
TFunc *criaFuncionario(int matricula, char *setor, char *nome)
{
    TFunc *f = (TFunc *)malloc(sizeof(TFunc));
    // inicializa espacos de memoria com ZEROS
    if (f)
        memset(f, 0, sizeof(TFunc));
    f->matricula = matricula;
    strcpy(f->setor, setor);
    strcpy(f->nome, nome);
    return f;
}

void criarBaseFunc(FILE *func, int tam,HashFunc hash[])
{

    int vet[tam];
    for (int i = 0; i < tam; i++)
        vet[i] = i + 1;

    printf("\nGerando a base de dados...\n");

    for (int i = 0; i < tam; i++)
    {
        TFunc *f = criaFuncionario(vet[i], "A", "01/01-/2001");
        hash_inserir_funcionario(f->matricula,f->nome,f->setor,hash);
        free(f);
    }
}

int main()
{
    FILE *func = fopen("func.dat", "wb+");
    FILE *arq_log = fopen("log.txt", "w");
    if (arq_log == NULL) {
        printf("Erro ao abrir o arquivo de log.\n");
        return 1;
    }
    HashFunc hash[MAX];
    init_hash(hash);
    criarBaseFunc(func, 30,hash);
    imprimir_hash(hash,arq_log);
    fclose(arq_log);
}
/*
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
}

void limpar_hash(Hash hash[]) {
    for (int i = 0; i < MAX; i++) {
        if (hash[i].infos)
            limpar_Dados(hash[i].infos);
    }
}

Dados *buscar(int valor, Hash hash[]) {
    Dados *aux = hash[valor % MAX].infos;
    while (aux != NULL && aux->num != valor) {
        aux = aux->prox;
    }
    return aux;
}

Dados *insert_dados(int valor, Dados *dados) {
    Dados *novo_dado = (Dados *)malloc(sizeof(Dados));
    novo_dado->num = valor;
    novo_dado->prox = dados;
    return novo_dado;
}

void hash_inserir_valor(int valor, Hash hash[], FILE *arq_log) {
    if (buscar(valor, hash)) {
        fprintf(arq_log, "O valor %i já existe na tabela.\n", valor);
        return;
    }
    if (hash[valor % MAX].infos == NULL) {
        hash[valor % MAX].infos = insert_dados(valor, NULL);
    } else {
        hash[valor % MAX].infos = insert_dados(valor, hash[valor % MAX].infos);
    }
    fprintf(arq_log, "Inserido: %i\n", valor);
}

int infos_dados(Dados *dados, FILE *arq_log) {
    fprintf(arq_log, "%d->", dados->num);
    if (dados->prox != NULL) {
        return 1 + infos_dados(dados->prox, arq_log);
    }
    return 1;
}

void imprimir_hash(Hash hash[], FILE *arq_log) {
    int cont = 0;
    fprintf(arq_log, "-----TABELA HASH COMPLETA----\n");
    for (int i = 0; i < MAX; i++) {
        fprintf(arq_log, "%i | ", i);
        if (hash[i].infos)
            cont += infos_dados(hash[i].infos, arq_log);
        fprintf(arq_log, "NULL\n");
    }
    fprintf(arq_log, "Número de valores encontrados em toda a hash é de: %i\n", cont);
    fprintf(arq_log, "-----------------------------\n");
}

void remover_valor(int valor, Hash hash[], FILE *arq_log) {
    Dados *q = hash[valor % MAX].infos;
    if (!q) {
        fprintf(arq_log, "Não existe o valor na coluna %i para ser excluído\n", valor % MAX);
        return;
    }

    if (q->num == valor) {
        hash[valor % MAX].infos = q->prox;
        fprintf(arq_log, "Valor %i retirado da posição.\n", valor);
        return;
    }
    Dados *p = buscar(valor, hash);
    if (!p) {
        fprintf(arq_log, "Não existe o valor %i para ser excluído\n", valor);
        return;
    }
    while (q->prox != p)
        q = q->prox;
    q->prox = p->prox;
    free(p);
    fprintf(arq_log, "Valor %i retirado da posição.\n", valor);
}

void test() {
    Hash hash[MAX];
    FILE *arq_log = fopen("log.txt", "w");
    if (arq_log == NULL) {
        printf("Erro ao abrir o arquivo de log.\n");
        return;
    }

    // Colocar números aleatórios na tabela
    srand(time(NULL));

    fprintf(arq_log, "\nComeçando a inserir valores aleatórios na tabela.\n");

    init_hash(hash);

    // Tenta colocar 50 valores aleatórios de 0 a 99;
    for (int i = 0; i < 50; i++) {
        int aux = rand() % 100;
        hash_inserir_valor(aux, hash, arq_log);
    }
    imprimir_hash(hash, arq_log);
    fprintf(arq_log, "\nRemovendo valores aleatórios da tabela.\n");

    // Tenta colocar 20 valores aleatórios de 0 a 99;
    for (int i = 0; i < 20; i++) {
        int aux = rand() % 100;
        remover_valor(aux, hash, arq_log);
    }
    imprimir_hash(hash, arq_log);
    fprintf(arq_log, "\nBuscando valores aleatórios na tabela.\n");

    // Tenta buscar 20 valores aleatórios na tabela.
    for (int i = 0; i < 20; i++) {
        int aux = rand() % 100;
        buscar(aux, hash) ? fprintf(arq_log, "Valor %i encontrado.\n", aux) : fprintf(arq_log, "Não existe valor %i.\n", aux);
    }

    fclose(arq_log);
    printf("Testes concluídos e arquivo fechado!!!\n");
    limpar_hash(hash);
}
*/
