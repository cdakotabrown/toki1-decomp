/* @CompileEnv: IDO52_FORCE_WEB_COLOR=21:v1,29:v1,37:v1,45:v1,70:v0,71:a0 */
/* @PostProc: swap_instructions --offset1 0xa0 --offset2 0xa4 */
/* @SymbolSize: 216 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* rpg_bat_reset - RPG_BAT overlay, Address: 0x8014F278, Size: 216 bytes */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern s32 rpg_bat_mode;
extern s32 rpg_bat_seq_work[50];
extern s32 rpg_bat_se_wait[10];
extern s32 rpg_bat_talk_work[12];
extern u8  cw125_prio;
extern u8  cw40_active[];
extern u8  cw64_active;
extern RPG_BAT_UNIT rpg_bat_unit2;

s32 rpg_bat_unit2_flag[4] = {0};
s32 rpg_bat_unit2_step[3] = {0};

void rpg_bat_reset(void)
{
    s32 *p0, *p1, *p2;
    u8 *q;

    rpg_bat_unit2_flag[3] = 0;
    rpg_bat_unit2_flag[0] = 0;
    rpg_bat_unit2_flag[1] = 0;
    rpg_bat_unit2_step[0] = 0;
    rpg_bat_unit2_step[2] = 0;
    rpg_bat_unit2_step[1] = 0;
    rpg_bat_unit2.wait = 0;

    for (p0 = rpg_bat_seq_work; p0 < &rpg_bat_seq_work[50]; p0++) {
        *p0 = 0;
    }
    for (p1 = rpg_bat_se_wait; p1 < &rpg_bat_se_wait[10]; p1++) {
        *p1 = 0;
    }
    for (p2 = rpg_bat_talk_work; p2 < &rpg_bat_talk_work[12]; p2++) {
        *p2 = 0;
    }

    q = cw40_active;
    do {
        q += 272;
        q[6257] = 5;
        q[6325] = 5;
        q[6393] = 5;
        q[6461] = 5;
    } while (q != &cw64_active);

    if (rpg_bat_mode != (s32)0x80000000) {
        cw125_prio = 5;
    }
}
