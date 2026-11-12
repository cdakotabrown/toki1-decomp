/* @PostProc: swap_instructions --offset1 0x40 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x3c --offset2 0x40 */
/* @PostProc: patch_insn_reg_field 0x44:rt:t9:t0 0x4c:rt:t9:t0 0x54:rt:t0:t1 0x58:rt:t0:t1 0x5c:rt:t1:t2 0x60:rs:t1:t2 0x60:rt:t1:t2 0x68:rt:t1:t2 0x6c:rt:t2:t3 0x70:rs:t2:t3 0x70:rt:t2:t3 0x78:rt:t2:t3 0x7c:rt:t3:t4 0x80:rs:t3:t4 0x80:rt:t3:t4 0x88:rt:t3:t4 0x8c:rt:t4:t5 0x90:rs:t4:t5 0x90:rt:t4:t5 0x98:rt:t4:t5 0x9c:rt:t5:t6 0xa0:rs:t5:t6 0xa0:rt:t5:t6 0xa8:rt:t5:t6 0xac:rt:t6:t7 0xb0:rs:t6:t7 0xb0:rt:t6:t7 0xbc:rt:t6:t7 */
/* Size: 176 bytes (44 insns). Named-extern destinations, integer- */
#include "types.h"

extern s32 D_80122228;
extern s32 D_8012222C;
extern s32 D_80122230;
extern s16 D_80122234;
extern s32 D_80122238;
extern s32 D_8012223C;
extern s32 D_80122240;
extern s32 D_80122244;
extern s32 D_80122248;
extern s32 D_8012224C;
extern s32 D_80122250;
extern s16 D_801D2230;

void func_8013D300(void) {
    D_80122228 = 0x801D21D0;
    D_8012222C = 0x801D21D8;
    D_80122230 = 0x801D2218;
    D_80122234 = D_801D2230;
    D_80122238 = 0x801B0000;
    D_8012223C = 0x801B2000;
    D_80122240 = 0x801B6000;
    D_80122244 = 0x801BA000;
    D_80122248 = 0x801BE000;
    D_8012224C = 0x801C2000;
    D_80122250 = 0x801C6000;
}
