/* Address: 0x8013CA0C (cross-prologue head absorbed), Size: 332 bytes (83 insns) */
/* @SymbolStart: 0x8013CA0C */
/* @SymbolSize: 332 */
/* @PostProc: patch_insn_reg_field 0x24:rt:v1:t7 0x28:rt:a1:a0 0x30:rs:a1:a0 0x30:rt:a1:a0 0x38:rt:a0:v1 0x3c:rs:v1:t7 0x3c:rt:t7:t8 0x40:rs:t7:t8 0x44:rs:a0:v1 0x48:rt:t8:t9 0x4c:rt:t1:t2 0x50:rs:t8:t9 0x50:rt:t9:t0 0x54:rt:t9:t0 0x58:rt:t0:t1 0x60:rs:t0:t1 0x6c:rt:t1:t2 0x70:rs:a0:v1 0x70:rt:t2:t3 0x78:rs:t2:t3 0x78:rt:t3:t4 0x7c:rs:t3:t4 0x84:rt:t4:t5 0x8c:rs:t4:t5 0x8c:rt:t5:t6 0x90:rs:t5:t6 0x90:rt:t6:t7 0x94:rs:t6:t7 0x9c:rt:t5:t6 0xa4:rt:t7:t8 0xa8:rt:v1:a1 0xac:rs:t7:t8 0xac:rt:t8:t9 0xb0:rs:t8:t9 0xb8:rs:v1:a1 0xc0:rs:a1:a0 0xc4:rs:a1:a0 0xc8:rt:v1:a1 0xd0:rs:v1:a1 0xd8:rs:v1:a1 0xe0:rt:t9:a0 0xe8:rs:t9:a0 */
#include "types.h"

extern u8 *tt_game_wk;
extern u8 *tt_step_wk;
extern u8 *tt_scroll_wk;
extern u8 *tt_text_buf;

extern void tt_widget_y_set(void);
extern void tt_bitmask_code_set(void);
extern s16 tt_pad_config_icon_get(s32 idx, u8 *dst);

void tt_pad_config_cursor(void)
{
    u8 *r;
    u8 *s;
    u8 *a;
    u8 *g;
    u8 *p;
    s32 h;

    r = tt_game_wk;
    h = *(u16 *)(r + 20);
    a = tt_step_wk;
    g = tt_scroll_wk;
    s = r + 16;
    if (h & 0x1000) {
        g[10] = g[10] - 1;
        if ((s8)g[10] < 0) {
            g[10] = 2;
        }
    } else if (*(u16 *)(s + 4) & 0x4000) {
        g[10] = g[10] + 1;
        if ((u32)g[10] >= 3U) {
            g[10] = 0;
        }
    }
    if ((*(u16 *)(s + 4) & 0x820) && g[10] == 2) {
        *(s16 *)(a + 4) = 0;
        *(s16 *)(a + 2) = 0;
    }
    if (g[10] == 0 || g[10] == 1) {
        if (*(u16 *)(s + 4) & 0xFF) {
            tt_bitmask_code_set();
        }
    }
    p = tt_text_buf + 320;
    *(s16 *)(p + 14) = tt_pad_config_icon_get(r[68], p + 2);
    p += 20;
    *(s16 *)(p + 14) = tt_pad_config_icon_get(r[69], p + 2);
    tt_widget_y_set();
}
