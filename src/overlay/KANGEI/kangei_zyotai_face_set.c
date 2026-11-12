/* kangei_zyotai_face_set - KANGEI overlay, Address: 0x80132354, Size: 124 bytes (31 insns) */
/* @SymbolSize: 84 */
#include "types.h"

extern s32 get_g_zyotai_h(u8 a0);
extern void bustup_face_change(s32 a0);
extern void sub_sub_step_up(void);

extern u8 now_girl;

void kangei_zyotai_face_set(void)
{
    s32 result;
    s32 mode;

    result = get_g_zyotai_h(now_girl);
    mode = result & 0xFF;
    if ((u32)result >= 3) {
        bustup_face_change(3);
    } else {
        bustup_face_change(mode);
    }
    sub_sub_step_up();
}
