/* Address: 0x80132C58, Size: 100 bytes (25 insns) */
/* @SymbolSize: 100 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v1:v0 0x14:rs:v1:v0 0x14:rt:v1:v0 0x1c:rs:v1:v0 */
#include "types.h"

extern s32 en_nichi_phase_cnt;
extern volatile s16 cw153_frame;

void en_nichi_phase_count(void)
{
    s32 v0;

    v0 = en_nichi_phase_cnt + 1;
    if (v0 >= 54) {
        v0 = 0;
    }
    if (v0 == 0) {
        goto phase0;
    }
    en_nichi_phase_cnt = v0;
    if (v0 != 27) {
        return;
    }
    goto phase27;
phase0:
    en_nichi_phase_cnt = v0;
    cw153_frame = 32;
    return;
phase27:
    cw153_frame = 33;
}
