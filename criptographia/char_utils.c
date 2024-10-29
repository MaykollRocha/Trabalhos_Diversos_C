#include "char_utils.h"

int char_to_code(char c) {
    if (c == '_') return 0;
    if (c == '.') return 27;
    return c - 'a' + 1;
}

char code_to_char(int code) {
    if (code == 0) return '_';
    if (code == 27) return '.';
    return code + 'a' - 1;
}

