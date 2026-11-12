/* Address: 0x80085098 */
/* Size: 580 bytes (145 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* `@CompileEnv: IDO52_NO_FINDINDUCT_STRIDE=1` restores retail's form. */
/* @CompileFlags: -Wo,-loopunroll,0 -Wc,-pic1 */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=64:8 IDO52_COPYPROP_SITE=lod:23:deny,lod:24:deny,lod:25:deny,lod:26:deny IDO52_AS1_KEEP_ANDI=occ:0 IDO52_FORCE_WEB_COLOR=94:a0 IDO52_AS1_TIE_REJECT=348,356,555,564 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"
#include "tokimeki/sprite.h"

extern void load_image_rect(s32 x, s32 y, s32 w, s32 h, s32 data);

extern s32 date_sprite_on;
extern s32 wink2_stop;
extern s32 bustup_mode;
extern s32 etc_rec_tbl0;
extern s32 etc_rec_tbl1;
extern s32 etc_rec_tbl2;
extern s32 etc_rec_tbl3;
extern s32 etc_rec_tbl4;
extern s32 etc_rec_tbl5;
extern u8 screen_w[];
extern u8 Sprites[];

void date_sprite_init(s32 a0, s32 a1, s32 a2, s32 a3, s32 arg4, s32 arg5) {
    s16 v1;
    s32 s;
    u16 h;
    u8 *sprite_base;
    u8 *data_ptr;

    date_sprite_on = 0;
    wink2_stop = 0;
    bustup_mode = 0;

    load_image_rect(320, 128, 64, 128, a0);
    load_image_rect(384, 128, 64, 128, a1);
    load_image_rect(448, 128, 64, 128, a2);
    load_image_rect(512, 128, 64, 128, a3);
    load_image_rect(576, 128, 64, 128, arg4);
    load_image_rect(640, 128, 64, 128, arg5);

    etc_rec_tbl0 = 1;
    etc_rec_tbl1 = 1;
    etc_rec_tbl2 = 1;
    etc_rec_tbl3 = 1;
    etc_rec_tbl4 = 1;
    etc_rec_tbl5 = 1;

    sprite_base = Sprites;
    data_ptr = screen_w;

    v1 = 58;
    while (v1 < 64) {
        s32 offset = v1 * 36;
        u8 *sprite = sprite_base + offset;
        s32 rem = (v1 - 58) % 3;

        *(u32 *)(sprite + 0) = 0xC1000000;
        *(u16 *)(sprite + 8) = 128;
        *(u16 *)(sprite + 10) = 128;
        *(s16 *)(sprite + 12) = v1 - 53;
        *(u8 *)(sprite + 14) = 0;
        *(u8 *)(sprite + 15) = 128;
        *(s16 *)(sprite + 16) = 0;
        *(s16 *)(sprite + 18) = 497;
        *(u8 *)(sprite + 20) = 0;
        *(u8 *)(sprite + 21) = 0;
        *(u8 *)(sprite + 22) = 0;
        *(s16 *)(sprite + 24) = 0;
        *(s16 *)(sprite + 26) = 0;
        *(s16 *)(sprite + 28) = 4096;
        *(s16 *)(sprite + 30) = 4096;
        *(u32 *)(sprite + 32) = 0;

        *(s16 *)(sprite + 4) = (rem << 7) - 160;
        *(s16 *)(sprite + 6) = ((v1 - 58) / 3 << 7) - 120;

        data_ptr[v1 + 4261] = 11;

        v1++;
    }

    (*(u16 *)&((SPRITE_REC *)Sprites)[60].w) = 64;

    s = 32;
    h = s;
    (*(u16 *)&((SPRITE_REC *)Sprites)[62].h) = h;
    (*(u16 *)&((SPRITE_REC *)Sprites)[61].h) = h;
    (*(u16 *)&((SPRITE_REC *)Sprites)[63].w) = 64;
    ((SPRITE_REC *)Sprites)[63].h = s;
}
