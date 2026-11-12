/* @SymbolStart: 0x8013EAA8 */
/* @SymbolSize: 112 */
/* @RodataBase: 0x8015B024 */
/* @PostProc: patch_insn_reg_field 0x2c:rt:v0:t7 0x34:rs:v0:t7 0x3c:rd:t8:t7 0x3c:rt:v0:t7 0x44:rt:t8:t7 0x48:rt:t8:t7 0x50:rs:t8:t7 */
/* Function: date_basyo17_fade_up */
/* Overlay: DATE */
/* Address: 0x8013EAA8 (cross-prologue head) / 0x8013EAB0 (body) */
/* Size: 112 bytes (28 instructions) */
#include "types.h"

extern u8 date_pattern;

extern s32 sub_sub_step_up(void);
extern s32 fade_up_main(void);

#define now_girl (*(volatile u8 *)0x800EF6BF)

s32 date_basyo17_fade_up(void) {
    u8 idx;
    if (date_pattern == 1) {
        idx = now_girl;
        switch (idx) {
            case 0: case 3: case 4: case 5: case 8: case 9:
                sub_sub_step_up();
                return 0;
            default:
                break;
        }
    }
    return fade_up_main();
}
