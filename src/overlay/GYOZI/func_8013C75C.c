/* @SymbolStart: 0x8013C724 */
/* @SymbolSize: 180 */
/* @PostProc: patch_insn_reg_field 0x74:rd:t5:t4 0x7c:rd:t6:t5 0x80:rt:t6:t5 0xa0:rt:t4:t3 0xa8:rs:t5:t4 0xa8:rt:t4:t3 */
/* func_8013C75C -- GYOZI overlay, Address: 0x8013C75C, Size: 108 bytes (27 insns) */
#include "types.h"

extern void func_8004DE1C(void);

extern s32 D_8014808C;
extern s32 D_801480C0;
extern s32 D_801480F4;
extern s32 D_801480FC;
extern s32 D_80148100;
extern s32 D_80148104;
extern s16 D_80148108;
extern s16 D_8014810C;

extern u8 D_800D9388[];

extern u8 D_800F53A0[];

void func_8013C75C(void)
{
    s16 i;

    D_801480FC = D_8014808C;
    D_80148100 = D_801480C0;
    D_80148104 = D_801480F4;
    D_80148108 = 0;
    D_8014810C = 0;

    for (i = 0; i < 11; i++) {
        D_800D9388[i] =
            (u8)(((u32)(D_800F53A0[i * 56 + 441] & 1) < 1) ^ 1);
    }

    func_8004DE1C();
}
