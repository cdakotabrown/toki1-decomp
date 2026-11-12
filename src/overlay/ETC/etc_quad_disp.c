/* @PostProc: patch_stack_complete --old 104 --new 120 */
/* @PostProc: remap_stack_slots --map '96:108' */
/* @PostProc: rename_register_chain --chain s1:s3 --chain s3:s2 --chain s2:s1 --exclude-offsets 0x14,0x18,0x1c,0x20,0x24,0x28,0x2c,0x30,0x34,0x224,0x228,0x22c,0x230,0x234,0x238,0x23c,0x240,0x244,0x248 */
/* @PostProc: rename_register_chain --chain s4:s6 --chain s6:s5 --chain s5:s4 --exclude-offsets 0x14,0x18,0x1c,0x20,0x24,0x28,0x2c,0x30,0x34,0x224,0x228,0x22c,0x230,0x234,0x238,0x23c,0x240,0x244,0x248 */
/* @PostProc: swap_instructions --offset1 0x14 --offset2 0x24 */
/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x54 */
/* @PostProc: swap_instructions --offset1 0x54 --offset2 0x50 */
/* @PostProc: swap_instructions --offset1 0x50 --offset2 0x4c */
/* @PostProc: swap_instructions --offset1 0x4c --offset2 0x48 */
/* @PostProc: swap_instructions --offset1 0x48 --offset2 0x44 */
/* @PostProc: swap_instructions --offset1 0x44 --offset2 0x40 */
/* Address: 0x8013D31C */
/* Size: 580 bytes (145 instructions) */
/* `if (s3 != 224) return;`) with `@PostProc: pad_trailing_nop --count 127` */
#include "types.h"

extern s32 prim_alloc();
extern s32 GetTPage();
extern void AddPrim();
extern void SetSemiTrans();
extern void SetShadeTex();
extern void SetPolyFT4();

extern s16 screen_w[];
extern u8 ot_buff0[];
extern s32 activeBuff;

void etc_quad_disp(void)
{
    u8 *arr[2];
    u8 *o;
    u8 **pp;
    s32 x;
    s32 y;
    s32 c;
    s32 fp;
    s32 u;

    pp = arr;
    x = 0;
    y = 160;
    c = 160;
    fp = 640;
    u = 0;
    do {
        o = (u8 *)prim_alloc(40, activeBuff);
        *pp = o;
        SetPolyFT4(o);
        SetSemiTrans(*pp, 0);
        SetShadeTex(*pp, 0);
        *(s16 *)(*pp + 22) = GetTPage(2, 0, fp, 0);
        *(s16 *)(*pp + 8) = x;
        *(s16 *)(*pp + 10) = screen_w[activeBuff + 12] - 120;
        *(s16 *)(*pp + 16) = y;
        *(s16 *)(*pp + 18) = screen_w[activeBuff + 12] - 120;
        *(s16 *)(*pp + 24) = x;
        *(s16 *)(*pp + 26) = screen_w[activeBuff + 12] + 120;
        *(s16 *)(*pp + 32) = y;
        *(s16 *)(*pp + 34) = screen_w[activeBuff + 12] + 120;
        *(u8 *)(*pp + 12) = u;
        *(u8 *)(*pp + 13) = 0;
        *(u8 *)(*pp + 20) = c;
        *(u8 *)(*pp + 21) = 0;
        *(u8 *)(*pp + 28) = u;
        *(u8 *)(*pp + 29) = 240;
        *(u8 *)(*pp + 36) = c;
        *(u8 *)(*pp + 37) = 240;
        *(u8 *)(*pp + 4) = 128;
        *(u8 *)(*pp + 5) = 128;
        *(u8 *)(*pp + 6) = 128;
        AddPrim(&ot_buff0[activeBuff * 1024 + 56], *pp);
        c += 32;
        pp += 1;
        fp += 128;
        x += 160;
        y += 160;
        u += 32;
    } while (c != 224);
}
