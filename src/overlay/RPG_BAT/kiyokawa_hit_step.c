/* @SymbolStart: 0x80150CD8 */
/* @SymbolSize: 264 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x24:rt:v1:v0 0x28:rs:v0:v1 0x28:rt:v1:v0 0x34:rs:v0:v1 0x58:rt:v0:v1 0x5c:rs:v0:v1 0x5c:rt:v0:v1 0x64:rs:v0:v1 0x64:rt:v0:v1 0x6c:rt:v0:v1 0x78:rs:t6:v0 0x78:rt:v1:t6 0xac:rt:v0:v1 0xb0:rs:v0:v1 0xb0:rt:v0:v1 0xb8:rs:v0:v1 0xb8:rt:v0:v1 0xc0:rt:v0:v1 0xf4:rt:v0:v1 0xf8:rs:v0:v1 0xf8:rt:v0:v1 0x100:rs:v0:v1 0x100:rt:v0:v1 0x104:rt:v0:v1 */
#include "types.h"

extern void rpg_bat_anim_set(s32 a0, s32 a1, s32 a2, s32 a3);
extern void rpg_bat_anim_set2(s32 a0, s32 a1, s32 a2, s32 a3);
extern void rpg_bat_sprite_pos_set(s32 a0, s32 a1, s32 a2);
extern void rpg_bat_se_set2(s32 a0, s32 a1, s32 a2);

extern s32 rpg_bat_seq_work;
extern s32 rpg_bat_seq_wait_cnt;
extern s16 cw146_frame;

void kiyokawa_hit_step(void)
{
    switch (rpg_bat_seq_work) {
    case 0:
        rpg_bat_anim_set2(50, 7, 5, 1);
        rpg_bat_seq_work = rpg_bat_seq_work + 1;
        break;
    case 1:
        if (cw146_frame == 1) {
            rpg_bat_sprite_pos_set(51, 0, 0);
            rpg_bat_anim_set(51, 8, 1, 1);
            rpg_bat_se_set2(1298, 0, 5);
            rpg_bat_seq_work = rpg_bat_seq_work + 1;
            break;
        }

    case 2:
        rpg_bat_seq_wait_cnt = rpg_bat_seq_wait_cnt + 1;
        if (rpg_bat_seq_wait_cnt >= 31) {
            rpg_bat_anim_set(51, 8, 1, 0);
            rpg_bat_seq_work = rpg_bat_seq_work + 1;
        }
        break;
    }
}
