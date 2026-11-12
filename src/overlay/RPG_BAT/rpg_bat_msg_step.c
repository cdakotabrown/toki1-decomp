/* @RodataBase: 0x8015CDCC */
/* @SymbolStart: 0x8014B79C */
/* @SymbolSize: 776 */
/* @CompileFlags: -cckr */
#include "types.h"
#include "tokimeki/class_w.h"

extern s32 *rpg_bat_msg_tbl_ptr;
extern s32 rpg_bat_msg_time;
extern s32 rpg_bat_msg_num;
extern s32 rpg_bat_msg_ctl;
extern s32 rpg_bat_msg_ctl_mode;
extern s32 rpg_bat_msg_ctl_cnt;
extern CLASS_W class_w[];

void rpg_bat_msg_step()
{
    s32 pad;
    s32 idx;

    if (rpg_bat_msg_ctl_mode == 0) {
        return;
    }
    switch (rpg_bat_msg_ctl) {
    case 0:
        rpg_bat_anim_set(36, 50, 5, 1);
        rpg_bat_count_up(0);
        break;
    case 1:
        if (rpg_bat_rec_flag_check(36)) {
            rpg_bat_count_up(0);
        }
        break;
    case 2:
        rpg_bat_anim_set(36, 3, 1, 1);
        rpg_bat_count_up(0);
        break;
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
        idx = rpg_bat_msg_ctl - 3;
        if (idx >= rpg_bat_msg_num) {
            rpg_bat_msg_ctl = 0xFFFE;
            break;
        }
        if (++rpg_bat_msg_ctl_cnt == 1) {
            k_disp_start(set_kanji_string(-126, -102, 0, rpg_bat_msg_tbl_ptr[idx], 0));
        }
        if (rpg_bat_msg_ctl_mode == 4 && get_CD_State() == 2) {
            rpg_bat_msg_ctl_cnt = rpg_bat_msg_time + 1;
            rpg_bat_msg_ctl_mode = 1;
        }
        if (rpg_bat_msg_time < rpg_bat_msg_ctl_cnt) {
            switch (rpg_bat_msg_ctl_mode) {
            case 1:
                k_sub_reset();
                rpg_bat_count_up(0);
                break;
            case 3:
                break;
            case 4:
                rpg_bat_msg_ctl_mode = 1;
                break;
            case 128:
            case 129:
            case 130:
            case 131:
            case 132:
            case 133:
            case 134:
            case 135:
            case 136:
            case 137:
            case 138:
                if (idx != (rpg_bat_msg_ctl_mode & 0x7F)) {
                    k_sub_reset();
                    rpg_bat_count_up(0);
                }
                break;
            }
        }
        break;
    case 0xFFFE:
        rpg_bat_anim_set(36, 50, 33, 1);
        class_w[132].frame = 6;
        rpg_bat_count_up(0);
        break;
    case 0xFFFF:
    default:
        if (rpg_bat_rec_flag_check(36)) {
            rpg_bat_anim_set(36, 255, 0, 0);
            rpg_bat_msg_ctl_cnt = 0;
            rpg_bat_msg_ctl = 0;
            rpg_bat_msg_ctl_mode = 0;
        }
        break;
    }
}
