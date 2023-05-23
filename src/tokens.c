#include "tokens.h"

M_Token M_TokenFromChar(char chr) {
    if (chr >= '0' && chr <= '9') {
        return M_TOK_INT(chr - '0');
    }
    if (chr >= 'a' && chr <= 'z') {
        return M_TOK_ID(chr);
    }
    if (chr >= 'A' && chr <= 'Z') {
        return M_TOK_ID(chr);
    }
    if (chr == '+' || chr == '-' || chr == '*' || chr == '/' || chr == '%') {
        return M_TOK_OP(chr);
    }
    return M_TOK_EMPTY; 
}

void M_PrintTok(const M_Token* tok) {
    switch (tok->kind) {
        case M_Empty:
            printf("Empty()");
            break;
        case M_Integer:
            printf("Int(%c)", tok->data);
            break;
        case M_Identifier:
            printf("Id(%c)", tok->data);
            break;
        case M_Op:
            printf("Op(%c)", tok->data);
            break;
        default:
            printf("UNIMPLEMENTED(M_PrintTok, kind: %i)", tok->kind);
            break;
    };
}

unsigned char M_TokCodepoint(const M_Token* tok, unsigned int* codepoints) {
    switch (tok->kind) {
        case M_Empty:
            codepoints[0] = 0x25CC; // ◌
            return 1;
        case M_Integer:
            codepoints[0] = tok->data;
            return 1;
        case M_Identifier:
            codepoints[0] = tok->data;
            return 1;
        case M_Op:
            codepoints[0] = tok->data;
            return 1;
        default:
            printf("UNIMPLEMENTED(M_PrintTok, kind: %i)", tok->kind);
            return 0;
    };
}
