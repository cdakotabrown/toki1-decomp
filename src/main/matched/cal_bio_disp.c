/* `@SymbolStart: 0x80068F6C` opts into the correction that */
/* 2. `@CompileFlags: -Wc,-pic1`.  The frame was 32 against retail's 48 -- */
/* Address: 0x80068F68 */
/* Size: 620 bytes (155 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x80068F6C */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x5c --offset2 0xac */
/* @PostProc: swap_instructions --offset1 0x108 --offset2 0x10c */
/* @PostProc: patch_insn_reg_field 0x40:rt:t6:t8 0x44:rt:t8:t7 0x48:rt:t8:t7 0x4c:rs:t6:t8 0x50:rs:t6:t8 0x58:rs:t6:t8 0x64:rs:t6:t8 0x70:rs:t6:t8 */
#include "types.h"

extern void biorhythm_curve_disp(void);
extern void dtd_on_tpage(s32, s32, s32, s32, s32);
extern void cal_base_show(void);

extern s16 cw97_tick;
extern s16 cw97_scalex;
extern s16 cw97_scaley;
extern u8 class_w[];

void system_moji_set(void) {
    s32 a0;
    s32 t1;
    s32 switchVal;
    s16 val;
    u8 *base;

    biorhythm_curve_disp();
    dtd_on_tpage(0, 0, 1, 2, 0);
    cal_base_show();

    t1 = cw97_tick;
    switchVal = (t1 >> 6) & 3;
    cw97_tick = (s16)(t1 + 1);

    switch (switchVal) {
    case 0:
        val = (s16)((t1 & 0x3F) << 6);
        cw97_scalex = val;
        cw97_scaley = val;
        break;
    case 1:
        cw97_scalex = 0x1000;
        cw97_scaley = 0x1000;
        break;
    case 2:
        val = (s16)((63 - (t1 & 0x3F)) << 6);
        cw97_scalex = val;
        cw97_scaley = val;
        break;
    case 3:
        cw97_scalex = 0;
        cw97_scaley = 0;
        break;
    }

    a0 = 0x62;
    do {
        base = class_w + (u32)a0 * 0x44;

        switch (switchVal) {
        case 0:
            val = (s16)((t1 & 0x3F) << 6);
            *(s16 *)(base + 0x1A) = val;
            *(s16 *)(base + 0x1C) = val;
            break;
        case 1:
            *(s16 *)(base + 0x1A) = 0x1000;
            *(s16 *)(base + 0x1C) = 0x1000;
            break;
        case 2:
            val = (s16)((63 - (t1 & 0x3F)) << 6);
            *(s16 *)(base + 0x1A) = val;
            *(s16 *)(base + 0x1C) = val;
            break;
        case 3:
            *(s16 *)(base + 0x1A) = 0;
            *(s16 *)(base + 0x1C) = 0;
            break;
        }

        switch (switchVal) {
        case 0:
            val = (s16)((t1 & 0x3F) << 6);
            *(s16 *)(base + 0x5E) = val;
            *(s16 *)(base + 0x60) = val;
            break;
        case 1:
            *(s16 *)(base + 0x5E) = 0x1000;
            *(s16 *)(base + 0x60) = 0x1000;
            break;
        case 2:
            val = (s16)((63 - (t1 & 0x3F)) << 6);
            *(s16 *)(base + 0x5E) = val;
            *(s16 *)(base + 0x60) = val;
            break;
        case 3:
            *(s16 *)(base + 0x5E) = 0;
            *(s16 *)(base + 0x60) = 0;
            break;
        }

        a0 += 2;
    } while (a0 != 0xA0);
}
