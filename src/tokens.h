#ifndef MIRA_TOKENS_H
#define MIRA_TOKENS_H

#include <stdio.h>

typedef enum M_TokenKind {
    M_Empty,
    M_Integer,
    M_Identifier,
    M_Op,
} M_TokenKind;

typedef struct M_Token {
    M_TokenKind kind;
    int pos;
    char data;
} M_Token;

typedef struct M_TokenInt {
    struct M_Token token;
    int value;
} M_TokenInt;


#define M_TOK_EMPTY ((M_Token){M_Empty,0,0})
#define M_TOK_INT(x) (*((M_Token*)&((M_TokenInt){{M_Integer,0,(x)+'0'},(x)})))
#define M_TOK_ID(x) ((M_Token){M_Identifier,0,(char)(x)})
#define M_TOK_OP(x) ((M_Token){M_Op,0,(char)(x)})

M_Token M_TokenFromChar(char chr);
unsigned char M_TokCodepoint(const M_Token* tok, unsigned int* codepoints);
void M_PrintTok(const M_Token* tok);

#endif
