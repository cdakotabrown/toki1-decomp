/* @SymbolStart: 0x80139cb4 */
/* @SymbolSize: 116 */
/* @PostProc: patch_insn_reg_field 0x30:rt:t9:t0 0x34:rt:t1:t2 0x38:rs:t9:t0 0x38:rt:t0:t1 0x3c:rt:t0:t1 0x48:rt:t1:t2 */
#include "types.h"

extern void func_800BCE10(void *a, void *b);
extern void func_8004DE1C(void);

extern u8 D_800F62CF;
extern u8 D_800F53A0[];
extern s16 D_801474A8;
extern s16 D_801474AC;
extern char D_800D92A0[];
extern char D_80145B90[];

void func_80139CE8(void)
{
    D_800F53A0[D_800F62CF * 56 + 440] |= 4;
    D_801474A8 = 11;
    D_801474AC = 0;
    func_800BCE10(D_800D92A0, D_80145B90);
    func_8004DE1C();
}
