/* @PostProc: patch_insn_reg_field   0x10:rt:a0:v1 0x14:rs:a0:v1 0x14:rt:a0:v1 0x28:rs:a0:v1 0x34:rt:a0:v1 0x78:rt:a0:v1 0x7c:rs:a0:v1 0x7c:rt:a0:v1 0x84:rs:a0:v1 0x84:rt:a0:v1 0x8c:rt:a0:v1 0xac:rs:a0:v1 0xac:rt:a0:v1 0xb8:rt:a0:v1 0x104:rt:a0:v1 0x108:rs:a0:v1 0x108:rt:a0:v1 0x110:rs:a0:v1 0x110:rt:a0:v1 0x118:rt:a0:v1 0x138:rs:a0:v1 0x138:rt:a0:v1 0x144:rt:a0:v1 */
/* @SymbolStart: 0x8013673C */
/* @RodataBase: 0x8015AFF4 */
/* @SymbolSize: 360 */
#include "types.h"
#include "tokimeki/class_w.h"

extern void rpg_bat_anim_set2(s32 a0, s32 a1, s32 a2, s32 a3);
extern void rpg_bat_anim_set(s32 a0, s32 a1, s32 a2, s32 a3);
extern void rpg_bat_sprite_pos_set(s32 a0, s32 a1, s32 a2);
extern void rpg_bat_se_set2(s32 a0, s32 a1, s32 a2);
extern void rpg_bat_step_next(void);

extern s32 rpg_bat_seq_work;
extern s32 rpg_bat_seq_wait_cnt;
extern CLASS_W class_w[];

void rpg_bat_himo_anim_step(void)
{
    s32 n1;
    s32 n3;

    class_w[146].rate = 130;

switch (rpg_bat_seq_work) {
    case 0:
        rpg_bat_anim_set2(50, 6, 5, 1);
        rpg_bat_anim_set(51, 8, 1, 0);
        rpg_bat_seq_work = rpg_bat_seq_work + 1;
        break;
    case 1:
        n1 = rpg_bat_seq_wait_cnt + 1;
        rpg_bat_seq_wait_cnt = n1;
        if (n1 >= 7) {
            rpg_bat_seq_work++;
        }
        break;
    case 2:
        rpg_bat_anim_set(52, 12, 1, 1);
        rpg_bat_sprite_pos_set(51, 0, 0);
        rpg_bat_anim_set(51, 8, 1, 1);
        rpg_bat_se_set2(1299, 1, 5);
        rpg_bat_seq_work = rpg_bat_seq_work + 1;
        break;
    case 3:
        n3 = rpg_bat_seq_wait_cnt + 1;
        rpg_bat_seq_wait_cnt = n3;
        if (n3 >= 10) {
            rpg_bat_seq_work = rpg_bat_seq_work + 1;
        }
        break;
    case 4:
        class_w[146].rate = 1;
        rpg_bat_seq_wait_cnt = 0;
        rpg_bat_seq_work = 0;
        rpg_bat_step_next();
        break;
    }
}
