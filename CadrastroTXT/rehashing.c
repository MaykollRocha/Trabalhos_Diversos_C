#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100

// Estrutura para um nó da lista encadeada
typedef struct Node {
    int key;
    char *data;
    struct Node *next;
} Node;

// Estrutura da tabela hash
typedef struct HashTable {
    Node *table[TABLE_SIZE];
} HashTable;

// Função de hash simples
int hashFunction(int key) {
    return key % TABLE_SIZE;
}

// Função para inicializar a tabela hash
void initHashTable(HashTable *hashTable) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable->table[i] = NULL;
    }
}

// Função para buscar um elemento na tabela hash
char *search(HashTable *hashTable, int key) {
    int hashIndex = hashFunction(key);
    Node *current = hashTable->table[hashIndex];
    while (current != NULL) {
        if (current->key == key) {
            return current->data;
        }
        current = current->next;
    }
    return NULL;
}

// Função para inserir um elemento na tabela hash
void insert(HashTable *hashTable, int key, char *data) {
    int hashIndex = hashFunction(key);
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Erro ao alocar memória para o novo nó.\n");
        exit(EXIT_FAILURE);
    }
    newNode->key = key;
    newNode->data = strdup(data);
    newNode->next = NULL;

    // Tratamento de colisões por encadeamento
    if (hashTable->table[hashIndex] == NULL) {
        hashTable->table[hashIndex] = newNode;
    } else {
        Node *current = hashTable->table[hashIndex];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

// Função para remover um elemento da tabela hash
void removeElement(HashTable *hashTable, int key) {
    int hashIndex = hashFunction(key);
    Node *current = hashTable->table[hashIndex];
    Node *prev = NULL;
    while (current != NULL && current->key != key) {
        prev = current;
        current = current->next;
    }
    if (current == NULL) {
        printf("Chave não encontrada na tabela hash.\n");
        return;
    }
    if (prev == NULL) {
        hashTable->table[hashIndex] = current->next;
    } else {
        prev->next = current->next;
    }
    free(current->data);
    free(current);
}

// Função para liberar a memória alocada pela tabela hash
void freeHashTable(HashTable *hashTable) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node *current = hashTable->table[i];
        while (current != NULL) {
            Node *temp = current;
            current = current->next;
            free(temp->data);
            free(temp);
        }
    }
}

int main() {
    HashTable hashTable;
    initHashTable(&hashTable);

    insert(&hashTable, 101, "Alice");
    insert(&hashTable, 202, "Bob");
    insert(&hashTable, 303, "Charlie");

    printf("Busca: Chave 202, Valor: %s\n", search(&hashTable, 202));
    printf("Busca: Chave 404, Valor: %s\n", search(&hashTable, 404));

    removeElement(&hashTable, 202);
    printf("Busca após remoção: Chave 202, Valor: %s\n", search(&hashTable, 202));

    freeHashTable(&hashTable);

    return 0;
}
