#ifndef MERA_UTF8_H
#define MERA_UTF8_H

#include <stddef.h>

#define UTF8_END_OF_STRING 1
#define UTF8_ERR_INCOMPLETE_MULTIBYTE 2 // end of string reached in the middle of a multi-byte codepoint
#define UTF8_ERR_INVALID_START 3 // cursor in the middle of a multi-byte codepoint
                                 //
//const char* str = "π∞×÷≥≤√◌";
//const char *str = "Hello, 世界!";
//const char *str = "g̈நிक्षि";
//const char *str = "Hello, \xff\xaf\xff\xff\xff\xff\xff\xff";


size_t utf8_len(const char* str);
int utf8_is_valid(unsigned int codepoint);
int utf8_next_codepoint(const char** str, unsigned int* codepoint);
unsigned int* utf8_to_codepoints(const char* str);

#endif
