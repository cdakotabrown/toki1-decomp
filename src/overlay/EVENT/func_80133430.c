/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x40 */
/* @PostProc: patch_insn_reg_field 0x44:rt:t9:t0 0x4c:rt:t9:t0 0x54:rt:t0:t1 0x58:rt:t0:t1 0x5c:rt:t1:t2 0x60:rs:t1:t2 0x60:rt:t1:t2 0x68:rt:t1:t2 0x6c:rt:t2:t3 0x70:rs:t2:t3 0x70:rt:t2:t3 0x78:rt:t2:t3 0x7c:rt:t3:t4 0x80:rs:t3:t4 0x80:rt:t3:t4 0x88:rt:t3:t4 0x8c:rt:t4:t5 0x90:rs:t4:t5 0x90:rt:t4:t5 0x98:rt:t4:t5 0x9c:rt:t5:t6 0xa0:rs:t5:t6 0xa0:rt:t5:t6 0xa8:rt:t5:t6 0xac:rt:t6:t7 0xb0:rs:t6:t7 0xb0:rt:t6:t7 0xbc:rt:t6:t7 */
/* Address: 0x80133430 */
/* Size: 176 bytes (44 insns) */
#include "types.h"

extern s32 D_80120714;
extern s32 D_80120718;
extern s32 D_8012071C;
extern s16 D_80120720;
extern s32 D_80120724;
extern s32 D_80120728;
extern s32 D_8012072C;
extern s32 D_80120730;
extern s32 D_80120734;
extern s32 D_80120738;
extern s32 D_8012073C;
extern s16 D_801CE0A8;

void func_80133430(void) {
    D_80120714 = 0x801CE080;
    D_80120718 = 0x801CE084;
    D_8012071C = 0x801CE094;
    D_80120720 = D_801CE0A8;
    D_80120724 = 0x801B0000;
    D_80120728 = 0x801B2000;
    D_8012072C = 0x801B6000;
    D_80120730 = 0x801BA000;
    D_80120734 = 0x801BE000;
    D_80120738 = 0x801C2000;
    D_8012073C = 0x801C6000;
}
