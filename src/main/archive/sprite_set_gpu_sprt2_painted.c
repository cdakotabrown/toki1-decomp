/* Function: sprite_set_gpu_sprt2 */
/* Address: 0x8004A004 */
/* Size: 848 bytes (212 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* Status: UNMATCHABLE — see config/c_unmatchable.txt */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_8004A004.py */
#include "types.h"

extern u8 *prim_alloc(s32, s32);
extern void SetPolyGT4(u8 *);
extern void SetSemiTrans(u8 *, s32);
extern s16 GetClut(s32, s32);
extern void AddPrim(u8 *, u8 *);

extern s32 activeBuff;
extern s16 screen_w[];
extern u8 ot_buff0[];

void sprite_set_gpu_sprt2(u8 *a2, s32 arg1) {
    u8 *s0;
    u32 v1;
    s16 *base;

    (void)&arg1;

    if (*(s32 *)a2 < 0) {
        return;
    }

    s0 = prim_alloc(52, activeBuff);
    SetPolyGT4(s0);

    if ((*(s32 *)a2 << 1) < 0) {
        SetSemiTrans(s0, 1);
    } else {
        SetSemiTrans(s0, 0);
    }

    s0[4] = a2[20];
    s0[5] = a2[21];
    s0[6] = a2[22];

    v1 = ((u32)*(s32 *)a2 & 0x03000000) >> 17;

    s0[16] = a2[20];
    s0[17] = a2[21];
    s0[18] = a2[22];

    base = screen_w;

    s0[28] = a2[20];
    s0[29] = a2[21];
    s0[30] = a2[22];
    s0[40] = a2[20];
    s0[41] = a2[21];
    s0[42] = a2[22];

    *(s16 *)(s0 + 8) = *(s16 *)((u8 *)base + activeBuff * 2 + 20) + *(s16 *)(a2 + 4) - *(s16 *)(a2 + 24);
    *(s16 *)(s0 + 10) = *(s16 *)((u8 *)base + activeBuff * 2 + 24) + *(s16 *)(a2 + 6) - *(s16 *)(a2 + 26);

    *(s16 *)(s0 + 20) = *(s16 *)((u8 *)base + activeBuff * 2 + 20) + *(s16 *)(a2 + 4) - *(s16 *)(a2 + 24) + *(u16 *)(a2 + 8);
    *(s16 *)(s0 + 22) = *(s16 *)((u8 *)base + activeBuff * 2 + 24) + *(s16 *)(a2 + 6) - *(s16 *)(a2 + 26);

    *(s16 *)(s0 + 32) = *(s16 *)((u8 *)base + activeBuff * 2 + 20) + *(s16 *)(a2 + 4) - *(s16 *)(a2 + 24);
    *(s16 *)(s0 + 34) = *(s16 *)((u8 *)base + activeBuff * 2 + 24) + *(s16 *)(a2 + 6) - *(s16 *)(a2 + 26) + *(u16 *)(a2 + 10);

    *(s16 *)(s0 + 44) = *(s16 *)((u8 *)base + activeBuff * 2 + 20) + *(s16 *)(a2 + 4) - *(s16 *)(a2 + 24) + *(u16 *)(a2 + 8);
    *(s16 *)(s0 + 46) = *(s16 *)((u8 *)base + activeBuff * 2 + 24) + *(s16 *)(a2 + 6) - *(s16 *)(a2 + 26) + *(u16 *)(a2 + 10);

    s0[12] = a2[14];
    s0[13] = a2[15];
    s0[24] = a2[14] + *(u16 *)(a2 + 8);
    s0[25] = a2[15];
    s0[36] = a2[14];
    s0[37] = a2[15] + *(u16 *)(a2 + 10);
    s0[48] = a2[14] + *(u16 *)(a2 + 8);
    s0[49] = a2[15] + *(u16 *)(a2 + 10);

    *(u16 *)(s0 + 26) = *(u16 *)(a2 + 12) | v1;

    *(s16 *)(s0 + 14) = GetClut(*(s16 *)(a2 + 16), *(s16 *)(a2 + 18));

    AddPrim((u8 *)((activeBuff << 10) + (arg1 << 2) + (s32)ot_buff0), s0);
}
