/* Address: 0x8004B6C8 */
/* Size: 552 bytes (138 instructions) */
/* @CompileEnv: IDO52_COPYPROP_VAR=-16,-12 IDO52_FORCE_WEB_COLOR=52:s2,20:s1 IDO52_AS1_TIE_REJECT=436 IDO52_UGEN_TEMP_REG=3:t9,4:t0,5:t1,6:t2,7:t3,8:t4,9:t5,10:t6,11:t7,12:t8 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: remap_stack_slots --map '76:92' --keep-frame */
/* @PostProc: patch_insn_reg_field 0x54:rt:v1:t8 0x5c:rs:v1:t8 0x6c:rs:v1:t8 */
/* @PostProc: swap_instructions --offset1 0x7c --offset2 0x80 */
/* @PostProc: swap_instructions --offset1 0x80 --offset2 0x84 */
/* @PostProc: swap_instructions --offset1 0x84 --offset2 0x88 */
/* @PostProc: swap_instructions --offset1 0x88 --offset2 0x8c */
/* @PostProc: swap_instructions --offset1 0x8c --offset2 0x90 */
/* @PostProc: swap_instructions --offset1 0x6c --offset2 0x70 */
/* @PostProc: swap_instructions --offset1 0x70 --offset2 0x74 */
/* @PostProc: swap_instructions --offset1 0x74 --offset2 0x78 */
/* @PostProc: swap_instructions --offset1 0x78 --offset2 0x7c */
/* @PostProc: swap_instructions --offset1 0x7c --offset2 0x80 */
/* @PostProc: swap_instructions --offset1 0x80 --offset2 0x84 */
#include "types.h"

extern u8 _debug_number_show_tbl0;
extern u8 _debug_number_show_str0;
extern s32 activeBuff;
extern u8 screen_w;
extern u8 debug_font_uv[];
extern u8 ot_buff0[];

extern s32 prim_alloc(s32, s32);
extern void SetSprt(s32);
extern void SetSemiTrans(s32, s32);
extern s16 GetClut(s32, s32);
extern void AddPrim(s32, s32);
extern void dtd_on_tpage(s32, s32, s32, s32, s32);

void _debug_string_show(u8 *a0_str, s32 a1_xOff, s32 a2_yOff, s32 a3_arg3) {
    s32 obj;
    s32 xOff;
    u32 ch;
    s32 half;
    u8 *str;
    s32 bright;
    u8 *tableBase;
    s32 width;
    s32 tpageBits;
    s32 clutBits;
    u32 raw;

    if ((u8)_debug_number_show_tbl0 < 4U) {
        return;
    }
    if (_debug_number_show_str0 == 0) {
        return;
    }
    str = a0_str;
    if (*a0_str == 0) {
        goto post_loop;
    }
    xOff = a1_xOff;
    tpageBits = (a3_arg3 & 8) * 2 + 256;
    clutBits = (a3_arg3 & 7) + 504;
    tableBase = &screen_w;
    width = 8;
    bright = 128;
    raw = *a0_str & 0xff;
    ch = raw;

    do {
        s32 charIdx;
        ch = ch & 0x1f;
        half = raw & 0x20;
        charIdx = ch;

        obj = prim_alloc(20, activeBuff);
        SetSprt(obj);
        SetSemiTrans(obj, 0);

        *(u8 *)(obj + 4) = bright;
        *(u8 *)(obj + 5) = bright;
        *(u8 *)(obj + 6) = bright;

        *(s16 *)(obj + 8) = *(s16 *)((u8 *)tableBase + activeBuff * 2 + 20) + xOff;
        *(s16 *)(obj + 10) = *(s16 *)((u8 *)tableBase + activeBuff * 2 + 24) + a2_yOff;

        if (half != 0) {
            xOff += 6;
        } else {
            xOff += 8;
        }

        if (half != 0) {
            *(u8 *)(obj + 12) = debug_font_uv[0] + charIdx * 8 - 8;
            *(u8 *)(obj + 13) = debug_font_uv[1];
        } else {
            *(u8 *)(obj + 12) = debug_font_uv[0] + charIdx * 8 - 8;
            *(u8 *)(obj + 13) = debug_font_uv[1] - 16;
        }

        if (half != 0) {
            *(s16 *)(obj + 16) = 6;
            *(s16 *)(obj + 18) = 12;
        } else {
            *(s16 *)(obj + 16) = width;
            *(s16 *)(obj + 18) = width;
        }

        *(s16 *)(obj + 14) = GetClut(tpageBits, clutBits);

        AddPrim((s32)&ot_buff0[(activeBuff << 10) + 4], obj);

        str++;
        raw = *str;
        ch = raw;
    } while (raw != 0);

post_loop:
    dtd_on_tpage(704, 256, 1, 0, 0);
}
