#ifndef FILE_UTILS_H
#define FILE_UTILS_H

char** read_ciphertext(const char* filename, int* num_words);
void write_plaintext(const char* filename, const char* plaintext);
char** load_dictionary(const char* filename, int* dict_size);
void free_memory(char** array, int size);
int is_valid_message(const char* message, char** dictionary, int dict_size);

#endif /* FILE_UTILS_H */

