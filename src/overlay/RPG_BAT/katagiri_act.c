/* @SymbolStart: 0x80151C10 */
/* @RodataBase: 0x8015D678 */
/* @SymbolSize: 808 */
/* @PostProc: patch_insn_reg_field 0xb4:rt:t6:t5 0x140:rt:t5:t7 0x144:rs:t5:t7 0x144:rt:t5:t7 0x148:rs:t6:t5 0x14c:rd:t7:t6 0x14c:rt:t6:t5 0x150:rs:t6:t5 0x154:rd:t7:t6 0x158:rs:t5:t6 0x258:rt:t6:t5 0x25c:rs:t6:t5 0x25c:rt:t6:t5 0x264:rd:t5:t6 0x264:rt:t6:t5 0x268:rs:t5:t6 */
/* katagiri_act - RPG_BAT overlay, Address: 0x80151C18, Size: 800 bytes (200 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern s32 strcpy();
extern s32 rpg_bat_effect_rec_set();
extern s32 rpg_bat_anim_set();
extern s32 get_rand_mod();
extern s32 rpg_bat_slot_adjust();
extern s32 rpg_bat_sprite_move();
extern s32 rpg_bat_param_set();
extern s32 rpg_bat_msg_set();
extern s32 rpg_bat_act_next();
extern s32 rpg_bat_reset();
extern s32 rpg_bat_act_wait();
extern s32 rpg_bat_act_gate();
extern s32 rpg_bat_msg_pick();

extern char katagiri_act_str0[];
extern char rpg_bat_msg_buf3[];
extern char rpg_bat_msg_tbl3[];

extern s32 rpg_bat_dmg[];
extern s32 rpg_bat_player_dmg;
extern volatile s32 rpg_bat_seq_work;
extern s32 rpg_bat_seq_wait_cnt;
extern s32 rpg_bat_seq_tween_from;
extern RPG_BAT_UNIT rpg_bat_unit1;
extern RPG_BAT_UNIT rpg_bat_unit2;

void katagiri_act(void)
{
    s32 sel;
    s32 tmp;
    s32 v0;

    sel = rpg_bat_unit2.step;
    switch ((u32)sel) {
    case 0:
        strcpy((s32)rpg_bat_msg_buf3, (s32)katagiri_act_str0);
        rpg_bat_msg_set((s32)rpg_bat_msg_tbl3, 1, 90);
        rpg_bat_effect_rec_set(255);
        rpg_bat_seq_work = get_rand_mod(4) + 3;
        rpg_bat_seq_tween_from = 0;
        rpg_bat_act_next();
        break;

    case 1:
        rpg_bat_act_gate(1);
        break;

    case 2:
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
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
        tmp = sel - 3;
        if ((sel & 1) == 0) {
            if (rpg_bat_unit2.cnt == 0) {
                rpg_bat_msg_pick();
                rpg_bat_anim_set(29, 5, 1, 1);
                v0 = get_rand_mod(56);
                rpg_bat_seq_wait_cnt = v0 - 48;
                rpg_bat_unit2.cnt = rpg_bat_unit2.cnt + 1;
            } else {
                rpg_bat_sprite_move(29, 0, rpg_bat_seq_tween_from, -239, rpg_bat_seq_wait_cnt, 30);
            }
        } else if ((tmp / 2) == rpg_bat_seq_work) {
            rpg_bat_msg_pick();
            rpg_bat_anim_set(29, 6, 1, 1);
            rpg_bat_unit2.step = 50;
        } else if (rpg_bat_unit2.cnt == 0) {
            rpg_bat_msg_pick();
            rpg_bat_anim_set(29, 6, 1, 1);
            v0 = get_rand_mod(56);
            rpg_bat_seq_tween_from = v0 - 48;
            rpg_bat_unit2.cnt = rpg_bat_unit2.cnt + 1;
        } else {
            rpg_bat_sprite_move(29, -239, rpg_bat_seq_wait_cnt, 0, rpg_bat_seq_tween_from, 30);
        }
        break;

    case 50:
        rpg_bat_sprite_move(29, -239, rpg_bat_seq_wait_cnt, 0, 0, 30);
        break;

    case 51:
        rpg_bat_anim_set(29, 0, 1, 1);
        rpg_bat_param_set();
        if ((rpg_bat_unit1.flag << 11) < 0) {
            rpg_bat_slot_adjust(0, 0, rpg_bat_dmg[0], 1);
            rpg_bat_slot_adjust(1, 1, rpg_bat_dmg[1], 1);
            rpg_bat_slot_adjust(2, 2, rpg_bat_dmg[2], 1);
            rpg_bat_slot_adjust(3, 33, rpg_bat_player_dmg, 1);
        } else {
            rpg_bat_slot_adjust(0, 3, rpg_bat_dmg[0], 1);
            rpg_bat_slot_adjust(1, 33, rpg_bat_player_dmg, 1);
        }
        rpg_bat_act_next();
        break;

    case 52:
        rpg_bat_act_wait(60);
        break;

    case 53:
        rpg_bat_reset();
        break;
    }
}
