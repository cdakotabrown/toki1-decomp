/* Address: 0x800580A4 */
/* Size: 1648 bytes (412 instructions) */
/* Region: Game (0x80041000-0x80086000) */
/* Compiler: IDO 5.2 */
/* @CompileEnv: IDO52_FORM_SPLIT=const:128:101 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_stack_complete --old 64 --new 80 */
/* @PostProc: remap_stack_slots --map '56:60,64:80' --keep-frame */
/* @PostProc: swap_instructions 0xb8 0xc8 */
/* @PostProc: swap_instructions 0x124 0x128 */
/* @PostProc: swap_instructions 0x258 0x268 */
/* @PostProc: swap_instructions 0x288 0x290 */
/* @PostProc: swap_instructions 0x2bc 0x2cc */
/* @PostProc: swap_instructions 0x3e0 0x3f0 */
/* @PostProc: swap_instructions 0x410 0x418 */
/* @PostProc: swap_instructions 0x444 0x454 */
/* @PostProc: swap_instructions 0x560 0x570 */
/* @PostProc: swap_instructions 0x590 0x598 */
/* @PostProc: swap_instructions 0x5c4 0x5d4 */
#include "types.h"

extern u8  bg_change_tbl0[];
extern u8  bg_change_tbl2;
extern s16 screen_w[];
extern u8  ot_buff0[];
extern s32 activeBuff;

extern s32  prim_alloc(s32 size, s32 bank);
extern void goto_tpage(s32 x, s32 y, s32 id, s32 d, s32 e);
extern void AddPrim(u8 *ot, s32 obj);
extern void SetSemiTrans(s32 obj, s32 mode);
extern void SetShadeTex(s32 obj, s32 mode);
extern void SetSprt(s32 obj);

