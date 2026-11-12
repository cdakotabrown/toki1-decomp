/* @PostProc: patch_insn_reg_field 0x100:rt:v0:v1 0x108:rs:v0:v1 0x114:rs:v0:v1 0x13c:rt:v0:v1 0x148:rs:v0:v1 0x154:rs:v0:v1 0x158:rs:v0:v1 0x17c:rt:v0:v1 0x184:rs:v0:v1 0x18c:rs:v0:v1 */
/* Address: 0x8013B928, Size: 424 bytes (106 insns) */
#include "types.h"

extern void tt_cmd_once(s32 a);
extern s32 get_angle(s32 x, s32 y);
extern void tt_shot_heading_make(s32 a, s32 b, s32 c, s32 d, s32 e);
extern void shot_fan(s32 a, s32 b, s32 c, s32 d, s32 e);

extern u8 *tt_player_wk;
extern u8 *tt_boss_wk;

void tt_aim_burst(void)
{
    u8 *p;
    u8 *q;
    s32 sel;
    s32 ang;
    s32 i;
    s32 h;

    p = tt_boss_wk;
    sel = *(u16 *)(p + 868);
    q = tt_player_wk;
    if (sel == 128 || sel == 256) {
        tt_cmd_once(1560);
        ang = get_angle((*(s32 *)(q + 36) - *(s32 *)(p + 36)) >> 16,
                            (*(s32 *)(q + 32) - *(s32 *)(p + 32)) >> 16);
        for (i = 384; i < 1664; i += 256) {
            tt_shot_heading_make(*(s16 *)(p + 20) - 18, *(s16 *)(p + 22) + 10, 24,
                          ang + i, 172);
        }
        for (i = 384; i != 1664; i += 256) {
            tt_shot_heading_make(*(s16 *)(p + 20) + 18, *(s16 *)(p + 22) + 10, 24,
                          ang - i, 172);
        }
    }
    h = *(u16 *)(p + 868);
    if (h == 376 || h == 512) {
        shot_fan(*(s16 *)(p + 20) - 18, *(s16 *)(p + 22) + 10, 192, 40,
                      160);
        h = *(u16 *)(p + 868);
    }
    if (h == 384 || h == 504) {
        shot_fan(*(s16 *)(p + 20) + 18, *(s16 *)(p + 22) + 10, 192, 40,
                      160);
        h = *(u16 *)(p + 868);
    }
    if ((u32)h >= 512U) {
        *(u16 *)(p + 868) = 0;
    } else {
        *(u16 *)(p + 868) = (u16)(h + 1);
    }
}
