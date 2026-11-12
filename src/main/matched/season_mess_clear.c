/* Address: 0x8006D01C */
/* Size: 140 bytes (35 instructions) -- `jr ra; nop` at 0x8006D0A0/A4; season_flag_set follows */
/* @SymbolSize: 140 */
/* Region: Game */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x48 */
/* @PostProc: swap_instructions --offset1 0x4c --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x54 --offset2 0x58 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x64 */
/* @PostProc: swap_instructions --offset1 0x64 --offset2 0x68 */
/* @PostProc: swap_instructions --offset1 0x68 --offset2 0x6c */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x80 */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x78 */
/* @CompileFlags: -Wc,-pic1 -Wo,-loopunroll,0 */
/* @CompileEnv: IDO52_CONST_NOINREG_VALUES=-2 IDO52_FORCE_WEB_COLOR=18:v1,21:v0 */
#include "types.h"

extern volatile u8 girl0_flag;
extern volatile u8 girl1_flag;
extern volatile u8 girl2_flag;
extern u8 girl_blk[];

void season_mess_clear(void) {
    u8 *p;
    u8 *end;

    girl0_flag &= ~1;
    girl1_flag &= ~1;
    girl2_flag &= ~1;

    p = girl_blk;
    end = &girl_blk[448];
    do {
        p[456] &= ~1;
        p[512] &= ~1;
        p[568] &= ~1;
        p[624] &= ~1;
        p += 224;
    } while (p != end);
}
