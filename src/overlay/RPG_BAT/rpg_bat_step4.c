/* @SymbolStart: 0x8013E300 */
/* @RodataBase: 0x8015BDA4 */
/* @SymbolSize: 456 */
/* rpg_bat_step4 - RPG_BAT overlay, Address: 0x8013E308, Size: 448 bytes (112 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern s32 get_CD_State();
extern s32 strcpy();
extern s32 rpg_bat_anim_set2();
extern s32 rpg_bat_koe_set();
extern s32 rpg_bat_girl_move_out();
extern s32 kiyokawa_hit_step();
extern s32 rpg_bat_girl_move_in();
extern s32 rpg_bat_msg_set();
extern s32 rpg_bat_step_next();
extern s32 rpg_bat_timer_step();

extern char rpg_bat_msg_buf[];
extern char rpg_bat_msg_tbl[];
extern char rpg_bat_step4_str0[];
extern s32 rpg_bat_msg_time;
extern s32 rpg_bat_msg_ctl_cnt;
extern s32 rpg_bat_seq_work[];
extern RPG_BAT_UNIT rpg_bat_unit0;

void rpg_bat_step4(void)
{
    s32 v1;

    switch ((u32)(rpg_bat_unit0.step - 6)) {
    case 0:
        rpg_bat_girl_move_out();
        break;
    case 1:
        strcpy((s32)rpg_bat_msg_buf, (s32)rpg_bat_step4_str0);
        rpg_bat_msg_set((s32)rpg_bat_msg_tbl, 1, 500);
        rpg_bat_koe_set(92, 0x0E000062, 0);
        rpg_bat_anim_set2(29, 8, 1, 1);
        rpg_bat_step_next();
        break;
    case 2:
        rpg_bat_timer_step(40);
        break;
    case 3:
        kiyokawa_hit_step();
        v1 = rpg_bat_unit0.cnt + 1;
        rpg_bat_unit0.cnt = v1;
        if (v1 >= 301) {
            rpg_bat_anim_set2(29, 0, 1, 1);
            rpg_bat_msg_ctl_cnt = rpg_bat_msg_time + 1;
            rpg_bat_step_next();
        }
        if (get_CD_State() == 4)
            rpg_bat_seq_work[49] = 1;
        if (rpg_bat_seq_work[49] != 1)
            return;
        if (get_CD_State() != 2)
            return;
        rpg_bat_anim_set2(29, 0, 1, 1);
        rpg_bat_msg_ctl_cnt = rpg_bat_msg_time + 1;
        rpg_bat_seq_work[49] = 0;
        rpg_bat_step_next();
        break;
    case 4:
        kiyokawa_hit_step();
        rpg_bat_timer_step(30);
        break;
    case 5:
        rpg_bat_girl_move_in();
        break;
    case 6:
        rpg_bat_unit0.step = 100;
        break;
    }
}
