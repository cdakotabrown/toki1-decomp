/* Function: basyo_show (file basyo_show) */
/* Address: 0x8006646C */
/* Size: 1776 bytes (444 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* Likely Symbol: class_clear_3d (from o.bin) */
/* @SymbolStart: 0x80066464 */
/* @SymbolSize: 1784 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=8:t5 IDO52_UGEN_TEMP_REG=27:t6,29:t8,28:t7 IDO52_AS1_TIE_REJECT=809,1178,1547,311,318,325,1592,1599,1606 */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x39c --offset2 0x3a0 */
/* @PostProc: swap_instructions --offset1 0x3a0 --offset2 0x3a4 */
/* @PostProc: swap_instructions --offset1 0x3a4 --offset2 0x3a8 */
/* @PostProc: swap_instructions --offset1 0x3a8 --offset2 0x3ac */
/* @PostProc: swap_instructions --offset1 0x1c4 --offset2 0x1cc */
/* @PostProc: swap_instructions --offset1 0x22c --offset2 0x234 */
/* @PostProc: patch_insn_reg_field 0x1ac:rt:v1:v0 0x1b4:rs:v1:v0 0x1b8:rs:v1:v0 0x1c4:rs:v1:v0 0x1cc:rs:v1:v0 0x220:rt:v1:v0 0x22c:rs:v1:v0 0x234:rs:v1:v0 */
#include "types.h"

typedef union {
    s32 l;
    struct { u16 lo; s16 hi; } w;
} Fix;

typedef struct {
    u8 pad0[36];
    Fix x;
    Fix y;
    u8 pad2c[24];
} Slot;

extern Slot class_w[];
extern volatile s16 mes_base_y;
extern s16 basyo_init_tbl0;
extern s32 pad_data;
extern u8 cw3_disp;

extern s16 *get_k_work();

void basyo_show()
{
    s32 n;
    s32 i;
    s16 *p;

    if (!(cw3_disp & 0x80)) {
        return;
    }
    p = get_k_work(0);
    n = mes_base_y;
    if (n < 0) {
        n = (s16)(n + 1);
        for (i = 0; i < basyo_init_tbl0 + 2; i++) {
            class_w[3 + i].x.w.hi = (basyo_init_tbl0 - i) * 14 + n - 135;
            class_w[3 + i].y.w.hi = n - 81;
        }
        class_w[3].x.w.hi -= 2;
        class_w[basyo_init_tbl0 + 4].x.w.hi += 2;
        mes_base_y = n;
        _sprite_set_box_shade(n - 144, n - 90, basyo_init_tbl0 * 14 + 8, 20, 2, 0x30E00, 0);
        p[0]++;
        p[1]++;
    } else if (n < 120) {
        mes_base_y = n + 1;
        _sprite_set_box_shade(-144, -90, basyo_init_tbl0 * 14 + 8, 20, 2, 0x30E00, 0);
        p[0] = -142;
        p[1] = -88;
    } else if (n < 184) {
        n = (s16)(n + 1);
        for (i = 0; i < basyo_init_tbl0 + 2; i++) {
            class_w[3 + i].x.w.hi = (basyo_init_tbl0 - i) * 14 + (120 - n) - 135;
            class_w[3 + i].y.w.hi = (120 - n) - 81;
        }
        class_w[3].x.w.hi -= 2;
        class_w[basyo_init_tbl0 + 4].x.w.hi += 2;
        mes_base_y = n;
        _sprite_set_box_shade((120 - n) - 144, (120 - n) - 90, basyo_init_tbl0 * 14 + 8, 20, 2, 0x30E00, 0);
        p[0]--;
        p[1]--;
    } else if (pad_data & 0x10) {
        for (i = 0; i < basyo_init_tbl0 + 2; i++) {
            class_w[3 + i].x.w.hi = (basyo_init_tbl0 - i) * 14 - 135;
            class_w[3 + i].y.w.hi = -81;
        }
        class_w[3].x.w.hi -= 2;
        class_w[basyo_init_tbl0 + 4].x.w.hi += 2;
        _sprite_set_box_shade(-144, -90, basyo_init_tbl0 * 14 + 8, 20, 2, 0x30E00, 0);
        p[0] = -142;
        p[1] = -88;
    } else {
        for (i = 0; i < basyo_init_tbl0 + 2; i++) {
            class_w[3 + i].x.w.hi = (basyo_init_tbl0 - i) * 14 - 199;
            class_w[3 + i].y.w.hi = -145;
        }
        class_w[3].x.w.hi -= 2;
        class_w[basyo_init_tbl0 + 4].x.w.hi += 2;
        _sprite_set_box_shade(-208, -154, basyo_init_tbl0 * 14 + 8, 20, 2, 0x30E00, 0);
        p[0] = -206;
        p[1] = -152;
    }
}
