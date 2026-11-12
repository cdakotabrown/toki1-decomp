/* @SymbolStart: 0x8014EBEC */
/* @SymbolSize: 344 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @CompileEnv: IDO52_NO_LOOP_TEST_REPLACEMENT=1 */
/* @PostProc: swap_instructions --offset1 0x114 --offset2 0x118 */
#include "types.h"

extern s32 rpg_bat_anim_set();

extern s32 rpg_bat_mode;
extern s32 rpg_bat_seq_work[];
extern s32 rpg_bat_se_wait[];
extern s32 rpg_bat_talk_work[];
s32 rpg_bat_talk_work_busy[2] = {1, 0};
s32 rpg_bat_unit0_flag[6] = {1, 2, 3, 4, 5, 6};
s32 rpg_bat_unit0_sub[3] = {7, 8, 9};
extern u8 cw40_active[];
extern u8 cw64_active;
extern u8 cw125_prio;

void rpg_bat_clear(void)
{
    s32 *p0, *p1, *p2;
    s32 s0, k;
    u8 *q;

    rpg_bat_unit0_flag[3] = 0;
    rpg_bat_unit0_flag[0] = 0;
    rpg_bat_unit0_flag[1] = 0;
    rpg_bat_unit0_flag[5] = 0;
    rpg_bat_unit0_sub[1] = 0;
    rpg_bat_unit0_sub[0] = 0;
    rpg_bat_unit0_sub[2] = 0;
    for (p0 = rpg_bat_seq_work; p0 < &rpg_bat_seq_work[50]; p0++) {
        *p0 = 0;
    }
    for (p1 = rpg_bat_se_wait; p1 < &rpg_bat_se_wait[10]; p1++) {
        *p1 = 0;
    }
    for (p2 = rpg_bat_talk_work; p2 < &rpg_bat_talk_work[12]; p2++) {
        *p2 = 0;
    }

    if (rpg_bat_mode != (s32)0x80000000) {
        s0 = 40;
        do {
            rpg_bat_anim_set(s0, 255, 1, 0);
            s0++;
        } while (s0 != 48);
    }
    k = 50;
    do {
        rpg_bat_anim_set(k, 255, 1, 0);
        k++;
    } while (k < 56);

    q = cw40_active;
    do {
        q += 272;
        q[6257] = 5;
        q[6325] = 5;
        q[6393] = 5;
        q[6461] = 5;
    } while (q != &cw64_active);

    if (rpg_bat_mode != (s32)0x80000000)
        cw125_prio = 5;
}
