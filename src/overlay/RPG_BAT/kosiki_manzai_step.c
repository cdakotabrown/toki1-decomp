/* @SymbolStart: 0x80143830 */
/* @RodataBase: 0x8015C444 */
/* Address: 0x80143838 */
/* Size: 420 bytes (105 insns) */
/* @SymbolSize: 428 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern void strcpy(s32, s32);
extern void rpg_bat_koe_set(s32, s32, s32);
extern void rpg_bat_msg_set(s32, s32, s32);
extern void rpg_bat_anim_set(s32, s32, s32, s32);
extern void rpg_bat_step_next(void);
extern s32  get_CD_State(void);
extern void rpg_bat_timer_step(s32);

extern u8  rpg_bat_msg_buf[];
extern u8  kosiki_manzai_step_str0[];
extern u8  rpg_bat_msg_tbl[];
extern s32 rpg_bat_msg_time;
extern s32 rpg_bat_msg_ctl_cnt;
extern s32 rpg_bat_seq_work[];
extern RPG_BAT_UNIT rpg_bat_unit0;

void kosiki_manzai_step(void) {
    s32 n;

    switch (rpg_bat_unit0.step - 6) {
    case 0:
        strcpy((s32)rpg_bat_msg_buf, (s32)kosiki_manzai_step_str0);
        rpg_bat_koe_set(121, 0x12000085, 0);
        rpg_bat_msg_set((s32)rpg_bat_msg_tbl, 1, 500);
        rpg_bat_anim_set(29, 6, 1, 1);
        rpg_bat_step_next();
        break;
    case 1:
        n = rpg_bat_unit0.cnt + 1;
        rpg_bat_unit0.cnt = n;
        if (n >= 301) {
            rpg_bat_anim_set(29, 0, 1, 1);
            rpg_bat_msg_ctl_cnt = rpg_bat_msg_time + 1;
            rpg_bat_step_next();
        }
        if (get_CD_State() == 4) {
            rpg_bat_seq_work[49] = 1;
        }
        if (rpg_bat_seq_work[49] != 1) {
            break;
        }
        if (get_CD_State() == 2) {
            rpg_bat_anim_set(29, 0, 1, 1);
            rpg_bat_msg_ctl_cnt = rpg_bat_msg_time + 1;
            rpg_bat_seq_work[49] = 0;
            rpg_bat_step_next();
        }
        break;
    case 2:
        rpg_bat_anim_set(29, 0, 1, 1);
        rpg_bat_step_next();
        break;
    case 3:
        rpg_bat_timer_step(30);
        break;
    case 4:
        rpg_bat_unit0.step = 100;
        break;
    }
}
