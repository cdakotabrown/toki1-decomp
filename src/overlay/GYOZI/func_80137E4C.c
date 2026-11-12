/* @CompileEnv: IDO52_UGEN_TREG_ORDER=t7,t8,t9,t0,t1,t2,t3,t4,t5,t6 */
/* @RodataBase: 0x80145A80 */
/* @PostProc: patch_insn_reg_field 0x20:rt:t7:v0 0x24:rs:t7:v0 0x24:rt:t7:v0 0x2c:rs:t7:v0 0x2c:rt:t8:t7 0x30:rs:t8:t7 */
/* Address: 0x80137E4C */
/* Size: 696 bytes (174 instructions) */
#include "types.h"

extern void func_80137524(void);
extern void func_8004DE1C(void);

extern volatile u8 D_800F5AAE;
extern volatile u8 D_800F62CF;

extern s32 D_801462BC;
extern s32 D_801462C0;
extern s32 D_801462C8;
extern s32 D_801462CC;
extern s32 D_801462D0;
extern s32 D_801462D4;
extern s32 D_801462D8;
extern s32 D_801462DC;
extern s32 D_801462E0;
extern s32 D_801462E4;
extern volatile s32 D_801462EC;

extern s32 D_801462F0;
extern s32 D_801462F4;
extern s32 D_801462FC;
extern s32 D_80146300;
extern s32 D_80146304;
extern s32 D_80146308;
extern s32 D_8014630C;
extern s32 D_80146310;
extern s32 D_80146314;
extern s32 D_80146318;
extern volatile s32 D_80146320;

extern s32 D_80146324;
extern s32 D_80146328;
extern s32 D_80146330;
extern s32 D_80146334;
extern s32 D_80146338;
extern s32 D_8014633C;
extern s32 D_80146340;
extern s32 D_80146344;
extern s32 D_80146348;
extern s32 D_8014634C;
extern volatile s32 D_80146354;

extern s32 D_8014718C;
extern s32 D_80147190;
extern s32 D_80147194;
extern s16 D_80147198;
extern s16 D_8014719C;

void func_80137E4C(void)
{
    func_80137524();

    D_800F5AAE = (D_800F5AAE & ~8) | 4;

    switch (D_800F62CF) {
    case 0:
        D_8014718C = D_801462C0;
        D_80147190 = D_801462F4;
        D_80147194 = D_80146328;
        break;
    case 1:
        D_8014718C = D_801462D4;
        D_80147190 = D_80146308;
        D_80147194 = D_8014633C;
        break;
    case 2:
        D_8014718C = D_801462C8;
        D_80147190 = D_801462FC;
        D_80147194 = D_80146330;
        break;
    case 3:
        D_8014718C = D_801462CC;
        D_80147190 = D_80146300;
        D_80147194 = D_80146334;
        break;
    case 4:
        D_8014718C = D_801462E4;
        D_80147190 = D_80146318;
        D_80147194 = D_8014634C;
        break;
    case 5:
        D_8014718C = D_801462D8;
        D_80147190 = D_8014630C;
        D_80147194 = D_80146340;
        break;
    case 6:
        D_8014718C = D_801462DC;
        D_80147190 = D_80146310;
        D_80147194 = D_80146344;
        break;
    case 7:
        D_8014718C = D_801462D0;
        D_80147190 = D_80146304;
        D_80147194 = D_80146338;
        break;
    case 8:
        D_8014718C = D_801462BC;
        D_80147190 = D_801462F0;
        D_80147194 = D_80146324;
        break;
    case 9:
        D_8014718C = D_801462E0;
        D_80147190 = D_80146314;
        D_80147194 = D_80146348;
        break;
    default:
        D_8014718C = D_801462EC;
        D_80147190 = D_80146320;
        D_80147194 = D_80146354;
        break;
    }

    D_80147198 = 1;
    D_8014719C = 0;
    func_8004DE1C();
}
