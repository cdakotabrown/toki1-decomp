/* date_girl6_serifu_face - DATE overlay, Address: 0x80157628, 300 bytes (75 insns) */
#include "types.h"

extern s32 get_g_zyotai_s();
extern s32 bustup_face_change();
extern void func_8014C530();
extern u16 now_serifuev_in;
extern s32 step_counter;

s32 date_girl6_serifu_face(s32 a0, s32 a1, s32 a2, s32 a3)
{
    s32 ret;
    s32 m;
    u8 masked;
    s32 one;

    func_8014C530(a0, a1, a2, a3);
    ret = get_g_zyotai_s(6);

    m = ret;
    masked = (u8)(m & 0x7F);
    m = masked;
    one = 1;
    if ((u32)m >= 2U && now_serifuev_in == 8 && one == step_counter) {
        bustup_face_change(2);
    } else if ((u32)m >= 2U && now_serifuev_in == 10 && one == step_counter) {
        bustup_face_change(3);
    } else if ((u32)m >= 2U && now_serifuev_in == 6 && one == step_counter) {
        bustup_face_change(1);
    } else if ((u32)m >= 2U && now_serifuev_in == 12 && one == step_counter) {
        bustup_face_change(1);
    }
    step_counter = step_counter + 1;
}
