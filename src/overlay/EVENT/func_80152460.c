/* @PostProc: scheduler_normalizer */
/* @PostProc: patch_insn_reg_field 0x18:rs:a0:v1 0x30:rt:a0:v1 0x58:rt:v1:t8 0x60:rt:t8:t9 0x64:rs:t8:t9 0x64:rt:t8:t9 0x68:rs:v1:t8 0x68:rt:t9:t0 0x6c:rs:t9:t0 0x6c:rt:t9:t0 0x70:rs:v1:t8 0x70:rt:t1:t2 0x74:rs:t1:t2 0x74:rt:t1:t2 0x78:rd:t0:t1 0x78:rs:t8:t9 0x78:rt:t9:t0 0x7c:rd:t2:t3 0x7c:rs:t0:t1 0x7c:rt:t1:t2 0x88:rt:t2:t3 */
/* Address: 0x80152460 */
/* Size: 188 bytes (47 instructions) */
#include "types.h"

extern s32 func_8002328C(s32 id);
extern void func_80011DFC(void);

extern u8 D_800B1746;
extern s32 D_800EECBC;
extern u8 D_8012531C;
extern s16 D_80094730;

void func_80152460(void)
{
    s32 v;
    s32 lo;

    v = func_8002328C(D_800B1746);

    if (D_800EECBC != 0) {
        goto bail;
    }
    if (D_800B1746 != 0 && D_800B1746 != 7) {
        goto bail;
    }
    if (D_8012531C != 0) {
        goto bail;
    }

    lo = v & 0x7F;
    D_80094730 = (s16)(D_80094730 + ((u32)lo >= 2) + ((u32)lo >= 3));
    goto single;

bail:
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
    func_80011DFC();
single:
    func_80011DFC();
}
