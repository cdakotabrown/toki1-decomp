/* @SymbolStart: 0x8013781C */
/* Address: 0x80137824, Size: 112 bytes (28 instructions) */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @SymbolSize: 120 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: patch_insn_reg_field 0x24:rt:a1:v1 0x38:rs:a1:v1 0x38:rt:a1:v1 0x40:rt:v1:t8 0x48:rd:t8:t9 0x48:rs:v1:t8 0x4c:rt:a1:v1 0x50:rt:t8:t9 0x78:rt:t9:t1 0x7c:rt:t9:t1 */
#include "types.h"

extern void goto_sub_sub_step();

extern u8 cw159_disp;
extern u8 cw96_active[];
extern u8 cw98_active;
extern s16 cw159_scale_x;
extern s16 cw159_scale_y;

void option_subgame_exit(void)
{
    s32 hidden_t6;
    u8 *v0;
    u8 *v1;
    s32 a0;
    u8 t8;

    hidden_t6 = cw159_disp;

    cw159_disp = (u8)(hidden_t6 & 0xFF7F);

    v1 = &cw98_active;
    v0 = &cw96_active[0];
    a0 = -129;

    do {
        t8 = v0[1023];
        v0 += 68;
        v0[955] = t8 & a0;
    } while (v0 != v1);

    goto_sub_sub_step(0);
    cw159_disp = 0;
    cw159_scale_y = 0x1000;
    cw159_scale_x = 0x1000;
}
