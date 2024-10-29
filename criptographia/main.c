/***************************************************************/
/**                                                           **/
/**   Fulano de Tal (� o nome do aluno)          N�mero USP   **/
/**   Exerc�cio-Programa xx                                   **/
/**   Professor: Ciclano de Tal                               **/
/**   Turma: (04, 05, ou 06)                                  **/
/**                                                           **/
/***************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "char_utils.h"
#include "file_utils.h"

int mdc(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

void decrypt(const char* ciphertext, int k, char* plaintext);

int main() {
    int num_words;
    char** ciphertext = read_ciphertext("ciphertext.txt", &num_words);
    int dict_size;
    char** dictionary = load_dictionary("dictionary.txt", &dict_size);
    for(int i = 0; i < num_words; i++) {
        char plaintext[100];
        for (int k = 1; k <= 300; ++k) {
            if(mdc(k,strlen(ciphertext[i]))!=1) continue;
            printf("%i",k);
            decrypt(ciphertext[i], k, plaintext);
            if (is_valid_message(plaintext, dictionary, dict_size)) {
                write_plaintext("plaintext.txt", plaintext);
                break;
            }
        }
    }

    free_memory(ciphertext, num_words);
    free_memory(dictionary, dict_size);

    return 1;
}

void decrypt(const char* ciphertext, int k, char* plaintext) {
    int n = strlen(ciphertext);
    int plaincode[n];
    int ciphercode[n];

    for (int i = 0; i < n; ++i) {
        ciphercode[i] = char_to_code(ciphertext[i]);
    }

    for (int i = 0; i < n; ++i) {
        plaincode[(k * i) % n] = (ciphercode[i] + i) % 28;
    }

    for (int i = 0; i < n; ++i) {
        plaintext[i] = code_to_char(plaincode[i]);
    }
    plaintext[n] = '\0';
}


