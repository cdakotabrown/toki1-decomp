/* Function: date_girl6_zyotai_face_step */
/* Overlay: DATE */
/* Address: 0x801570B4 */
/* Size: 96 bytes (24 instructions) */
#include "types.h"

extern u8 now_girl;
extern s32 get_g_zyotai_s(u8 val);
extern s32 get_g_zyotai_h(u8 val);
extern void bustup_face_change(s32 arg);
extern void sub_sub_step_up(void);

void date_girl6_zyotai_face_step(void)
{
    u8 val;
    s32 result;

    val = now_girl;
    result = get_g_zyotai_s(val);

    if ((u32)result < 2u) {
        bustup_face_change(0);
    } else {
        val = now_girl;
        result = get_g_zyotai_h(val);
        bustup_face_change(result & 0xFF);
    }
    sub_sub_step_up();
}
