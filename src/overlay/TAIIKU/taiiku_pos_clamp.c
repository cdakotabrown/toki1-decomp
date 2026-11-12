/* @SymbolSize: 120 */
/* @CompileEnv: IDO52_AS1_NO_LA_PULL=all */
/* @PostProc: patch_insn_reg_field 0x10:rt:v1:v0 0x14:rs:v1:v0 0x3c:rt:v1:v0 0x44:rt:v1:v0 0x48:rt:v1:v0 0x4c:rs:v1:v0 0x74:rt:v1:v0 0x7c:rt:v1:v0 */
/* Address: 0x8013B144 */
/* Size: 120 bytes (30 insns) */
#include "types.h"

extern s16 cal_cw_x;
extern s16 cal_cw_y;

void taiiku_pos_clamp(void)
{
    s32 v;

    v = cal_cw_x;
    if (v >= 161) {
        v = 160;
    }
    cal_cw_x = v;
    if (v < -160) {
        v = -160;
        cal_cw_x = v;
    }
    v = cal_cw_y;
    if (v >= 49) {
        v = 48;
    }
    cal_cw_y = v;
    if (v < -24) {
        v = -24;
        cal_cw_y = v;
    }
}
