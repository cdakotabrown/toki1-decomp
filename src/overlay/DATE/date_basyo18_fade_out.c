/* @SymbolStart: 0x8013FA5C */
/* @SymbolSize: 668 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=92:a1 IDO52_COPYPROP_VAR=-24 IDO52_COPYPROP_SITE=lod:18:deny,lod:19:deny,lod:20:deny,lod:21:deny,lod:22:deny,lod:23:deny */
/* Function: date_basyo18_fade_out */
/* Overlay: DATE */
/* Address: 0x8013FAA8 */
/* Size: 592 bytes (148 instructions) */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 palette_upload_enqueue();
extern s32 device_cmd_send();
extern s32 class_clear_f();
extern s32 yazirushi_off();

extern volatile u8 bustup2_cw_bright;
extern volatile u8 bustup2_cw_disp;
extern u8 class_w[];
extern u8 Sprites[];
extern s32 date_effect_on;
extern s32 bustup_on;
extern s32 bustup_check_init_tbl3;
extern s32 screen_w[];
extern u8 update_anim_state_tbl4;
extern u8 date_effect_bri;

void date_basyo18_fade_out(void)
{
    s16 i;
    s16 j;
    s16 k;
    u32 girl;
    u8 c;
    s32 *p;
    u32 bit;
    u32 w0;
    u32 w1;
    u32 mode;
    u8 c0;
    u8 c1;

    mode = (date_effect_bri - 4) & 0xFF;
    Sprites[21] = c0 = mode;
    Sprites[20] = c0;
    Sprites[57] = c1 = mode;
    Sprites[56] = c1;
    date_effect_bri = mode;
    Sprites[22] = c0;
    Sprites[58] = c1;
    bustup2_cw_bright = (u8)mode; for (i = 0; i < 4; i++) {
        class_w[i * 68 + 6535] = bustup2_cw_bright;
    }
    girl = bustup2_cw_bright;
    c = (u8)girl;
    for (j = 58; j < 64; j++) {
        Sprites[j * 36 + 22] = c;
        Sprites[j * 36 + 21] = c;
        Sprites[j * 36 + 20] = c;
    }
    bit = 0x80000000;
    if (girl < 17) {
        date_effect_on = 0;
    }
    bustup2_cw_bright = (u8)girl;
    if (girl != 0) {
        return;
    }
    date_effect_on = 0;
    bustup_on = 0;
    for (i = 0; i < 10; i++) {
        class_w[i * 68 + 6531] =
            (u8)(class_w[i * 68 + 6531] & 0x7F);
    }
    bustup2_cw_disp = (u8)(bustup2_cw_disp & 0xFF7F);
    Sprites[21] = w0 = 128;
    Sprites[20] = w0;
    Sprites[57] = w1 = 128;
    Sprites[56] = w1;
    *(s32 *)Sprites |= bit;
    *(s32 *)(Sprites + 36) |= bit;
    Sprites[22] = w0;
    Sprites[58] = w1;
    for (j = 58; j < 64; j++) {
        *(s32 *)(Sprites + j * 36) |= bit;
    }
    palette_upload_enqueue(bustup_check_init_tbl3, 16, 1, 2, 0);
    for (k = 11; k < 16; k++) {
        p = &screen_w[k];
        p[1162] = 119;
        p[1194] = 119;
    }
    for (k = 21; k < 26; k++) {
        p = &screen_w[k];
        p[1162] = 119;
        p[1194] = 119;
    }
    device_cmd_send(517);
    class_clear_f();
    yazirushi_off();
    update_anim_state_tbl4 = 15;
    sub_sub_step_up();
}
