/* en_nichi_bar_disp - EN_NICHI overlay, Address: 0x8013819C, Size: 448 bytes (112 insns) */
#include "types.h"

extern s32 prim_alloc();
extern void SetPolyG4();
extern void SetSemiTrans();
extern void SetShadeTex();
extern void AddPrim();

extern volatile s32 activeBuff;
extern s16 screen_w[];
extern u8 ot_buff0[];
extern u16 en_nichi_bar_len;

void en_nichi_bar_disp(void)
{
    s32 s0;
    s32 a1;
    s32 a2;

    a1 = activeBuff;
    s0 = prim_alloc(36, a1);
    SetPolyG4(s0);
    SetSemiTrans(s0, 1);
    SetShadeTex(s0, 0);

    a2 = en_nichi_bar_len;

    *(u8 *)(s0 + 6) = 255;
    *(u8 *)(s0 + 4) = 31;
    *(u8 *)(s0 + 5) = 31;
    *(u8 *)(s0 + 12) = (u8)(a2 * 2 + 31);
    *(u8 *)(s0 + 14) = 255;
    *(u8 *)(s0 + 13) = 31;
    *(u8 *)(s0 + 22) = 255;
    *(u8 *)(s0 + 20) = 31;
    *(u8 *)(s0 + 21) = 31;
    *(u8 *)(s0 + 28) = (u8)(a2 * 2 + 31);
    *(u8 *)(s0 + 30) = 255;
    *(u8 *)(s0 + 29) = 31;

    *(s16 *)(s0 + 8) = screen_w[activeBuff + 10] + 32;
    *(s16 *)(s0 + 10) = screen_w[activeBuff + 12] + 80;

    *(s16 *)(s0 + 16) = screen_w[activeBuff + 10] + a2 + 32;
    *(s16 *)(s0 + 18) = screen_w[activeBuff + 12] + 80;

    *(s16 *)(s0 + 24) = screen_w[activeBuff + 10] + 32;
    *(s16 *)(s0 + 26) = screen_w[activeBuff + 12] + 96;

    *(s16 *)(s0 + 32) = screen_w[activeBuff + 10] + a2 + 32;
    *(s16 *)(s0 + 34) = screen_w[activeBuff + 12] + 96;

    AddPrim(activeBuff * 1024 + 12 + (s32)ot_buff0, s0);
}
