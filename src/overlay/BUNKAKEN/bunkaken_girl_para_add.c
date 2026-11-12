/* @CompileFlags: -cckr -Wc,-pic0 */
/* @CompileEnv: IDO52_NO_CSE_SITE=mpy:2 */
/* bunkaken_girl_para_add - BUNKAKEN overlay, Address: 0x80145548, Size: 756 bytes (189 insns) */
#include "types.h"

typedef struct { u8 t[9][6]; } Buf9;
typedef struct { u8 t[11][6]; } Buf11;
typedef struct { s16 h[28]; } Row;

extern Buf9 bunkaken_girl_para_add_tbl0;
extern Buf11 bunkaken_girl_para_add_tbl1;
extern Buf11 bunkaken_girl_para_add_tbl2;

extern s32 bunka_girl2;
extern s32 bunka_bukatu;
extern s32 bunka_girl;

extern Row screen_w[];

void bunkaken_girl_para_add()
{
    s32 x;
    u8 y;
    u8 val;
    Buf9 buf0;
    Buf11 buf1;
    Buf11 buf2;
    s32 z;
    s32 w;
    s32 sel;
    Row *p;

    buf0 = bunkaken_girl_para_add_tbl0;
    buf1 = bunkaken_girl_para_add_tbl1;
    buf2 = bunkaken_girl_para_add_tbl2;
    sel = ((u8 *)screen_w)[62] - 95;
    sel = (u8)sel;
    if (bunka_girl == 11) {
        screen_w[0].h[225] += 2;
        switch (sel) {
        case 0:
            val = buf0.t[0][bunka_bukatu];
            break;
        case 1:
            val = buf1.t[0][bunka_bukatu];
            break;
        case 2:
            val = buf2.t[0][bunka_bukatu];
            break;
        }
        if (val != 0) {
            val--;
        }
        screen_w[0].h[223] += val;
    } else if (bunka_girl < 11) {
        screen_w[bunka_girl].h[225] += 2;
        switch (sel) {
        case 0:
            val = buf0.t[bunka_girl][bunka_bukatu];
            break;
        case 1:
            val = buf1.t[bunka_girl][bunka_bukatu];
            break;
        case 2:
            val = buf2.t[bunka_girl][bunka_bukatu];
            break;
        }
        if (val != 0) {
            val--;
        }
        screen_w[bunka_girl].h[223] += val;
    }
    if (bunka_girl2 != 255) {
        screen_w[bunka_girl2].h[225] -= 10;
        goto L;
    L:
        screen_w[bunka_girl2].h[227] += 10;
    }
}
