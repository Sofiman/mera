#ifndef MIRA_RENDER_H
#define MIRA_RENDER_H

#include <stddef.h>

typedef struct M_Canvas {
    int (*const init)(size_t unit_width, size_t unit_height, void* user_data);
    void (*const drawChar)(size_t y, unsigned int codepoint, void* user_data);
    void (*const measureChar)(unsigned int codepoint, void* user_data, size_t* width, size_t* height);
    int (*const save)(void* user_data);
    void* user_data;
} M_Canvas;

#endif
