/* Address: 0x8004FEA8 */
/* Size: 228 bytes (57 instructions) */
/* Region: Game */
/* @CompileFlags: -cckr */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=8:v1,11:a3,26:a0 IDO52_UGEN_TEMP_REG=6:t6 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x1c */
/* @PostProc: patch_insn_reg_field 0x3c:rd:v0:t2 0x40:rd:t2:v0 0x48:rs:v0:t2 0x4c:rs:v0:t2 0x5c:rs:v0:t2 0x6c:rs:v0:t2 0x70:rs:v0:t2 0x8c:rs:v0:t2 0x98:rs:t2:v0 0xa8:rs:v0:t2 0xa8:rt:v0:t2 */
#include "types.h"

typedef struct {
    s16 x;
    s16 y;
    u8 w;
    u8 h;
    u8 hit;
    u8 pad;
} Row;

typedef struct {
    u8 pad0[3984];
    Row row[16][2];
    u8 cnt[3];
    s8 sel[2];
    u8 chosen[2];
} Save;

extern Save screen_w;
extern s32 pad_data;

s32 menu_check_1(slot, x, y)
    s32 slot;
    s32 x;
    s32 y;
{
    s32 hits;
    s32 last;
    s32 i;
    s32 dx;
    s32 dy;
    Save *sv;

    x = (s16)x;
    y = (s16)y;
    hits = 0;
    last = -1;
    sv = &screen_w;
    for (i = 0; i < sv->cnt[slot]; i++) {
        dx = x - screen_w.row[i][slot].x;
        screen_w.row[i][slot].hit = 0;
        if ((dx - (int)screen_w.row[i][slot].w) * dx < 0) {
            dy = y - screen_w.row[i][slot].y;
            if ((dy - (int)screen_w.row[i][slot].h) * dy < 0) {
                screen_w.row[i][slot].hit = 1;
                hits++;
                last = i;
            }
        }
    }
    if (hits == 0) {
        screen_w.sel[slot] = -1;
    }
    if (!(pad_data & 4) && last != -1) {
        screen_w.chosen[slot] = last;
    }
    return last;
}
