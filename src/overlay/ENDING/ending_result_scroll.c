/* @SymbolStart: 0x8013B540 */
/* @SymbolSize: 212 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=54:s2,11:s1,32:t0 */
/* ending_result_scroll - ENDING overlay, Address: 0x8013B540, Size: 204 bytes (51 instructions) */
#include "types.h"

extern s32 get_now_k();
extern s32 get_k_work();
extern void sub_sub_step_up();
extern s32 frame_counter;
extern s16 last_x_y;

s32 ending_result_scroll(void)
{
    s32 tick = frame_counter;
    s32 s0;
    s32 s1;
    s32 s2;
    s32 v0;
    s16 v1;
    s32 t0;

    if ((tick & 3) != 0) {
        return 0;
    }
    {
        s1 = get_now_k() & 0xFF;
        s0 = 0;
        s2 = 1;
    loop:
        v0 = get_k_work(s0);
        s0 += 1;
        *(s16 *)(v0 + 2) = (s16)(*(s16 *)(v0 + 2) - 1);
        v1 = *(s16 *)(v0 + 2);
        t0 = s0 & 0xFF;
        if (v1 >= 81) {
            goto store_zero;
        }
        if (v1 < -80) {
        store_zero:
            *(u8 *)(v0 + 7) = 0;
        } else {
            *(u8 *)(v0 + 7) = (u8)s2;
        }
        s0 = t0;
        if ((u32)s1 >= (u32)t0) {
            goto loop;
        }
        last_x_y = last_x_y - 1;
        if (last_x_y < -64) {
            sub_sub_step_up();
        }
    }
}
