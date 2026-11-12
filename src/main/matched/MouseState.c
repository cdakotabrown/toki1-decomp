/* Address: 0x80059A90 */
/* Size: 144 bytes (36 instructions) */
/* Region: Game (0x80041000-0x80085FFF), IDO 5.2, leaf */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:6 IDO52_CONST_INREG_VALUES=0 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: swap_branch_operands --offsets 0x20 */
/* @PostProc: swap_instructions --offset1 0x28 --offset2 0x3C */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x8C */
/* @PostProc: patch_insn_reg_field --patch 0x10:rt:v1:a2 --patch 0x14:rd:a2:v1 --patch 0x18:rs:v1:a2 --patch 0x40:rs:v1:a2 --patch 0x8C:rs:a2:v1 */
#include "types.h"

extern s8 *mouse_rec_tbl[];

s32 MouseState(s32 idx, s8 *out) {
    s32 r = 0;

    if (mouse_rec_tbl[idx][0] != 0) {
        out[3] = 0;
        out[0] = 0;
        out[1] = 0;
        out[2] = 0;
        return -1;
    }
    out[0] = mouse_rec_tbl[idx][4];
    out[1] = mouse_rec_tbl[idx][5];
    out[2] = mouse_rec_tbl[idx][3];
    out[3] = mouse_rec_tbl[idx][1];
    return r;
}
