/* @CompileEnv: IDO52_FORCE_WEB_COLOR=102:t1 */
/* @PostProc: swap_instructions --offset1 0x178 --offset2 0x17c */
/* tt_rec_target_claim - TT overlay, Address: 0x80148E6C, Size: 540 bytes. */
#include "types.h"

extern s32 *tt_stage_wk;
extern u8 *tt_player_wk;
extern u8 *tt_trail_buf;
extern u8 *tt_obj_pool;

void tt_rec_target_claim(u8 *r, s32 idx)
{
    s32 *ctl;
    u8 *src;
    u8 *e;
    s32 i;
    s32 d;
    s32 h;
    s32 a;

    ctl = tt_stage_wk;
    src = tt_player_wk;
    if (idx < 2) {
        *(s32 *)(r + 32) = *(s16 *)(src + 20) << 16;
        *(s32 *)(r + 28) = *ctl - ((*(s16 *)(src + 22) - 96) << 16);
        *(u16 *)(r + 68) = 0xFFFF;
        i = 0;
        e = tt_obj_pool;
        do {
            if (e[0] != 0 && *(u16 *)(e + 4) == 64 && e[100] == 0) {
                d = *(s16 *)(src + 22) - *(s16 *)(e + 22);
                if (d < 96 && d >= 17) {
                    h = *(s16 *)(e + 20) - *(s16 *)(src + 20);
                    a = (h < 0) ? -h : h;
                    if (a < 32) {
                        *(s32 *)(r + 32) = *(s32 *)(e + 32);
                        *(s32 *)(r + 28) = *(s32 *)(e + 28);
                        *(u16 *)(r + 68) = i;
                        e[100] = 1;
                        goto done;
                    }
                }
            }
            i += 1;
            e += 120;
        } while (i != 32);
    } else {
        src = tt_trail_buf + (idx >> 1) * 164 - 164;
        *(s32 *)(r + 32) = *(s16 *)(src + 20) << 16;
        *(s32 *)(r + 28) = *ctl - ((*(s16 *)(src + 22) - 96) << 16);
        *(u16 *)(r + 68) = 0xFFFF;
        i = 0;
        e = tt_obj_pool;
        do {
            if (e[0] != 0 && e[100] == 0) {
                d = *(s16 *)(src + 22) - *(s16 *)(e + 22);
                if (d < 96 && d >= 17) {
                    h = *(s16 *)(e + 20) - *(s16 *)(src + 20);
                    a = (h < 0) ? -h : h;
                    if (a < 32) {
                        *(s32 *)(r + 32) = *(s32 *)(e + 32);
                        *(s32 *)(r + 28) = *(s32 *)(e + 28);
                        *(u16 *)(r + 68) = i;
                        e[100] = 1;
                        goto done;
                    }
                }
            }
            i += 1;
            e += 120;
        } while (i != 32);
    }
done:
    *(s32 *)(r + 40) = 0;
    *(s32 *)(r + 44) = 0;
    *(u16 *)(r + 70) = 0;
}
