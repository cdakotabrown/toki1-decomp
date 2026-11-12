/* func_8014010C -- GYOZI overlay, Address: 0x8014010C, Size: 168 bytes (42 insns) */
/* @CompileEnv: IDO52_CONST_INREG_VALUES=64,-64 */
/* @SymbolSize: 168 */
/* @PostProc: patch_stack_complete --old 80 --new 88 */
/* @PostProc: remap_stack_slots --map '48:56,50:58,52:60,54:62,56:64,58:66,60:68,62:70,64:72,66:74,68:76,70:78,72:80,74:82,76:84,78:86' */
/* @PostProc: scheduler_normalizer --block-aware */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x74 */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x80 */
/* @PostProc: patch_insn_reg_field 0x18:rt:v0:t2 0x1c:rt:t0:t3 0x20:rt:v1:t4 0x38:rt:t2:t6 0x3c:rt:t1:t5 0x4c:rt:v0:t2 0x50:rt:v0:t2 0x54:rt:v0:t2 0x58:rt:v1:t3 0x5c:rt:v1:t3 0x60:rt:v1:t3 0x64:rt:t0:t4 0x68:rt:t0:t4 0x6c:rt:t0:t4 0x70:rt:t1:t5 0x74:rt:t2:t6 0x78:rt:t0:t4 0x7c:rt:v1:t3 0x80:rt:v0:t2 0x98:rt:t3:t7 0xa4:rt:t3:t7 */
#include "types.h"

extern void func_8005BD20(s32 a0, s32 a1, s16 *a2, s16 *a3, s16 *e, u16 *f);
extern void func_8004DE1C(void);

extern s32 D_8012E6C0;

void func_8014010C(void)
{
    s16 rect[4];
    s16 neg[4];
    s16 pos[4];
    u16 pos2[4];
    s32 cneg;
    s32 cpos;
    s32 cpos2;

    rect[0] = -150;
    rect[1] = -72;
    rect[2] = 6;
    rect[3] = 84;
    cneg = -64;
    cpos = 64;
    cpos2 = 64;
    neg[0] = neg[1] = neg[2] = cneg;
    pos[0] = pos[1] = pos[2] = cpos;
    pos2[0] = pos2[1] = pos2[2] = cpos2;
    pos2[3] = cpos2;
    pos[3] = cpos;
    neg[3] = cneg;

    func_8005BD20(0, 4, rect, neg, pos, pos2);
    D_8012E6C0 = 1;
    func_8004DE1C();
}
