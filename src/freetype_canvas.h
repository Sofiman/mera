#ifndef MERA_FREETYPE_CANVAS
#define MERA_FREETYPE_CANVAS

#include <ft2build.h>
#include FT_GLYPH_H

FT_Library __library;

#define FREETYPE_CANVAS_DATA_M 64254

struct FreeTypeCanvasData {
    unsigned short magic;
    size_t width;
    size_t height;
    size_t cur_x;
    char* tex;
    FT_Face face;
};

int mera_ftc_init(size_t u_width, size_t u_height, void* user_data);
void mera_ftc_drawChar(size_t y, unsigned int codepoint, void* user_data);
int mera_ftc_save(void* user_data);

#endif
