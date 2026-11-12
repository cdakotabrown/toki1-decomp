/* @SymbolStart: 0x8015709C */
/* @SymbolSize: 244 */
/* @PostProc: scheduler_normalizer --block-aware */
/* @PostProc: patch_insn_reg_field 0x60:rt:v1:t0 0x64:rs:v1:t0 0x64:rt:v1:t0 0x6c:rs:v1:t0 0x74:rs:a1:v1 0x74:rt:a1:v1 0x78:rt:a1:t4 0x7c:rt:v1:t8 0x80:rd:t0:t1 0x84:rs:a1:v1 0x84:rt:t0:t1 0x88:rt:t1:t2 0x90:rs:t1:t2 0x90:rt:t2:t3 0x94:rt:t2:t3 0x98:rs:v1:t4 0x98:rt:v1:t4 0xa0:rs:v1:t4 0xa4:rd:t3:t5 0xa8:rs:a1:v1 0xa8:rt:t3:t5 0xac:rt:t4:t6 0xb4:rs:t4:t6 0xb4:rt:t5:t7 0xb8:rt:t5:t7 0xbc:rs:v1:t8 0xbc:rt:v1:t8 0xc4:rs:v1:t8 0xc8:rd:t6:t9 0xcc:rs:a1:v1 0xcc:rt:t6:t9 0xd0:rt:t7:t0 0xd8:rs:t7:t0 0xd8:rt:t8:t1 0xe0:rt:t8:t1 */
/* Address: 0x801570A4 (real entry hoisted to 0x8015709C, cross-prologue) */
/* Size: 244 bytes (61 instructions incl. hoisted head) */
#include "types.h"

extern volatile u8 D_800B1746;
extern volatile u8 D_800B1AF6;
extern s16 D_80124344;
extern s16 D_800B0966;
extern u8 D_800B0860[];

extern void func_8004C250(s32);
extern void func_80011DFC(void);

void func_801570A4(void) {
    s16 sel = D_800B0966;

    if (sel < 30) goto body;
    if (sel >= 35) goto ret;
    if (D_800B1746 != 7) goto ret;
body:
    D_80124344 = 12;
    D_800B1AF6 = D_800B1AF6 + 15;
    *(s16 *)(D_800B0860 + D_800B1746 * 52 + 422) -= 1;
    *(s16 *)(D_800B0860 + D_800B1746 * 52 + 426) -= 1;
    *(s16 *)(D_800B0860 + D_800B1746 * 52 + 430) += 5;
    func_8004C250(52);
    return;
ret:
    func_80011DFC();
}
