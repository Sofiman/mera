#ifndef MIRA_TOKENS_H
#define MIRA_TOKENS_H

#include <stdio.h>

typedef enum M_TokenKind {
    M_Empty,
    M_Integer,
    M_Identifier,
    M_Op,
    M_Func
} M_TokenKind;

typedef struct M_Token {
    M_TokenKind kind;
    void* data;
} M_Token;

typedef struct M_Expr {
    M_Token* tokens;
    size_t tokens_count;
} M_Expr;

typedef struct M_TokenAsciiData {
    const char* str;
    size_t str_len;
} M_TokenAsciiData;

typedef struct M_TokenIntData {
    long value;
} M_TokenIntData;

typedef struct M_TokenFuncData {
    const char* name;
    size_t name_len;
    struct M_Expr* params;
    size_t params_count;
} M_TokenFuncData;

#define M_TOK_EMPTY ((M_Token){M_Empty,NULL})
#define M_TOK_ASCII(kind,c,l) ((M_Token){(kind),&((M_TokenAsciiData){(c),(l)})})
#define M_TOK_INT(x) ((M_Token){M_Integer,&((M_TokenIntData){(x)})})

#define M_TOK_ID(x) M_TOK_ASCII((M_Identifier),(x),1)
#define M_TOK_OP(x) M_TOK_ASCII((M_Op),(x),1)

M_Token M_TokenFromChar(char chr);
size_t M_TokSize(const M_Token* tok);
size_t M_ExprToCodepoint(const M_Expr* expr, unsigned int* codepoints);
size_t M_TokCodepoint(const M_Token* tok, unsigned int* codepoints);
void M_PrintTok(const M_Token* tok);

#endif
