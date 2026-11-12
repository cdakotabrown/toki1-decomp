/* @SymbolStart: 0x80157CCC */
/* @SymbolSize: 340 */
/* @RodataBase: 0x8015E1D8 */
/* @CompileEnv: IDO52_COPYPROP_VAR=-8 IDO52_COPYPROP_SITE=lod:4:deny */
#include "types.h"

extern s32 get_rand_mod(s32 arg);
extern void rpg_bat_koe_set(s32 a0, s32 a1, s32 a2);
extern void rpg_bat_talk_clear(void);
extern void rpg_bat_seq_step_next(void);

extern s32 rpg_bat_talk_work_step;
extern s32 rpg_bat_seq_work[];

s32 rpg_bat_effect_pick(void)
{
    s32 sel;

    sel = rpg_bat_talk_work_step;
    switch (sel) {
    case 0:
        sel = get_rand_mod(6);
        rpg_bat_seq_work[49] = sel;
        switch (sel) {
        case 0:
            rpg_bat_koe_set(0x180000B1, 0x0C0000B7, 0);
            break;
        case 1:
            rpg_bat_koe_set(0x1200009B, 0x050000A4, 0);
            break;
        case 2:
            rpg_bat_koe_set(0x1D0000AF, 0x200000B7, 0);
            break;
        case 3:
            rpg_bat_koe_set(0x0300006B, 0x0B00006E, 0);
            break;
        case 4:
            rpg_bat_koe_set(0x08000028, 0x06000030, 0);
            break;
        case 5:
            rpg_bat_koe_set(0x150000AC, 0x0B0000B7, 0);
            break;
        }
        rpg_bat_talk_work_step++;
        break;
    case 1:
        rpg_bat_talk_clear();
        rpg_bat_seq_step_next();
        break;
    }
}
