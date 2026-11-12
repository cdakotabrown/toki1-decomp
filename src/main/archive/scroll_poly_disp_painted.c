/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script cs_func_8008612C.py */
/* @CompileEnv: IDO52_NO_FINDINDUCT_STRIDE=1 */
/* Function: scroll_poly_disp */
/* Address: 0x8008612C */
/* Size: 760 bytes (190 instructions) */
/* Region: Library (0x80086000+) */
#include "types.h"

extern s32 activeBuff;
extern u8 screen_bri;
extern s16 screen_w[];
extern u8 ot_buff0;

extern void *prim_alloc(s32, s32);
extern void SetPolyFT4(void *);
extern void SetSemiTrans(void *, s32);
extern void SetShadeTex(void *, s32);
extern s16 GetTPage(s32, s32, s32, s32);
extern s16 GetClut(s32, s32);
extern void AddPrim(s32, void *);

void scroll_poly_disp(s32 arg0, s32 arg1, s16 arg2, s16 arg3, s16 arg4) {
    u8 *prim;
    s32 i;
    s32 xacc;
    s32 aacc;
    s32 arg0x2;
    s32 arg1x4;
    s32 ux;
    u32 sum;

    arg0x2 = arg0 << 1;
    arg1x4 = arg1 << 2;
    xacc = 0;
    i = 0;
    aacc = 0;

    do {
        prim = (u8 *)prim_alloc(40, activeBuff);
        SetPolyFT4(prim);
        SetSemiTrans(prim, 0);
        SetShadeTex(prim, 0);

        *(prim + 4) = screen_bri;
        *(prim + 5) = screen_bri;
        *(prim + 6) = screen_bri;

        ux = (arg0x2 + ((i % 2) << 6)) & 0x7F;

        *(prim + 20) = (u8)(ux + 64);
        *(prim + 36) = (u8)(ux + 64);
        *(prim + 12) = (u8)ux;
        *(prim + 28) = (u8)ux;
        *(prim + 37) = (u8)(arg2 + arg3);
        *(prim + 29) = (u8)(arg2 + arg3);
        *(prim + 21) = (u8)arg2;
        *(prim + 13) = (u8)arg2;

        *(s16 *)(prim + 8) = *(s16 *)((u8 *)screen_w + (activeBuff << 1) + 20) + xacc - 160;
        *(s16 *)(prim + 10) = *(s16 *)((u8 *)screen_w + (activeBuff << 1) + 24) + arg2 - 120;
        *(s16 *)(prim + 16) = *(s16 *)((u8 *)screen_w + (activeBuff << 1) + 20) + xacc - 96;
        *(s16 *)(prim + 18) = *(s16 *)((u8 *)screen_w + (activeBuff << 1) + 24) + arg2 - 120;
        *(s16 *)(prim + 24) = *(s16 *)((u8 *)screen_w + (activeBuff << 1) + 20) + xacc - 160;
        *(s16 *)(prim + 26) = *(s16 *)((u8 *)screen_w + (activeBuff << 1) + 24) + arg2 + arg3 - 120;
        *(s16 *)(prim + 32) = *(s16 *)((u8 *)screen_w + (activeBuff << 1) + 20) + xacc - 96;
        *(s16 *)(prim + 34) = *(s16 *)((u8 *)screen_w + (activeBuff << 1) + 24) + arg2 + arg3 - 120;

        sum = (u32)(aacc + arg0);
        *(s16 *)(prim + 22) = GetTPage(1, 0, (((sum >> 6) << 6) % (u32)arg4) + 320, 0);
        *(s16 *)(prim + 14) = GetClut(0, 496);

        AddPrim((s32)(&ot_buff0 + (activeBuff << 10) + arg1x4), prim);

        i++;
        aacc += 32;
        xacc += 64;
    } while (i != 5);
}