void bg_show_girl_out(s32 arg0)
{
    s32 obj;

    obj = prim_alloc(20, activeBuff);
    SetSprt(obj);
    SetSemiTrans(obj, 1);
    SetShadeTex(obj, 0);
    *(u8 *)(obj + 4) = (u8)(128 - bg_change_tbl2);
    *(u8 *)(obj + 5) = (u8)(128 - bg_change_tbl2);
    *(u8 *)(obj + 6) = (u8)(128 - bg_change_tbl2);
    if (arg0 == 1) {
        *(s16 *)(obj + 8) = (s16)(screen_w[activeBuff + 10] - 160);
        *(s16 *)(obj + 10) = (s16)(screen_w[activeBuff + 12] - 120);
        *(u8 *)(obj + 13) = 0;
        *(u8 *)(obj + 12) = 0;
        *(s16 *)(obj + 16) = 192;
        *(s16 *)(obj + 18) = 160;
    } else {
        *(s16 *)(obj + 8) = (s16)(screen_w[activeBuff + 10] - 160);
        *(s16 *)(obj + 10) = (s16)(screen_w[activeBuff + 12] - 48);
        *(u8 *)(obj + 13) = 160;
        *(u8 *)(obj + 12) = 0;
        *(s16 *)(obj + 16) = 192;
        *(s16 *)(obj + 18) = 88;
    }
    AddPrim(&ot_buff0[(activeBuff << 10) + 40], obj);
    if (bg_change_tbl0[arg0] != 0) {
        goto_tpage(320, 256, 10, 1, 2);
    } else {
        goto_tpage(704, 0, 10, 1, 2);
    }

    obj = prim_alloc(20, activeBuff);
    SetSprt(obj);
    SetSemiTrans(obj, 1);
    SetShadeTex(obj, 0);
    *(u8 *)(obj + 4) = (u8)(128 - bg_change_tbl2);
    *(u8 *)(obj + 5) = (u8)(128 - bg_change_tbl2);
    *(u8 *)(obj + 6) = (u8)(128 - bg_change_tbl2);
    if (arg0 == 1) {
        *(s16 *)(obj + 8) = (s16)(screen_w[activeBuff + 10] + 32);
        *(s16 *)(obj + 10) = (s16)(screen_w[activeBuff + 12] - 120);
        *(u8 *)(obj + 13) = 0;
        *(u8 *)(obj + 12) = 0;
        *(s16 *)(obj + 16) = 128;
        *(s16 *)(obj + 18) = 160;
    } else {
        *(s16 *)(obj + 8) = (s16)(screen_w[activeBuff + 10] + 32);
        *(s16 *)(obj + 10) = (s16)(screen_w[activeBuff + 12] - 48);
        *(u8 *)(obj + 13) = 160;
        *(u8 *)(obj + 12) = 0;
        *(s16 *)(obj + 16) = 128;
        *(s16 *)(obj + 18) = 88;
    }
    AddPrim(&ot_buff0[(activeBuff << 10) + 40], obj);
    if (bg_change_tbl0[arg0] != 0) {
        goto_tpage(512, 256, 10, 1, 2);
    } else {
        goto_tpage(896, 0, 10, 1, 2);
    }

    obj = prim_alloc(20, activeBuff);
    SetSprt(obj);
    SetSemiTrans(obj, 0);
    SetShadeTex(obj, 0);
    *(u8 *)(obj + 4) = bg_change_tbl2;
    *(u8 *)(obj + 5) = bg_change_tbl2;
    *(u8 *)(obj + 6) = bg_change_tbl2;
    if (arg0 == 1) {
        *(s16 *)(obj + 8) = (s16)(screen_w[activeBuff + 10] - 160);
        *(s16 *)(obj + 10) = (s16)(screen_w[activeBuff + 12] - 120);
        *(u8 *)(obj + 13) = 0;
        *(u8 *)(obj + 12) = 0;
        *(s16 *)(obj + 16) = 192;
        *(s16 *)(obj + 18) = 160;
    } else {
        *(s16 *)(obj + 8) = (s16)(screen_w[activeBuff + 10] - 160);
        *(s16 *)(obj + 10) = (s16)(screen_w[activeBuff + 12] - 48);
        *(u8 *)(obj + 13) = 160;
        *(u8 *)(obj + 12) = 0;
        *(s16 *)(obj + 16) = 192;
        *(s16 *)(obj + 18) = 88;
    }
    AddPrim(&ot_buff0[(activeBuff << 10) + 52], obj);
    if (bg_change_tbl0[arg0] != 0) {
        goto_tpage(320, 256, 13, 0, 2);
    } else {
        goto_tpage(704, 0, 13, 0, 2);
    }

    obj = prim_alloc(20, activeBuff);
    SetSprt(obj);
    SetSemiTrans(obj, 0);
    SetShadeTex(obj, 0);
    *(u8 *)(obj + 4) = bg_change_tbl2;
    *(u8 *)(obj + 5) = bg_change_tbl2;
    *(u8 *)(obj + 6) = bg_change_tbl2;
    if (arg0 == 1) {
        *(s16 *)(obj + 8) = (s16)(screen_w[activeBuff + 10] + 32);
        *(s16 *)(obj + 10) = (s16)(screen_w[activeBuff + 12] - 120);
        *(u8 *)(obj + 13) = 0;
        *(u8 *)(obj + 12) = 0;
        *(s16 *)(obj + 16) = 128;
        *(s16 *)(obj + 18) = 160;
    } else {
        *(s16 *)(obj + 8) = (s16)(screen_w[activeBuff + 10] + 32);
        *(s16 *)(obj + 10) = (s16)(screen_w[activeBuff + 12] - 48);
        *(u8 *)(obj + 13) = 160;
        *(u8 *)(obj + 12) = 0;
        *(s16 *)(obj + 16) = 128;
        *(s16 *)(obj + 18) = 88;
    }
    AddPrim(&ot_buff0[(activeBuff << 10) + 52], obj);
    if (bg_change_tbl0[arg0] != 0) {
        goto_tpage(512, 256, 13, 0, 2);
    } else {
        goto_tpage(896, 0, 13, 0, 2);
    }
}
