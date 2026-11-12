/* @CompileEnv: IDO52_FORCE_WEB_COLOR=21:v1,29:v1 */
/* @SymbolSize: 156 */
/* rpg_bat_work_clear - RPG_BAT overlay, Address: 0x8014F540, Size: 156 bytes */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern s32 rpg_bat_seq_work[50];
extern s32 rpg_bat_se_wait[10];
extern s32 rpg_bat_talk_work[12];
extern RPG_BAT_UNIT rpg_bat_unit3;

s32 rpg_bat_unit3_step[4] = {0};

void rpg_bat_work_clear(void)
{
    s32 *p0, *p1;
    s32 i;

    rpg_bat_unit3.aux10 = 0;
    rpg_bat_unit3.flag = 0;
    rpg_bat_unit3.aux08 = 0;
    rpg_bat_unit3_step[0] = 0;
    rpg_bat_unit3_step[2] = 0;
    rpg_bat_unit3_step[1] = 0;
    rpg_bat_unit3_step[3] = 0;

    for (p0 = rpg_bat_seq_work; p0 < &rpg_bat_seq_work[50]; p0++) {
        *p0 = 0;
    }
    for (p1 = rpg_bat_se_wait; p1 < &rpg_bat_se_wait[10]; p1++) {
        *p1 = 0;
    }
    i = 0;
    do {
        rpg_bat_talk_work[i] = 0;
        i++;
    } while (i < 12);
}
