/* @SymbolStart: 0x80142A30 */
/* @SymbolSize: 648 */
/* @PostProc: scheduler_normalizer --block-aware --fence 24-35 */
/* @PostProc: patch_insn_reg_field 0x34:rt:t6:t4 0x40:rt:t3:t0 0xac:rt:t2:at 0xb0:rs:t2:at 0xc4:rt:t3:t2 0xc8:rs:t3:t2 0xc8:rt:t4:t2 0xd0:rs:t4:t2 0xd0:rt:t5:t3 0xd4:rs:t5:t3 0x12c:rs:t6:t4 0x12c:rt:t7:t4 0x134:rs:t7:t4 0x134:rt:t8:t5 0x138:rs:t8:t5 0x150:rt:v1:v0 0x154:rt:t9:t6 0x158:rs:v1:v0 0x158:rt:v1:v0 0x15c:rs:t9:t6 0x15c:rt:t9:t6 0x160:rt:v0:v1 0x164:rs:t9:t6 0x164:rt:v1:v0 0x168:rt:t1:t8 0x16c:rs:t1:t8 0x16c:rt:t1:t8 0x174:rd:t0:t7 0x180:rs:t0:t7 0x180:rt:v0:v1 0x184:rs:v0:v1 0x194:rs:v0:v1 0x19c:rs:t0:t7 0x1b4:rs:t1:t8 0x1b4:rt:v1:v0 0x1b8:rd:t2:t9 0x1c4:rs:t2:t9 0x1c4:rt:v0:v1 0x1c8:rs:v0:v1 0x1d8:rs:v0:v1 0x1e0:rs:t2:t9 0x24c:rs:t3:t0 0x24c:rt:t4:t0 0x254:rs:t4:t0 0x254:rt:t5:t1 0x258:rs:t5:t1 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_anim_set2();
extern s32 rpg_bat_sprite_pos_set();
extern s32 rpg_bat_se_set2();
extern s32 rpg_bat_step_next();
extern s32 rpg_bat_state_next();

extern s32 rpg_bat_target_x;
extern s32 rpg_bat_target_y;
extern s32 rpg_bat_move_dx;
extern s32 rpg_bat_move_dy;
extern s32 rpg_bat_target_no;
extern RPG_BAT_UNIT rpg_bat_unit0;

void rpg_bat_step10(void)
{
    s32 sel;
    s32 v0;
    s32 v1;
    s32 a1v;
    s32 a2v;

    sel = rpg_bat_unit0.sub;
    switch (sel) {
    case 0:
        rpg_bat_anim_set(43, 11, 5, 1);
        rpg_bat_se_set2(1282, 1, 2);
        rpg_bat_move_dx = rpg_bat_target_x - 240;
        rpg_bat_move_dy = rpg_bat_target_y - 88;
        if (rpg_bat_target_no == 1) {
            *(u8 *)0x80129ADD = 4;
        }
        rpg_bat_state_next();
        break;
    case 1:
        if ((*(u8 *)0x8012969E & 1) == 0) {
            rpg_bat_sprite_pos_set(42, 240, 88);
            rpg_bat_anim_set2(42, 12, 1, 1);
            rpg_bat_anim_set(43, 20, 5, 1);
            rpg_bat_se_set2(1286, 1, 6);
            rpg_bat_state_next();
        }
        break;
    case 2:
        if ((*(u8 *)0x8012969E & 1) == 0) {
            rpg_bat_anim_set(43, 6, 1, 1);
        }
        v0 = rpg_bat_unit0.cnt;
        v1 = 100;
        a1v = rpg_bat_move_dx * v0 / v1 + 240;
        a2v = rpg_bat_move_dy * v0 / v1 + 88;
        rpg_bat_sprite_pos_set(42, a1v, a2v);
        v0 = rpg_bat_unit0.cnt + 3;
        rpg_bat_unit0.cnt = v0;
        if (v0 >= 101) {
            rpg_bat_anim_set2(42, 14, 5, 1);
            rpg_bat_se_set2(1283, 1, 3);
            rpg_bat_state_next();
        }
        break;
    case 3:
        if ((*(u8 *)0x8012965A & 1) == 0) {
            rpg_bat_anim_set(43, 6, 1, 1);
            rpg_bat_anim_set2(42, 255, 1, 0);
            rpg_bat_step_next();
        }
        break;
    }
}
