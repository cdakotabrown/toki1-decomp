/* @SymbolStart: 0x80133E9C */
/* @SymbolSize: 216 */
/* @PostProc: scheduler_normalizer */
/* func_80133EB0 -- GYOZI overlay, Address: 0x80133EB0, Size: 196 bytes (49 insns) */
#include "types.h"

extern void func_800BCE10(s32 a0, s32 a1);
extern void func_8008FB00(void);
extern void func_8004DE1C(void);

extern s8 D_800D9328[];
extern s16 D_800F5496;
extern s16 D_800F549A;
extern s16 D_800F549E;
extern u8 D_800F62CF;
extern s32 D_8012E66C;
extern s16 D_80145EB4;
extern u8 D_801460C0;
extern s8 D_801460C4[];

void func_80133EB0(void)
{
    s32 value;

    value = D_8012E66C;
    D_801460C0 = (u8)value;
    D_80145EB4 = (s16)(D_80145EB4 + value);
    func_800BCE10((s32)D_800D9328, (s32)&D_801460C4[value * 9]);
    if (D_8012E66C == 0) {
        D_800F549A += 10;
        D_800F549E += 10;
        D_800F62CF = 14;
    } else if (D_8012E66C == 1) {
        D_800F5496 += 25;
        D_800F62CF = 14;
    }
    func_8008FB00();
    func_8004DE1C();
}
