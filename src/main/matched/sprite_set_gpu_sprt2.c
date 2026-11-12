/* Address: 0x8004A004 */
/* Size: 848 bytes (212 instructions) */
/* Region: Game (0x80041000-0x80086000) */
/* Compiler: IDO 5.2 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern s16 screen_w[];
extern s32 activeBuff;
extern u8 ot_buff0[];

extern s32 prim_alloc(s32 size, s32 bank);
extern void SetPolyGT4(s32 obj);
extern void SetSemiTrans(s32 obj, s32 mode);
extern s16 GetClut(s32 x, s32 y);
extern void AddPrim(u8 *ot, s32 obj);

void sprite_set_gpu_sprt2(u8 *src, s32 arg1)
{
    s32 obj;
    u32 semi;

    if ((*(s32 *)src & 0x80000000) != 0) {
        return;
    }

    obj = prim_alloc(52, activeBuff);
    SetPolyGT4(obj);

    if ((*(s32 *)src << 1) < 0) {
        SetSemiTrans(obj, 1);
    } else {
        SetSemiTrans(obj, 0);
    }

    semi = (*(u32 *)src & 0x03000000) >> 17;

    *(u8 *)(obj + 4) = src[20];
    *(u8 *)(obj + 5) = src[21];
    *(u8 *)(obj + 6) = src[22];
    *(u8 *)(obj + 16) = src[20];
    *(u8 *)(obj + 17) = src[21];
    *(u8 *)(obj + 18) = src[22];
    *(u8 *)(obj + 28) = src[20];
    *(u8 *)(obj + 29) = src[21];
    *(u8 *)(obj + 30) = src[22];
    *(u8 *)(obj + 40) = src[20];
    *(u8 *)(obj + 41) = src[21];
    *(u8 *)(obj + 42) = src[22];

    *(s16 *)(obj + 8) = screen_w[activeBuff + 10] + *(s16 *)(src + 4)
                        - *(s16 *)(src + 24);
    *(s16 *)(obj + 10) = screen_w[activeBuff + 12] + *(s16 *)(src + 6)
                         - *(s16 *)(src + 26);
    *(s16 *)(obj + 20) = *(s16 *)(src + 4) - *(s16 *)(src + 24) + *(u16 *)(src + 8) + screen_w[activeBuff + 10];
    *(s16 *)(obj + 22) = screen_w[activeBuff + 12] + *(s16 *)(src + 6)
                         - *(s16 *)(src + 26);
    *(s16 *)(obj + 32) = screen_w[activeBuff + 10] + *(s16 *)(src + 4)
                         - *(s16 *)(src + 24);
    *(s16 *)(obj + 34) = *(s16 *)(src + 6) - *(s16 *)(src + 26) + *(u16 *)(src + 10) + screen_w[activeBuff + 12];
    *(s16 *)(obj + 44) = *(s16 *)(src + 4) - *(s16 *)(src + 24) + *(u16 *)(src + 8) + screen_w[activeBuff + 10];
    *(s16 *)(obj + 46) = *(s16 *)(src + 6) - *(s16 *)(src + 26) + *(u16 *)(src + 10) + screen_w[activeBuff + 12];

    *(u8 *)(obj + 12) = src[14];
    *(u8 *)(obj + 13) = src[15];
    *(u8 *)(obj + 24) = (u8)(*(u16 *)(src + 8) + src[14]);
    *(u8 *)(obj + 25) = src[15];
    *(u8 *)(obj + 36) = src[14];
    *(u8 *)(obj + 37) = (u8)(*(u16 *)(src + 10) + src[15]);
    *(u8 *)(obj + 48) = (u8)(*(u16 *)(src + 8) + src[14]);
    *(u8 *)(obj + 49) = (u8)(*(u16 *)(src + 10) + src[15]);

    *(u16 *)(obj + 26) = (u16)(*(u16 *)(src + 12) | semi);

    *(s16 *)(obj + 14) = GetClut(*(s16 *)(src + 16), *(s16 *)(src + 18));

    AddPrim(&ot_buff0[(activeBuff << 10) + arg1 * 4], obj);
}
