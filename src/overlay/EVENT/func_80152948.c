/* @SymbolStart: 0x80152940 */
/* @SymbolSize: 360 */
/* @PostProc: scheduler_normalizer --block-aware */
/* @PostProc: patch_insn_reg_field 0x64:rs:v0:v1 0x64:rt:v1:v0 0xfc:rs:v0:v1 0xfc:rt:v1:v0 0x138:rs:v0:v1 0x138:rt:v1:v0 */
/* Address: 0x80152948 */
/* Size: 360 bytes (90 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern s32 D_800EECBC;

extern u8  D_800B1746;
extern s16 D_80094730;
extern u8  D_800B1AF6;
extern u8  D_800B0E64;

extern void func_80011DFC(void);

void func_80152948(void) {
    s32 t6_v = D_800EECBC;

    s32 v0;

    if (t6_v != 1) {
        goto blockB;
    }
    v0 = D_800B1746;
    D_80094730 = 9;
    if (v0 != 7) {
        goto a1;
    }
    D_80094730 = 10;
    goto a_tail;
a1:
    if (v0 == 2) {
        goto a2;
    }
    if (9 != v0) {
        goto a3;
    }
a2:
    D_80094730 = 8;
    goto a_tail;
a3:
    if (v0 == 4) {
        goto a4;
    }
    if (v0 == 8) {
        goto a4;
    }
    if (v0 == 5) {
        goto a4;
    }
    if (v0 != 10) {
        goto a_tail;
    }
a4:
    D_80094730 = 7;
a_tail:
    D_800B1AF6 = (u8)(D_800B1AF6 + 17);
    goto end;

blockB:
    v0 = D_800B1746;
    if (v0 == 4) {
        goto b1;
    }
    if (v0 == 8) {
        goto b1;
    }
    if (v0 == 5) {
        goto b1;
    }
    if (v0 != 10) {
        goto b2;
    }
b1:
    D_800B1AF6 = (u8)(D_800B1AF6 + 5);
    goto end;
b2:
    if (9 != v0) {
        goto b3;
    }
    if (D_800B0E64 != 0) {
        goto b3;
    }
    D_80094730 = (s16)(D_80094730 + 1);
    D_800B1AF6 = (u8)(D_800B1AF6 + 5);
    goto end;
b3:
    if (9 != v0) {
        goto end;
    }
    if (D_800B0E64 == 0) {
        goto end;
    }
    D_800B1AF6 = (u8)(D_800B1AF6 + 17);
end:
    func_80011DFC();
}
