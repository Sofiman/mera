#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <ft2build.h>
#include FT_GLYPH_H
#include "freetype_canvas.h"

void measureText(FT_Face face, const unsigned int* text, size_t* width, size_t* height) {
    FT_GlyphSlot slot = face->glyph;
    FT_Size_Metrics metrics = face->size->metrics;

    int error;
    size_t total = 0;
    size_t maxY = (metrics.ascender - metrics.descender) >> 6;

    for (; *text != 0; text++) {
        FT_UInt glyph_index = FT_Get_Char_Index(face, *text);
        error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
        if (error){
            fprintf(stderr, "Warning: Error Loading glpyh %d\n", glyph_index);
            continue;
        }
        total += slot->advance.x >> 6;

        if (slot->bitmap.rows > maxY) {
            maxY = slot->bitmap.rows;
        }
    }
    *width = total;
    *height = maxY;
}

int renderText(FT_Face face, const unsigned int* text)
{
    size_t width, height;
    measureText(face, text, &width, &height);
    if (width == 0 || height == 0) {
        fprintf(stderr, "empty\n");
        return 1;
    }

    FT_GlyphSlot slot = face->glyph;
    char* tex = calloc(1, width * height);

    int maxAscent = (int)(face->ascender * (face->size->metrics.y_scale / 65536.0)) >> 6;
    //int maxDescent = (int)(fabs(face->descender * (face->size->metrics.y_scale / 65536.0))) >> 6;

    size_t cur_x = 0;

    for(; *text; ++text) {
        if (FT_Load_Char(face, *text, FT_LOAD_RENDER)) {
            fprintf(stderr, "Warning: Error Loading glpyh %d\n", *text);
            continue;
        }

        FT_Bitmap bitmap = slot->bitmap;
        int dy = maxAscent - slot->bitmap_top;
        for (size_t i = 0; i < bitmap.rows; i++) {
            for (size_t j = 0; j < bitmap.width; j++) {
                tex[(dy + i) * width + cur_x + slot->bitmap_left + j] = 
                    bitmap.buffer[i * bitmap.pitch + j];
            }
        }

        cur_x += slot->advance.x >> 6;
        if (cur_x >= width) break;
    }

    return stbi_write_png("out.png", width, height, 1, tex, width);
}

int mera_ftc_init(size_t u_width, size_t u_height, void* user_data) {
    if (user_data == NULL || *((unsigned short*)user_data) != FREETYPE_CANVAS_DATA_M) {
        return 1;
    }
    struct FreeTypeCanvasData* ftc = user_data;

    int error = FT_Init_FreeType(&__library);
    if (error)
    {
        fprintf(stderr, "FT_Init_FreeType failed\n");
        return 1;
    }

    error = FT_New_Face(__library, "tests/fonts/NotoSansMath-Regular.ttf", 0, &ftc->face);
    if (error == FT_Err_Unknown_File_Format)
    {
        fprintf(stderr, "Unknown font format\n");
        return 1;
    }
    else if (error)
    {
        fprintf(stderr, "Unknown error\n");
        return 1;
    }
    error = FT_Set_Char_Size(
          ftc->face, /* handle to face object         */
          0,          /* char_width in 1/64 of points  */
          16*64,      /* char_height in 1/64 of points */
          300,        /* horizontal device resolution  */
          300);
    if (error)
    {
        fprintf(stderr, "FT_Set_Char_Size fail\n");
        return 1;
    }

    FT_Size_Metrics metrics = ftc->face->size->metrics;
    size_t maxHeight = (metrics.ascender - metrics.descender) >> 6;

    ftc->width = u_width * (ftc->face->max_advance_width >> 5);
    ftc->height = u_height * maxHeight;
    ftc->cur_x = 0;

    ftc->tex = calloc(1, ftc->width * ftc->height);
    if (ftc->tex == NULL){
        fprintf(stderr, "Not enough memory\n");
        return 1;
    }
    return 0;
}

void mera_ftc_drawChar(size_t y, unsigned int codepoint, void* user_data) {
    if (user_data == NULL || *((unsigned short*)user_data) != FREETYPE_CANVAS_DATA_M) {
        return;
    }
    struct FreeTypeCanvasData* ftc = user_data;

    FT_Face face = ftc->face;

    if (FT_Load_Char(ftc->face, codepoint, FT_LOAD_RENDER)) {
        fprintf(stderr, "Error: Error Loading glpyh %d\n", codepoint);
        return;
    }

    FT_GlyphSlot slot = face->glyph;
    int maxAscent = (int)(face->ascender * (ftc->face->size->metrics.y_scale / 65536.0)) >> 6;

    FT_Bitmap bitmap = slot->bitmap;
    int dy = maxAscent - slot->bitmap_top;
    for (size_t i = 0; i < bitmap.rows; i++) {
        for (size_t j = 0; j < bitmap.width; j++) {
            ftc->tex[(y + dy + i) * ftc->width + ftc->cur_x + slot->bitmap_left + j] = 
                bitmap.buffer[i * bitmap.pitch + j];
        }
    }

    ftc->cur_x += slot->advance.x >> 6;
}

int mera_ftc_save(void* user_data) {
    if (user_data == NULL || *((unsigned short*)user_data) != FREETYPE_CANVAS_DATA_M) {
        return 1;
    }
    struct FreeTypeCanvasData* ftc = user_data;

    return stbi_write_png("out.png", ftc->cur_x, ftc->height, 1, ftc->tex, ftc->width);
}
