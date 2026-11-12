/* @SymbolStart: 0x801361DC */
/* @SymbolSize: 136 */
/* func_801361EC -- GYOZI overlay, Address: 0x801361EC */
#include "types.h"

extern s32 system_config_disp(s32 a0);
extern void func_80090960(s32 a0, s32 a1);
extern void func_8004DE1C(void);

extern struct {
    u8 flags;
    u8 pad[0x820];
    u8 girl;
} D_800F5AAE;
extern u8 D_800F647A;
extern s32 D_8012E698;

void func_801361EC(void)
{
    u8 r;

    D_800F5AAE.flags |= 8;
    r = system_config_disp(D_800F5AAE.girl) & 0x7F;
    if (r >= 2U) {
        D_800F647A += 6;
    } else {
        func_80090960(13, D_8012E698);
        D_800F5AAE.flags |= 4;
        func_8004DE1C();
    }
}
