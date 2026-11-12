/* @PostProc: patch_insn_reg_field 0x2c:rt:t1:t0 0x30:rt:t0:t1 0x34:rt:t3:t4 0x4c:rs:t3:t9 0x4c:rt:t9:t4 */
/* tt_obj_move - TT overlay, Address: 0x80133288, Size: 332 bytes (83 insns) */
#include "types.h"

extern s32 *tt_stage_wk;
extern u8 *tt_player_wk;

extern void tt_shot_aimed_make(s16 x, s16 y, s32 w, s32 h);

void tt_obj_move(u8 *obj)
{
    u8 *p;
    s16 x;
    s16 y;
    s32 dx;
    s32 dy;
    s32 adx;
    s32 ady;

    p = tt_player_wk;
    *(s32 *)(obj + 36) = *tt_stage_wk - *(s32 *)(obj + 28);
    *(s32 *)(obj + 32) = *(s32 *)(obj + 32) + *(s32 *)(obj + 40);
    *(s32 *)(obj + 36) = *(s32 *)(obj + 36) + *(s32 *)(obj + 44);
    *(s16 *)(obj + 20) = *(s16 *)(obj + 34);
    *(s16 *)(obj + 22) = *(s16 *)(obj + 38);

    if (*(u16 *)(obj + 4) == 64) {
        if ((u32)*(u16 *)(obj + 94) >= 41U && obj[96] == 0) {
            obj[64] = obj[64] & ~0x40;
        }
        if (obj[64] & 1) {
            y = *(s16 *)(obj + 22);
            obj[64] = 192;
            *(s16 *)(obj + 94) = 0;
            if (y < 192) {
                x = *(s16 *)(obj + 20);
                dx = *(s16 *)(p + 20) - x;
                adx = (dx < 0) ? -dx : dx;
                if (adx >= 32 ||
                    (ady = ((dy = *(s16 *)(p + 22) - y) < 0) ? -dy : dy) >= 32) {
                    obj[96] = obj[96] + 1;
                    tt_shot_aimed_make(x, y, 32, 160);
                }
            }
        }
        *(u16 *)(obj + 94) = *(u16 *)(obj + 94) + 1;
    }
}
