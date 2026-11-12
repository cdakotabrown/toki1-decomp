/* @CompileFlags: -cckr */
/* @SymbolSize: 256 */
/* date2_scroll_add - DATE2 overlay, Size: 256 bytes (64 insns) */
#include "types.h"

extern s32 get_g_zyotai_s();

extern s16 date2_g0_now_s;
extern s16 date2_girl_now_s;
extern u8 girl_no;

s32 date2_scroll_add()
{
    s32 v0;
    s32 v1;
    u8 masked;

    v0 = get_g_zyotai_s(0);
    v1 = v0;
    masked = (u8)(v1 & 0x7F);
    v1 = masked;
    if ((u32)v1 >= 2U) {
        date2_g0_now_s += 10;
    }
    if ((u32)v1 >= 3U) {
        date2_g0_now_s += 10;
    }
    if (v1 == 4) {
        date2_g0_now_s += 10;
    }
    v0 = get_g_zyotai_s(girl_no);
    v1 = v0;
    masked = (u8)(v1 & 0x7F);
    v1 = masked;
    if ((u32)v1 >= 2U) {
        date2_girl_now_s += 10;
    }
    if ((u32)v1 >= 3U) {
        date2_girl_now_s += 10;
    }
    if (v1 == 4) {
        date2_girl_now_s += 10;
    }
}
