#ifndef MIRA_CONTEXT_H
#define MIRA_CONTEXT_H

#include "tokens.h"
#include "render.h"
#include <stdio.h>

typedef struct M_Layout {
    const M_Token* tokens;
    const size_t token_count;

    unsigned int height;
    unsigned int width;
} M_Layout;

M_Layout M_NewLayout(const M_Token* tokens, size_t token_count);
void M_PrintLayout(const M_Layout* layout);
void M_RenderLayoutTo(const M_Layout* layout, M_Canvas* canvas);

#endif
