/* @SymbolStart: 0x80134A70 */
/* @RodataBase: 0x8015AD60 */
/* @SymbolSize: 700 */
/* rpg_bat_load_step - RPG_BAT overlay, Address: 0x80134A78, Size: 692 bytes (173 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern s32 get_CD_State();
extern s32 strcpy();
extern s32 rpg_bat_anim_set2();
extern s32 rpg_bat_koe_set();
extern s32 rpg_bat_girl_move_out();
extern s32 kiyokawa_hit_step2();
extern s32 rpg_bat_girl_move_in();
extern s32 rpg_bat_msg_set();
extern s32 rpg_bat_step_next();
extern s32 rpg_bat_timer_step();

extern char rpg_bat_msg_buf[];
extern char rpg_bat_msg_buf_1[];
extern char rpg_bat_msg_tbl[];
extern char rpg_bat_load_step_str0[];
extern char rpg_bat_load_step_str1[];
extern s32 rpg_bat_msg_time;
extern s32 rpg_bat_msg_ctl_cnt;
extern s32 rpg_bat_msg_ctl_mode;
extern s32 rpg_bat_seq_work[];
extern RPG_BAT_UNIT rpg_bat_unit0;

void rpg_bat_load_step(void)
{
    s32 v1;

    switch ((u32)(rpg_bat_unit0.step - 6)) {
    case 0:
        rpg_bat_girl_move_out();
        break;
    case 1:
        strcpy((s32)rpg_bat_msg_buf, (s32)rpg_bat_load_step_str0);
        strcpy((s32)rpg_bat_msg_buf_1, (s32)rpg_bat_load_step_str1);
        rpg_bat_msg_set((s32)rpg_bat_msg_tbl, 2, 10);
        rpg_bat_koe_set(6, 0x02000008, 0);
        rpg_bat_msg_ctl_mode = 3;
        rpg_bat_anim_set2(29, 6, 1, 1);
        rpg_bat_step_next();
        break;
    case 2:
        v1 = rpg_bat_unit0.cnt + 1;
        rpg_bat_unit0.cnt = v1;
        if (v1 >= 121) {
            rpg_bat_msg_ctl_mode = 1;
            rpg_bat_step_next();
        }
        if (get_CD_State() == 4)
            rpg_bat_seq_work[49] = 1;
        if (rpg_bat_seq_work[49] != 1)
            return;
        if (get_CD_State() != 2)
            return;
        rpg_bat_msg_ctl_mode = 1;
        rpg_bat_seq_work[49] = 0;
        rpg_bat_step_next();
        break;
    case 3:
        rpg_bat_koe_set(9, 0x03000011, 1);
        rpg_bat_msg_ctl_mode = 3;
        rpg_bat_step_next();
        break;
    case 4:
        rpg_bat_timer_step(5);
        break;
    case 5:
        kiyokawa_hit_step2();
        v1 = rpg_bat_unit0.cnt + 1;
        rpg_bat_unit0.cnt = v1;
        if (v1 >= 301) {
            rpg_bat_msg_ctl_mode = 1;
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
        rpg_bat_msg_ctl_mode = 1;
        rpg_bat_msg_ctl_cnt = rpg_bat_msg_time + 1;
        rpg_bat_anim_set2(29, 0, 1, 1);
        rpg_bat_seq_work[49] = 0;
        rpg_bat_step_next();
        break;
    case 6:
        kiyokawa_hit_step2();
        rpg_bat_timer_step(30);
        break;
    case 7:
        rpg_bat_girl_move_in();
        break;
    case 8:
        rpg_bat_unit0.step = 100;
        break;
    }
}
