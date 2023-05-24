#include "tokens.h"
#include "utf8.h"
#include <stdlib.h>
#include <string.h>

M_Token M_TokenFromChar(char chr) {
    if (chr >= '0' && chr <= '9') {
        return M_TOK_INT(chr - '0');
    }
    if (chr >= 'a' && chr <= 'z') {
        return M_TOK_ID(&chr);
    }
    if (chr >= 'A' && chr <= 'Z') {
        return M_TOK_ID(&chr);
    }
    if (chr == '+' || chr == '-' || chr == '*' || chr == '/' || chr == '%') {
        return M_TOK_OP(&chr);
    }
    return M_TOK_EMPTY; 
}

size_t M_TokSize(const M_Token* tok) {
    switch (tok->kind) {
        case M_Empty:
            return 1;
        case M_Integer: {
            M_TokenIntData* data = (M_TokenIntData*)tok->data;
            long v = data->value;
            size_t count = v < 0 ? 1 : 0;
            while (v > 0) {
                v /= 10;
                ++count;
            }
            return count;
        };
        case M_Identifier:
        case M_Op: {
            M_TokenAsciiData* data = (M_TokenAsciiData*)tok->data;
            return data->str_len;
        };
        case M_Func: {
            M_TokenFuncData* data = (M_TokenFuncData*)tok->data;
            size_t total = data->name_len;
            for (size_t i = 0; i < data->params_count; i++) {
                const M_Expr* expr = data->params + i;
                for (size_t j = 0; j < expr->tokens_count; j++) {
                    total += M_TokSize(expr->tokens + j);
                }
            }
            return total;
        };
        default:
            printf("UNIMPLEMENTED(M_PrintTok, kind: %i)", tok->kind);
            return 0;
    };
}

void M_PrintTok(const M_Token* tok) {
    switch (tok->kind) {
        case M_Empty:
            printf("Empty()");
            break;
        case M_Integer: {
            M_TokenIntData* data = (M_TokenIntData*)tok->data;
            printf("Int(%li)", data->value);
        } break;
        case M_Identifier: {
            M_TokenAsciiData* data = (M_TokenAsciiData*)tok->data;
            printf("Id(");
            for (size_t i = 0; i < data->str_len; i++){
                printf("%c", data->str[i]);
            }
            printf(")");
        } break;
        case M_Op: {
            M_TokenAsciiData* data = (M_TokenAsciiData*)tok->data;
            printf("Op(");
            for (size_t i = 0; i < data->str_len; i++){
                printf("%c", data->str[i]);
            }
            printf(")");
        } break;
        case M_Func: {
            M_TokenFuncData* data = (M_TokenFuncData*)tok->data;
            printf("Func::%s(", data->name);
            for (size_t i = 0; i < data->params_count; ++i) {
                const M_Expr* expr = data->params + i;
                for (size_t j = 0; j < expr->tokens_count; ++j) {
                    printf(" ");
                    M_PrintTok(expr->tokens + j);
                }
                printf(",");
            }
            printf(")");
        } break;
        default:
            printf("UNIMPLEMENTED(M_PrintTok, kind: %i)", tok->kind);
            break;
    };
}

size_t M_ExprToCodepoint(const M_Expr* expr, unsigned int* codepoints) {
    size_t written = 0;
    for (size_t i = 0; i < expr->tokens_count; ++i) {
        written += M_TokCodepoint(expr->tokens + i, codepoints + written);
    }
    return written;
}

size_t M_TokCodepoint(const M_Token* tok, unsigned int* codepoints) {
    switch (tok->kind) {
         case M_Empty:
             codepoints[0] = 0x25CC; // ◌
             return 1;
         case M_Integer: {
             M_TokenIntData* data = (M_TokenIntData*)tok->data;
             size_t count = 0;
             long v = data->value;
 
             while(v > 0) {
                 codepoints[count++] = '0' + v % 10;
                 v /= 10;
             }
             return count;
         };
         case M_Identifier:
         case M_Op: {
             M_TokenAsciiData* data = (M_TokenAsciiData*)tok->data;
             size_t total = 0;
             unsigned int* chars = utf8_to_codepoints(data->str);
             if (chars != NULL) {
                 unsigned int* src = chars;
                 while (*src) {
                     codepoints[total++] = *(src++);
                 }
                 free(chars);
             }
             return total;
        };
        case M_Func: {
             M_TokenFuncData* data = (M_TokenFuncData*)tok->data;
             size_t total = 0;
             unsigned int* chars = utf8_to_codepoints(data->name);
             if (chars != NULL) {
                 unsigned int* src = chars;
                 while (*src) {
                     codepoints[total++] = *(src++);
                 }
                 free(chars);
             }

             codepoints[total++] = '(';
             for (size_t i = 0; i < data->params_count; ++i, ++total) {
                 total += M_ExprToCodepoint(data->params + i, codepoints + total);
                 codepoints[total] = ',';
             }
             codepoints[data->params_count > 0 ? total - 1 : total] = ')';

             return total;
        };
        default:
            printf("UNIMPLEMENTED(M_TokCodepoint, kind: %i)", tok->kind);
            return 0;
    };
}
