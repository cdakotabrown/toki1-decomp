/* @SymbolStart: 0x801427E4 */
/* @RodataBase: 0x8015C38C */
/* @SymbolSize: 588 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: swap_instructions --offset1 0x1a8 --offset2 0x1ac */
/* @PostProc: patch_insn_reg_field 0x140:rt:v1:v0 0x144:rs:v1:v0 0x144:rt:v1:v0 0x14c:rs:v1:v0 0x14c:rt:v1:v0 0x150:rt:v1:v0 0x158:rt:v1:v0 0x16c:rt:t2:v0 0x170:rs:t2:v0 0x170:rt:t2:v0 0x178:rs:t2:v0 0x1a0:rt:v1:v0 0x1a4:rs:v1:v0 0x1a4:rt:v1:v0 0x1a8:rt:t4:t3 0x1ac:rs:v1:v0 0x1ac:rt:v1:v0 0x1b0:rd:t3:t2 0x1b0:rt:v1:v0 0x1b8:rt:v1:v0 0x1bc:rs:t4:t3 0x1bc:rt:t3:t2 0x1cc:rt:t5:v0 0x1d0:rs:t5:v0 0x1d0:rt:t5:v0 0x1d8:rs:t5:v0 0x214:rt:v1:v0 0x218:rs:v1:v0 0x218:rt:v1:v0 0x220:rs:v1:v0 0x220:rt:v1:v0 0x224:rt:v1:v0 0x228:rs:v1:v0 */
/* rpg_bat_step0d - RPG_BAT overlay, Address: 0x801427EC, Size: 580 bytes (145 insns) */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"


extern s32 rpg_bat_anim_set();
extern s32 rpg_bat_anim_set2();
extern s32 rpg_bat_sprite_pos_set();
extern s32 rpg_bat_se_set2();
extern s32 rpg_bat_step_next();
extern s32 rpg_bat_state_next();

extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;

void rpg_bat_step0d(void)
{
    s32 v0;

    switch ((u32)rpg_bat_unit0.sub) {
    case 0:
        rpg_bat_anim_set(43, 8, 5, 1);
        rpg_bat_state_next();
        break;
    case 1:
        if ((class_w[139].anim_flag & 1) != 0)
            return;
        rpg_bat_state_next();
        break;
    case 2:
        rpg_bat_anim_set(43, 7, 1, 1);
        rpg_bat_sprite_pos_set(42, 0, 0);
        rpg_bat_anim_set(42, 9, 5, 1);
        rpg_bat_state_next();
        break;
    case 3:
        if (class_w[138].frame != 7)
            return;
        rpg_bat_state_next();
        break;
    case 4:
        rpg_bat_sprite_pos_set(42, 210, 72);
        rpg_bat_anim_set2(42, 10, 1, 1);
        rpg_bat_se_set2(1284, 1, 4);
        rpg_bat_state_next();
        break;
    case 5:
        v0 = rpg_bat_unit0.cnt + 1;
        rpg_bat_unit0.cnt = v0;
        rpg_bat_sprite_pos_set(42, 210 - (v0 << 3), 72);
        if (rpg_bat_unit0.cnt < 49)
            return;
        rpg_bat_anim_set2(42, 22, 1, 1);
        rpg_bat_state_next();
        break;
    case 6:
        v0 = rpg_bat_unit0.cnt - 1;
        rpg_bat_unit0.cnt = v0;
        rpg_bat_sprite_pos_set(42, 210 - (v0 << 3), 72);
        if (rpg_bat_unit0.cnt >= -4)
            return;
        rpg_bat_anim_set(43, 8, 1, 1);
        rpg_bat_anim_set2(42, 255, 1, 0);
        rpg_bat_state_next();
        break;
    case 7:
        v0 = rpg_bat_unit0.wait + 1;
        rpg_bat_unit0.wait = v0;
        if (v0 < 9)
            return;
        rpg_bat_anim_set(43, 6, 1, 1);
        rpg_bat_step_next();
        break;
    }
}
