/* Function: _sprite_set_light_effect1 */
/* Address: 0x800495F4 */
/* Size: 1124 bytes (281 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: patch_insn_reg_field --patch 0x250:rt:v1:a1 --patch 0x258:rt:a1:v1 --patch 0x25c:rt:v1:a1 --patch 0x268:rt:a1:v1 */
#include "types.h"

extern s32 frame_counter;
extern s32 activeBuff;
extern s16 screen_w[];
extern u8 ot_buff0[];

extern s32 rsin(s32);
extern s32 abs(s32);
extern void *prim_alloc(s32, s32);
extern void SetPolyG3(void *);
extern void SetSemiTrans(void *, s32);
extern void AddPrim(void *, void *);

void _sprite_set_light_effect1(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, u32 arg7, s32 arg8) {
    s32 unused;
    void *obj;
    s32 sinR, sinG, sinB;
    s32 sp28;
    s32 v1;
    u8 *a2;
    s16 *base;
    s32 alpha1, alpha2, alpha3;

    sinR = (rsin(frame_counter * 0xFF0) << 5) / 4096 + ((arg7 >> 16) & 0xFF);
    sinG = (rsin(frame_counter * 0xFF0) << 5) / 4096 + ((arg7 >> 8) & 0xFF);
    sinB = (rsin(frame_counter * 0xFF0) << 5) / 4096 + (arg7 & 0xFF);
    sp28 = abs(rsin((frame_counter + arg8) * 0xFF0));
    obj = prim_alloc(28, activeBuff);
    SetPolyG3(obj);
    SetSemiTrans(obj, 1);
    a2 = (u8 *)obj;
    base = screen_w;

    a2[4] = sinR * 3 / 5;
    a2[5] = sinG * 3 / 5;
    a2[6] = sinB * 3 / 5;
    alpha1 = sinR * 0 / 5;
    alpha2 = sinG * 0 / 5;
    alpha3 = sinB * 0 / 5;
    a2[14] = alpha3;
    a2[13] = alpha2;
    a2[12] = alpha1;
    a2[22] = alpha3;
    a2[21] = alpha2;
    a2[20] = alpha1;

    if (arg4 == 0) {
        v1 = sp28 << 3;
        v1 = v1 / 4096;
        *(s16 *)(a2 + 8) = base[activeBuff + 10] + arg0;
        *(s16 *)(a2 + 10) = base[activeBuff + 12] + arg1;
        *(s16 *)(a2 + 16) = base[activeBuff + 10] + arg2 - v1;
        *(s16 *)(a2 + 18) = base[activeBuff + 12] + arg3;
        *(s16 *)(a2 + 24) = base[activeBuff + 10] + arg2 + v1;
        *(s16 *)(a2 + 26) = base[activeBuff + 12] + arg3;
    } else {
        *(s16 *)(a2 + 8) = base[activeBuff + 10] + arg0;
        *(s16 *)(a2 + 10) = base[activeBuff + 12] + arg1;
        *(s16 *)(a2 + 16) = base[activeBuff + 10] + arg2;
        *(s16 *)(a2 + 18) = base[activeBuff + 12] + arg3;
        *(s16 *)(a2 + 24) = base[activeBuff + 10] + arg4;
        *(s16 *)(a2 + 26) = base[activeBuff + 12] + arg5;
    }

    AddPrim(&ot_buff0[(activeBuff << 10) + (arg6 << 2)], a2);
}
