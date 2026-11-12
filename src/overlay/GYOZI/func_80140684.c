/* @SymbolStart: 0x8014067C */
/* @SymbolSize: 276 */
/* func_80140684 -- GYOZI overlay, Address: 0x8014067C, Size: 276 bytes (69 insns) */
#include "types.h"

extern void func_8004DEAC(s32 a0);
extern void func_8004DE1C(void);

extern u8 D_801486E4;
extern s32 D_8012E6BC;
extern s32 D_8012E660;
extern s32 D_8012E664;
extern u8 D_800F62CF;
extern u8 D_800F5902;
extern u8 D_800F592F;
extern u8 D_800F593D;

s32 func_80140684(void)
{
    u32 mode = D_801486E4;

    D_8012E6BC = 0;

    if (mode == 0) {
        D_800F62CF = 0;
        D_8012E664 = 0;
        D_8012E660 = 7;
        func_8004DEAC(6);
        D_800F5902 = D_800F5902 + 1;
        return 0;
    } else if (mode == 7) {
        D_800F62CF = 7;
        D_8012E664 = 7;
        D_8012E660 = 7;
        func_8004DEAC(7);
        D_800F592F = D_800F592F + 1;
        return 0;
    } else if (mode == 9) {
        D_800F62CF = 9;
        D_8012E664 = 9;
        D_8012E660 = 7;
        func_8004DEAC(8);
        D_800F593D = D_800F593D + 1;
        return 0;
    } else {
        func_8004DE1C();
    }
}
