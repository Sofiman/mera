#include <stdio.h>
#include "utf8.h"

#include "tokens.h"
#include "layout.h"

#define MERA_EN_FREETYPE_CANVAS
#include "freetype_canvas.h"

#define UNUSED(x) ((void)(x))

/*
int main()
{
    const char *str = "g̈";
    printf("text = `%s`\n", str);

    const char* cur = str;
    unsigned int c;
    int res = 0;
    while (!(res = utf8_next_codepoint(&cur, &c))) {
        printf("%X\n", c);
    }
    printf("==> %i\n", res);
    return 0;
}*/

int main(int argc, char** argv) {
    UNUSED(argc);
    UNUSED(argv);

    M_Token test[] = {
        (M_Token){
            M_Func,
            &(M_TokenFuncData){
                "max", 3,
                (M_Expr[]){
                    (M_Expr){
                        (M_Token[]){ M_TOK_INT(1), M_TOK_OP("+"), M_TOK_INT(2) },
                        3
                    },
                    (M_Expr){
                        (M_Token[]){ M_TOK_INT(6) },
                        1
                    }
                }, 2
            }
        },
        M_TOK_OP("="),
        M_TOK_INT(6),
    };

    M_Layout l = M_NewLayout(&(M_Expr){test,3});

    printf("l = ");
    M_PrintLayout(&l);
    printf("\n");

    printf("l->width = %u\n", l.width);
    printf("l->height = %u\n", l.height);

    struct FreeTypeCanvasData ftcd;
    ftcd.magic = FREETYPE_CANVAS_DATA_M;
    M_Canvas c = { mera_ftc_init, mera_ftc_drawChar, mera_ftc_measureChar, mera_ftc_save, (void*)&ftcd };

    M_RenderLayoutTo(&l, &c);

    if (!c.save(c.user_data)) {
        fprintf(stderr, "Failed to save canvas\n");
        return 1;
    }
    return 0;
}
