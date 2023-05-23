#include "layout.h"
#include <stdlib.h>

M_Layout M_NewLayout(const M_Token* tokens, size_t token_count) {
    // TODO
    return (M_Layout){
        .tokens = tokens,
        .token_count = token_count,
        .height = 1,
        .width = token_count,
    };
}

void M_PrintLayout(const M_Layout* layout) {
    if (layout == NULL) {
        return;
    }
    printf("< ");
    for (size_t i = 0; i < layout->token_count; i++) {
        M_PrintTok(layout->tokens + i);
        printf(" ");
    }
    printf(">");
};

void M_RenderLayoutTo(const M_Layout* layout, M_Canvas* c) {
    c->init(layout->width, layout->height, c->user_data);

    unsigned char count;
    unsigned int* codepoints = malloc(256 * sizeof(unsigned int));

    for (size_t i = 0; i < layout->token_count; i++) {
        const M_Token* tok = layout->tokens + i;

        if ((count = M_TokCodepoint(tok, codepoints)) == 1) {
            c->drawChar(0, codepoints[0], c->user_data);
        }
    }

    free(codepoints);
}
