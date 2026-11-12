/* @SymbolStart: 0x80154764 */
/* @SymbolSize: 228 */
/* @PostProc: patch_insn_reg_field 0x70:rs:v0:v1 0x70:rt:v1:v0 0x98:rs:v0:v1 0x98:rt:v1:v0 */
/* Address: 0x8015476C */
/* Size: 228 bytes (57 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern s32 D_800EECBC;

extern u8  D_800B1746;
extern u8  D_800B0B1F;
extern s16 D_80094730;

extern void func_80011DFC(void);
extern void func_8004A7F4(void);
extern void func_8004A7C4(void);

void func_8015476C(void) {
    s32 t6_v = D_800EECBC;

    s32 v0;

    if (t6_v != 2) {
        goto def;
    }
    v0 = D_800B1746;
    if ((s32)v0 - 2 == 0) {
        goto a;
    }
    if (v0 != 3) {
        goto b;
    }
a:
    func_80011DFC();
    goto end;
b:
    if (v0 != 8) {
        goto c;
    }
    func_8004A7F4();
    goto end;
c:
    if (v0 != 5) {
        goto d;
    }
    if (D_800B0B1F != 0) {
        goto d;
    }
    func_80011DFC();
    goto end;
d:
    if (v0 != 5) {
        goto e;
    }
    if (D_800B0B1F == 0) {
        goto e;
    }
    D_80094730 = (s16)(D_80094730 + 1);
    func_8004A7C4();
    goto end;
e:
    func_8004A7C4();
    goto end;
def:
    func_80011DFC();
end:
    ;
}
