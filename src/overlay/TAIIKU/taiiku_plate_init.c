/* @SymbolSize: 608 */
/* @CompileFlags: -cckr */
#include "types.h"

typedef union {
    s32 l;
    struct {
        u16 lo;
        s16 hi;
    } w;
} Fix;

typedef struct {
    u8 b0;
    u8 b1;
    u8 b2;
    u8 b3;
    u8 b4;
    u8 b5;
    u8 b6;
    u8 b7;
    u8 pad8[4];
    s32 w12;
    s32 w16;
    s16 h20;
    s16 h22;
    s16 h24;
    s16 h26;
    s16 h28;
    u8 pad1E[6];
    Fix x;
    Fix y;
    u8 pad2C[8];
    s32 *w52;
    s32 w56;
    u8 pad3C[7];
    u8 b67;
} Actor;

extern Actor class_w[];
extern s32 taiiku_spr_img_13;
extern s32 taiiku_spr_anim_13;
extern s32 taiiku_tama_plate_tpg;
extern u8 taiiku_tama_rank[];
extern s32 taiiku_tama_girl;
extern s32 taiiku_spr_aux[];

void game_reset();

void taiiku_plate_init()
{
    s32 i;

    for (i = 0; i < 8; i++) {
        class_w[96 + i].b3 = 164;
        class_w[96 + i].b2 = 64;
        class_w[96 + i].b5 = 128;
        class_w[96 + i].b1 = 3;
        class_w[96 + i].w56 = 0x01000000;
        class_w[96 + i].b7 = 128;
        class_w[96 + i].w12 = taiiku_spr_img_13;
        class_w[96 + i].w16 = taiiku_spr_anim_13;
        class_w[96 + i].w52 = &taiiku_tama_plate_tpg;
        class_w[96 + i].h20 = (*(s16 *)&taiiku_spr_aux[13]);
        class_w[96 + i].h26 = 4096;
        class_w[96 + i].h28 = 4096;
        class_w[96 + i].b6 = 2;
    }
    for (i = 0; i < 4; i++) {
        class_w[96 + i].x.w.hi = i * 64 - 120;
        class_w[96 + i].y.w.hi = -8;
        switch ((s32)taiiku_tama_rank[i]) {
        case 0:
            class_w[96 + i].h24 = taiiku_tama_girl * 7;
            break;
        case 1:
            class_w[96 + i].h24 = 77;
            class_w[96 + i].b67 = 28;
            break;
        case 2:
            class_w[96 + i].h24 = 77;
            class_w[96 + i].b67 = 29;
            break;
        case 3:
            class_w[96 + i].h24 = 77;
            break;
        default:
            game_reset();
            break;
        }
    }
    for (i = 0; i < 4; i++) {
        class_w[100 + i].x.w.hi = i * 64 - 120;
        class_w[100 + i].y.w.hi = -8;
        class_w[100 + i].h24 = 88;
        switch ((s32)taiiku_tama_rank[i]) {
        case 0:
            break;
        case 1:
            class_w[100 + i].b67 = 28;
            break;
        case 2:
            class_w[100 + i].b67 = 29;
            break;
        case 3:
            class_w[100 + i].b67 = 30;
            break;
        }
    }
}
