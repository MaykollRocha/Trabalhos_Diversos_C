#ifndef HASH_H
#define HASH_H

#include <stdio.h>

// Estrutura de um nó de dados
typedef struct _dados {
    int num;
    struct _dados *prox;
} Dados;

// Estrutura da tabela hash
typedef struct _hash {
    struct _dados *infos;
} Hash;

// Inicializa a tabela hash
void init_hash(Hash hash[]);

// Limpa os dados de uma lista encadeada
void limpar_Dados(Dados *dados);

// Limpa toda a tabela hash
void limpar_hash(Hash hash[]);

// Busca por um valor na tabela hash
Dados *buscar(int valor, Hash hash[]);

// Insere um valor na tabela hash
Dados *insert_dados(int valor, Dados *dados);
void hash_inserir_valor(int valor, Hash hash[], FILE *arq_log);

// Remove um valor da tabela hash
void remover_valor(int valor, Hash hash[], FILE *arq_log);

// Imprime a tabela hash
void imprimir_hash(Hash hash[], FILE *arq_log);

// Retorna o número de dados em uma lista encadeada
int infos_dados(Dados *dados, FILE *arq_log);

// Função de teste
void test();

#endif
