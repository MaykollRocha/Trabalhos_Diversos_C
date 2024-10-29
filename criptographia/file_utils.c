#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_utils.h"

#define MAX_WORDS 1000
#define MAX_WORD_LENGTH 100
#define MAX_LENGTH (MAX_WORDS * MAX_WORD_LENGTH)

char** read_ciphertext(const char* filename, int* num_words) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    char** ciphertext_array = (char**)malloc(MAX_WORDS * sizeof(char*));
    if (!ciphertext_array) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    int count = 0;
    char word[MAX_WORD_LENGTH];
    while (fscanf(file, "%s", word) == 1 && count < MAX_WORDS) {
        ciphertext_array[count] = strdup(word);
        count++;
    }

    fclose(file);
    *num_words = count;
    return ciphertext_array;
}

void write_plaintext(const char* filename, const char* plaintext) {
    FILE* file = fopen(filename, "a");
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%s\n", plaintext);
    fclose(file);
}

int is_valid_message(const char* message, char** dictionary, int dict_size) {
    char temp[MAX_LENGTH];
    strcpy(temp, message);

    char* word = strtok(temp, "_.");
    while (word != NULL) {
        int found = 0;
        for (int i = 0; i < dict_size; ++i) {
            if (strcmp(word, dictionary[i]) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) return 0;
        word = strtok(NULL, "_.");
    }
    return 1;
}


char** load_dictionary(const char* filename, int* dict_size) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    char** dictionary = (char**)malloc(MAX_WORDS * sizeof(char*));
    if (!dictionary) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    char word[MAX_WORD_LENGTH];
    int count = 0;

    while (fgets(word, MAX_WORD_LENGTH, file) && count < MAX_WORDS) {
        word[strcspn(word, "\n")] = '\0';
        dictionary[count] = strdup(word);
        count++;
    }

    fclose(file);
    *dict_size = count;
    return dictionary;
}

void free_memory(char** array, int size) {
    for (int i = 0; i < size; ++i) {
        free(array[i]);
    }
    free(array);
}
