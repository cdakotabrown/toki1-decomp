/* @SymbolStart: 0x80136530 */
/* @RodataBase: 0x8015AFC4 */
/* @SymbolSize: 524 */
/* rpg_bat_step2 - RPG_BAT overlay, Address: 0x80136538, Size: 516 bytes (129 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern s32 get_CD_State();
extern s32 strcpy();
extern s32 rpg_bat_himo_anim_step();
extern s32 rpg_bat_anim_set2();
extern s32 rpg_bat_koe_set();
extern s32 rpg_bat_girl_move_out();
extern s32 rpg_bat_girl_move_in();
extern s32 rpg_bat_msg_set();
extern s32 rpg_bat_step_next();
extern s32 rpg_bat_timer_step();

extern char rpg_bat_msg_buf[];
extern char rpg_bat_msg_tbl[];
extern char rpg_bat_step2_str0[];
extern s32 rpg_bat_msg_time;
extern s32 rpg_bat_msg_ctl_cnt;
extern s32 rpg_bat_seq_work[];
extern RPG_BAT_UNIT rpg_bat_unit0;

void rpg_bat_step2(void)
{
    s32 v0;

    switch ((u32)(rpg_bat_unit0.step - 6)) {
    case 0:
        rpg_bat_girl_move_out();
        break;
    case 1:
        strcpy((s32)rpg_bat_msg_buf, (s32)rpg_bat_step2_str0);
        rpg_bat_msg_set((s32)rpg_bat_msg_tbl, 1, 500);
        rpg_bat_koe_set(39, 0x0600002D, 0);
        rpg_bat_anim_set2(29, 11, 1, 1);
        rpg_bat_step_next();
        break;
    case 2:
        rpg_bat_timer_step(30);
        break;
    case 3:
        v0 = rpg_bat_unit0.cnt + 1;
        rpg_bat_unit0.cnt = v0;
        if (v0 >= 301) {
            rpg_bat_msg_ctl_cnt = rpg_bat_msg_time + 1;
            rpg_bat_anim_set2(29, 0, 1, 1);
            rpg_bat_step_next();
        }
        if (get_CD_State() == 4)
            rpg_bat_seq_work[49] = 1;
        if (rpg_bat_seq_work[49] != 1)
            return;
        if (get_CD_State() != 2)
            return;
        rpg_bat_msg_ctl_cnt = rpg_bat_msg_time + 1;
        rpg_bat_anim_set2(29, 0, 1, 1);
        rpg_bat_seq_work[49] = 0;
        rpg_bat_step_next();
        break;
    case 4:
        rpg_bat_himo_anim_step();
        break;
    case 5:
        rpg_bat_himo_anim_step();
        break;
    case 6:
        rpg_bat_himo_anim_step();
        break;
    case 7:
        rpg_bat_himo_anim_step();
        break;
    case 8:
        rpg_bat_himo_anim_step();
        break;
    case 9:
        rpg_bat_timer_step(30);
        break;
    case 10:
        rpg_bat_girl_move_in();
        break;
    case 11:
        rpg_bat_unit0.step = 100;
        break;
    }
}
