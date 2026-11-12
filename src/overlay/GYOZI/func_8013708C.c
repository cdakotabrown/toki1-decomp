/* @SymbolStart: 0x8013707C */
/* @SymbolSize: 844 */
/* @RodataBase: 0x80145A48 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=159:v1 IDO52_AS1_XBB_SKIP_CAND=0,1 */
/* func_8013708C -- GYOZI overlay, Address: 0x8013708C, Size: 828 bytes */
#include "types.h"

extern void func_8004DDD8(void);
extern void func_8008F618(u8);
extern s32 system_config_disp(u8);
extern u32 func_8005EBE0(u8);
extern s32 func_8004DE1C(void);

extern u8 D_800F62CF;
extern s32 D_801470F0;
extern s32 D_801470F8;
extern s32 D_801470FC;
extern s32 D_80147104;
extern s32 D_80147108;
extern s32 D_8014710C;
extern s32 D_80147110;
extern s32 D_80147114;
extern s32 D_80147118;
extern s32 D_8014711C;
extern s32 D_80147120;
extern s32 D_80147124;
extern s32 D_8014712C;
extern s32 D_80147130;
extern s32 D_80147138;
extern s32 D_8014713C;
extern s32 D_80147140;
extern s32 D_80147144;
extern s32 D_80147148;
extern s32 D_8014714C;
extern s32 D_80147150;
extern s32 D_80147154;
extern s32 D_80147158;
extern s32 D_80147160;
extern s32 D_80147164;
extern s32 D_8014716C;
extern s32 D_80147170;
extern s32 D_80147174;
extern s32 D_80147178;
extern s32 D_8014717C;
extern s32 D_80147180;
extern s32 D_80147184;
extern s32 D_80147188;
extern s32 D_8014718C;
extern s32 D_80147190;
extern s32 D_80147194;
extern s16 D_80147198;
extern s16 D_80147220;
extern u8 D_80147224[];
extern s16 D_8014723C;
extern u8 D_80147240;

s32 func_8013708C(void)
{
    u32 sel;
    u8 tmp;

    if (D_80147220 == D_8014723C) {
        func_8004DDD8();
        return 0;
    }

    D_800F62CF = D_80147224[D_80147220 * 2];
    func_8008F618(D_800F62CF);
    tmp = (u8)(system_config_disp(D_800F62CF) & 0x7F);
    D_80147198 = (s16)(func_8005EBE0(D_800F62CF) >= 70);
    sel = D_800F62CF;

    if (sel == 9) {
        if (D_80147240 == 1) {
            D_80147198 = 3;
        }
    }
    if (sel == 0) {
        if (D_80147240 == 1) {
            if (tmp < 2U) {
                D_80147198 = 3;
            }
        }
    }

    switch (sel) {
    case 0:
        D_8014718C = D_801470F8;
        D_80147190 = D_8014712C;
        D_80147194 = D_80147160;
        break;
    case 1:
        D_8014718C = D_8014710C;
        D_80147190 = D_80147140;
        D_80147194 = D_80147174;
        break;
    case 2:
        D_8014718C = D_801470FC;
        D_80147190 = D_80147130;
        D_80147194 = D_80147164;
        break;
    case 3:
        D_8014718C = D_80147104;
        D_80147190 = D_80147138;
        D_80147194 = D_8014716C;
        break;
    case 4:
        D_8014718C = D_8014711C;
        D_80147190 = D_80147150;
        D_80147194 = D_80147184;
        break;
    case 5:
        D_8014718C = D_80147110;
        D_80147190 = D_80147144;
        D_80147194 = D_80147178;
        break;
    case 6:
        D_8014718C = D_80147114;
        D_80147190 = D_80147148;
        D_80147194 = D_8014717C;
        break;
    case 7:
        D_8014718C = D_80147108;
        D_80147190 = D_8014713C;
        D_80147194 = D_80147170;
        break;
    case 8:
        D_8014718C = D_801470F0;
        D_80147190 = D_80147124;
        D_80147194 = D_80147158;
        break;
    case 9:
        D_8014718C = D_80147118;
        D_80147190 = D_8014714C;
        D_80147194 = D_80147180;
        break;
    case 10:
        D_8014718C = D_80147120;
        D_80147190 = D_80147154;
        D_80147194 = D_80147188;
        break;
    }

    return func_8004DE1C();
}
