/* tt_trio_spawn - TT overlay, Address: 0x8014B264, Size: 416 bytes (104 insns) */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @PostProc: patch_insn_reg_field 0x7c:rd:a0:v1 0x88:rs:a0:v1 0x88:rt:v1:t6 0x8c:rd:t6:t7 0x8c:rt:v1:t6 0x94:rt:t6:t7 0x98:rt:t7:t8 0x9c:rd:v1:t9 0x9c:rs:t7:t8 0x9c:rt:a0:v1 0xa0:rd:t8:t0 0xa0:rt:v1:t9 0xa4:rt:t8:t0 0x124:rd:t9:t1 0x128:rt:t9:t1 0x134:rt:v1:t3 0x138:rt:a0:t4 0x140:rd:t0:t2 0x148:rt:t0:t2 0x174:rt:v1:t3 0x17c:rt:a0:t4 */
#include "types.h"

extern s32 rsin(s32 angle);
extern s32 rcos(s32 angle);
extern s32 tt_angle_between(void *a0, void *a1);
extern s32 tt_slot_get(void *a0, void *a1);
extern s32 tt_slot_kind_set(void *a0, u32 a1, s32 a2);

extern s32 tt_player_wk;
extern s32 tt_slot_buf;

void tt_trio_spawn(u8 param)
{
    u8 *base;
    u8 *p;
    u32 pm;
    s32 i;
    s32 v1;
    s32 v0;
    s32 ang;
    s32 t1, t2, t3, t4;

    pm = param;
    base = (u8 *)tt_player_wk;
    p = (u8 *)tt_slot_buf;
    i = 0;

    do {
        p = (u8 *)tt_slot_get(p, p + 7680);
        if (p == 0) {
            return;
        }
        p[80] = (u8)i;
        v1 = i << 4;
        if (pm != 0) {
            v1 = v1 + 48;
            *(s32 *)(p + 32) = v1 << 16;
        } else {
            v1 = -48 - v1;
            *(s32 *)(p + 32) = v1 << 16;
        }
        *(s32 *)(p + 36) = -16;
        ang = tt_angle_between(p + 20, base + 20);
        if (i == 1) {
            tt_slot_kind_set(p, 66, 6);
        } else {
            tt_slot_kind_set(p, 65, 5);
            if (pm != 0) {
                if (i == 0) {
                    v0 = 96;
                } else {
                    v0 = -96;
                }
                ang = ang + v0;
            } else {
                if (i == 0) {
                    v0 = 96;
                } else {
                    v0 = -96;
                }
                ang = ang - v0;
            }
        }
        t1 = rcos(ang) << 5;
        *(s32 *)(p + 40) = t1;
        t2 = rsin(ang) << 5;
        t3 = *(s16 *)(p + 34);
        t4 = *(s16 *)(p + 38);
        i++;
        p += 120;
        *(s32 *)(p - 76) = t2;
        p[-21] = pm;
        *(s16 *)(p - 26) = 0;
        p[-24] = 0;
        *(s16 *)(p - 116) = 64;
        *(s16 *)(p - 110) = 0;
        *(s16 *)(p - 112) = 0;
        *(s16 *)(p - 114) = 0;
        p[-105] = 0;
        p[-106] = 0;
        p[-107] = 0;
        *(s16 *)(p - 100) = t3;
        *(s16 *)(p - 98) = t4;
    } while (i != 3);
}
