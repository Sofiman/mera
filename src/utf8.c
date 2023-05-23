#include "utf8.h"
#include <stdlib.h>

static unsigned char const utf8_byte_count[] = {
// 0 1 2 3 4 5 6 7 8 9 A B C D E F
   1,1,1,1,1,1,1,1,0,0,0,0,2,2,3,4
};

size_t utf8_len(const char* str)
{
    size_t count = 0;
    while (*str) {
        if ((*(str++) & 0xC0) != 0x80) ++count;
    }
    return count;
}

int utf8_is_valid(unsigned int codepoint)
{
    return (codepoint <= 0x10FFFF) && ((codepoint < 0xD800) || (codepoint > 0xDFFF));
}

int utf8_next_codepoint(const char** str, unsigned int* codepoint)
{
    if (**str == 0) {
        return UTF8_END_OF_STRING;
    }
    if ((**str & 0xC0) == 0x80) {
        return UTF8_ERR_INVALID_START;
    }

    unsigned char bytes = utf8_byte_count[(**str & 0xFF) >> 4];
    if (bytes == 0) {
        *codepoint = 0xFFFD;
        ++(*str); // advance by in the string
    } else if (bytes == 1) {
        *codepoint = **str;
        ++(*str); // advance by in the string
    } else {
        unsigned int code = (*(*str)++) & ((1 << (8 - bytes)) - 1);
        for (--bytes; bytes > 0 && ((**str) & 0xC0) == 0x80; --bytes, ++(*str)) {
            code = (code << 6) | ((**str) & 0x3F);
        }
        if (bytes != 0) {
            if (**str == 0) {
                return UTF8_ERR_INCOMPLETE_MULTIBYTE;
            }
            code = 0xFFFD;
        } else if (!utf8_is_valid(code)) {
            code = 0xFFFD;
        }
        *codepoint = code;
    }
    return 0;
}

unsigned int* utf8_to_codepoints(const char* str) {
    size_t count = utf8_len(str);
    unsigned int* buf = malloc(sizeof(unsigned int) * (count + 1));

    size_t i = 0;
    int res;
    while (!(res = utf8_next_codepoint(&str, buf + i))) {
        i++;
    }

    if (res != UTF8_END_OF_STRING) {
        free(buf); // invalid string
        return NULL;
    }

    buf[i] = 0;
    return buf;
}
