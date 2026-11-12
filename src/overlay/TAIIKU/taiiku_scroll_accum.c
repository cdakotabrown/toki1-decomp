/* @SymbolSize: 104 */
/* @PostProc: patch_insn_reg_field 0x18:rt:t7:v0 0x24:rt:t8:t7 0x28:rs:t7:v0 0x28:rt:t7:v0 0x2c:rs:t8:t7 0x2c:rt:t8:t7 0x30:rt:a0:v1 0x34:rs:t7:v0 0x34:rt:t8:t7 0x38:rt:a0:v1 0x44:rs:a0:v1 */
/* Address: 0x80146AC8 */
/* Size: 104 bytes (26 insns) */
#include "types.h"

extern s32 taiiku_spoon_pl_w0C;
extern s32 taiiku_spoon_pl_spd;
extern s32 taiiku_spoon_pl_acc;
extern volatile s32 taiiku_spoon_vmax;

void taiiku_scroll_accum(void)
{
    s32 acc;
    s32 lim;

    if ((u32)taiiku_spoon_pl_w0C < 11U) {
        return;
    }
    acc = taiiku_spoon_pl_spd + taiiku_spoon_pl_acc;
    if (acc < -65536) {
        acc = -65536;
    }
    taiiku_spoon_pl_spd = acc;
    lim = taiiku_spoon_vmax;
    if (lim < acc) {
        taiiku_spoon_pl_spd = lim;
    }
    taiiku_spoon_pl_w0C = 0;
}
