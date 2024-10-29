#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100

// Estrutura para um n� da lista encadeada
typedef struct Node {
    int key;
    char *data;
    struct Node *next;
} Node;

// Estrutura da tabela hash
typedef struct HashTable {
    Node *table[TABLE_SIZE];
} HashTable;

// Fun��o de hash simples
int hashFunction(int key) {
    return key % TABLE_SIZE;
}

// Fun��o para inicializar a tabela hash
void initHashTable(HashTable *hashTable) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable->table[i] = NULL;
    }
}

// Fun��o para buscar um elemento na tabela hash
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

// Fun��o para inserir um elemento na tabela hash
void insert(HashTable *hashTable, int key, char *data) {
    int hashIndex = hashFunction(key);
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Erro ao alocar mem�ria para o novo n�.\n");
        exit(EXIT_FAILURE);
    }
    newNode->key = key;
    newNode->data = strdup(data);
    newNode->next = NULL;

    // Tratamento de colis�es por encadeamento
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

// Fun��o para remover um elemento da tabela hash
void removeElement(HashTable *hashTable, int key) {
    int hashIndex = hashFunction(key);
    Node *current = hashTable->table[hashIndex];
    Node *prev = NULL;
    while (current != NULL && current->key != key) {
        prev = current;
        current = current->next;
    }
    if (current == NULL) {
        printf("Chave n�o encontrada na tabela hash.\n");
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

// Fun��o para liberar a mem�ria alocada pela tabela hash
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
    printf("Busca ap�s remo��o: Chave 202, Valor: %s\n", search(&hashTable, 202));

    freeHashTable(&hashTable);

    return 0;
}
