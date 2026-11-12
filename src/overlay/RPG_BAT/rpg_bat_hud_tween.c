/* @SymbolStart: 0x80135358 */
/* @SymbolSize: 1700 */
/* @PostProc: patch_stack_complete --old 56 --new 72 */
/* @PostProc: remap_stack_slots --from 32 --to 52 */
/* @PostProc: patch_insn_reg_field 0x48c:rd:v0:v1 0x490:rs:v0:v1 0x5a0:rd:v0:v1 0x658:rd:v0:v1 0x65c:rt:v0:v1 */
/* rpg_bat_hud_tween - RPG_BAT overlay, Address: 0x80135360, Size: 1692 bytes */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern s32 rpg_bat_target_no[];
extern s32 rpg_bat_tgt_x;
extern s32 rpg_bat_tgt_y;
extern s32 rpg_bat_seq_work;
extern s32 rpg_bat_seq_wait_cnt;
extern s32 rpg_bat_seq_tween_from;
extern s32 rpg_bat_seq_tween_to;
extern s32 rpg_bat_seq_arc_base_y;
extern RPG_BAT_UNIT rpg_bat_unit0;
extern RPG_BAT_UNIT rpg_bat_unit1;

extern s32 rsin(s32);
extern void rpg_bat_anim_set2(s32, s32, s32, s32);
extern void rpg_bat_sprite_pos_set(s32, s32, s32);
extern void rpg_bat_se_set2(s32, s32, s32);
extern void rpg_bat_sprite_nudge(s32, s32);
extern void rpg_bat_step_next(void);

void rpg_bat_hud_tween(void)
{
    s32 i;
    s32 x;
    s32 h;
    s32 z;
    s32 r;

    if (rpg_bat_unit0.cnt >= 104 && rpg_bat_unit0.cnt < 1104) {
        if ((rpg_bat_unit0.cnt & 7) == 0) {
            rpg_bat_sprite_nudge(0, 1);
        } else {
            rpg_bat_sprite_nudge(0, -1);
        }
    }
    if (rpg_bat_unit0.cnt >= 304 && rpg_bat_unit0.cnt < 952) {
        if ((rpg_bat_unit0.cnt & 7) == 0) {
            rpg_bat_sprite_nudge(0, 1);
        } else {
            rpg_bat_sprite_nudge(0, -1);
        }
    }
    if (rpg_bat_unit0.cnt >= 504 && rpg_bat_unit0.cnt < 800) {
        if ((rpg_bat_unit0.cnt & 7) == 0) {
            rpg_bat_sprite_nudge(0, 1);
        } else {
            rpg_bat_sprite_nudge(0, -1);
        }
    }
    rpg_bat_se_set2(1290, 17, 1);
    rpg_bat_sprite_pos_set(50,
        (rpg_bat_seq_wait_cnt - rpg_bat_seq_work) * rpg_bat_unit0.cnt / 1000 + rpg_bat_seq_work, 136);
    rpg_bat_sprite_pos_set(51,
        (rpg_bat_seq_wait_cnt - rpg_bat_seq_work) * rpg_bat_unit0.cnt / 1000 + rpg_bat_seq_work, 136);
    rpg_bat_sprite_pos_set(54,
        (rpg_bat_seq_tween_to - rpg_bat_seq_tween_from) * rpg_bat_unit0.cnt / 1000 + rpg_bat_seq_tween_from, 80);
    rpg_bat_sprite_pos_set(55,
        (rpg_bat_seq_tween_to - rpg_bat_seq_tween_from) * rpg_bat_unit0.cnt / 1000 + rpg_bat_seq_tween_from, 80);
    for (i = 0; i < 8; i++) {
        rpg_bat_sprite_pos_set(i + 40,
            rpg_bat_target_no[60 + i]
                + (rpg_bat_target_no[70 + i] - rpg_bat_target_no[60 + i]) * rpg_bat_unit0.cnt
                    / 1000, rpg_bat_target_no[80 + i]);
    }
    x = rpg_bat_target_no[67]
        + (rpg_bat_target_no[77] - rpg_bat_target_no[67]) * rpg_bat_unit0.cnt / 1000;
    h = rpg_bat_seq_arc_base_y;
    if (x >= rpg_bat_tgt_x && x <= h - rpg_bat_tgt_y + rpg_bat_tgt_x) {
        rpg_bat_se_set2(1292, 0, 3);
        rpg_bat_anim_set2(47, 4, 1, 1);
        if (rpg_bat_unit1.flag & 0x100000) {
            r = rsin(((x - rpg_bat_tgt_x) << 11) / (h - rpg_bat_tgt_y));
            rpg_bat_sprite_pos_set(47, x,
                ((h >> 0) - h) + rpg_bat_tgt_y + x - rpg_bat_tgt_x - (r >> 7));
        } else {
            r = rsin(((x - rpg_bat_tgt_x) << 11) / (h - rpg_bat_tgt_y));
            rpg_bat_sprite_pos_set(47, x,
                ((h >> 0) - h) + rpg_bat_tgt_y + x - rpg_bat_tgt_x - (r >> 6));
        }
    }
    if ((rpg_bat_tgt_x - (h - rpg_bat_tgt_y) * 2) < x && x < rpg_bat_tgt_x) {
        z = (h >> 0) - h;
        rpg_bat_se_set2(1291, 0, 2);
        rpg_bat_anim_set2(47, 4, 1, 1);
        if (rpg_bat_unit1.flag & 0x100000) {
            r = rsin(((rpg_bat_tgt_x - x) << 11)
                / ((h - rpg_bat_tgt_y) * 2));
            rpg_bat_sprite_pos_set(47, x,
                z + rpg_bat_tgt_y + (rpg_bat_tgt_x - x) / 2 - (r >> 7));
        } else {
            r = rsin(((rpg_bat_tgt_x - x) << 11)
                / ((h - rpg_bat_tgt_y) * 2));
            rpg_bat_sprite_pos_set(47, x,
                z + rpg_bat_tgt_y + (rpg_bat_tgt_x - x) / 2 - (r >> 6));
        }
    }
    if (x == rpg_bat_tgt_x - (h - rpg_bat_tgt_y) * 2) {
        rpg_bat_anim_set2(47, 5, 1, 1);
    }
    rpg_bat_unit0.cnt = rpg_bat_unit0.cnt + 4;
    if (rpg_bat_unit0.cnt >= 1201) {
        rpg_bat_step_next();
    }
}
