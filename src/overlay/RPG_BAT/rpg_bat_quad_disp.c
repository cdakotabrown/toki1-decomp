/* @SymbolSize: 528 */
/* rpg_bat_quad_disp - RPG_BAT overlay, Address: 0x80154220, Size: 524 bytes (131 insns) */
#include "types.h"

extern volatile s32 activeBuff;
extern u8 ot_buff0[];

extern s32 prim_alloc();
extern s32 GetTPage();
extern s32 GetClut();
extern s32 AddPrim();
extern s32 SetSemiTrans();
extern s32 SetShadeTex();
extern s32 SetPolyFT4();

void rpg_bat_quad_disp(s32 *s1, s32 *s2, s32 arg2, s32 arg3, s32 arg5)
{
    u8 *obj;
    s32 v1;

    obj = (u8 *)prim_alloc(40, activeBuff);
    SetPolyFT4(obj);
    SetShadeTex(obj, 0);
    if (arg3 != 0) {
        SetSemiTrans(obj, 1);
    } else {
        SetSemiTrans(obj, 0);
    }
    obj[4] = (u8)arg2;
    obj[5] = (u8)arg2;
    obj[6] = (u8)arg2;
    if (arg3 != 0) {
        *(s16 *)(obj + 14) = (s16)GetClut(0, 503);
    } else {
        *(s16 *)(obj + 14) = (s16)GetClut(0, 501);
    }
    *(s16 *)(obj + 22) = (s16)GetTPage(1, arg3 + 4, (arg5 << 6) + 320, 256);

    v1 = -activeBuff * 240 + 240;
    *(s16 *)(obj + 8) = (s16)s1[0];
    *(s16 *)(obj + 10) = (s16)(s1[1] + v1);
    *(s16 *)(obj + 16) = (s16)(s1[2] + 1);
    *(s16 *)(obj + 18) = (s16)(s1[3] + v1);
    *(s16 *)(obj + 24) = (s16)s1[4];
    *(s16 *)(obj + 26) = (s16)(s1[5] + v1 + 1);
    *(s16 *)(obj + 32) = (s16)(s1[6] + 1);
    *(s16 *)(obj + 34) = (s16)(s1[7] + v1 + 1);
    obj[12] = (u8)s2[0];
    obj[13] = (u8)s2[1];
    obj[20] = (u8)(s2[2] + 1);
    obj[21] = (u8)s2[3];
    obj[28] = (u8)s2[4];
    obj[29] = (u8)(s2[5] + 1);
    obj[36] = (u8)(s2[6] + 1);
    obj[37] = (u8)(s2[7] + 1);
    AddPrim((s32)&ot_buff0[(activeBuff << 10) + 32], obj);
}
