#include "layout.h"
#include <stdlib.h>

M_Layout M_NewLayout(const M_Expr* expr) {
    size_t width = 0;

    for (size_t i = 0; i < expr->tokens_count; i++) {
        width += M_TokSize(expr->tokens + i);
    }

    return (M_Layout){
        .expr = expr,
        .height = 1,
        .width = width,
    };
}

void M_PrintLayout(const M_Layout* layout) {
    if (layout == NULL) {
        return;
    }
    printf("< ");
    const M_Expr* expr = layout->expr;
    for (size_t i = 0; i < expr->tokens_count; i++) {
        M_PrintTok(expr->tokens + i);
        printf(" ");
    }
    printf(">");
};

void M_RenderLayoutTo(const M_Layout* layout, M_Canvas* c) {
    c->init(layout->width, layout->height, c->user_data);

    unsigned char count;
    unsigned int* codepoints = malloc(1024 * sizeof(unsigned int));
    const M_Expr* expr = layout->expr;

    for (size_t i = 0; i < expr->tokens_count; i++) {
        const M_Token* tok = expr->tokens + i;

        count = M_TokCodepoint(tok, codepoints);
        for (size_t i = 0; i < count; i++) {
            c->drawChar(0, codepoints[i], c->user_data);
        }
    }

    free(codepoints);
}
