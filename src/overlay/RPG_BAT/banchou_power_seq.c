/* @SymbolStart: 0x8015906C */
/* @RodataBase: 0x8015E2E8 */
/* banchou_power_seq -- RPG_BAT overlay, Address: 0x80159074, Size: 416 bytes */
/* @SymbolSize: 424 */
/* @PostProc: swap_instructions --offset1 0xf8 --offset2 0x104 */
/* @PostProc: patch_insn_reg_field 0x78:rt:a0:v0 0x7c:rs:a0:v0 0xbc:rt:a0:v0 0xc0:rs:a0:v0 0xf0:rt:a0:v0 0xf4:rs:a0:v0 0xf4:rt:a0:v0 0xfc:rs:a0:v0 0x130:rt:a0:v0 0x134:rs:a0:v0 0x134:rt:a0:v0 0x13c:rs:a0:v0 0x13c:rt:v1:v0 0x140:rt:v1:v0 0x144:rs:v1:v0 0x160:rt:t8:v0 0x164:rs:t8:v0 0x164:rt:t8:v0 0x16c:rd:t9:t8 0x16c:rs:t8:v0 0x178:rd:t0:t9 0x178:rs:t9:t8 0x184:rt:t0:t9 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
extern RPG_BAT_UNIT rpg_bat_unit1;


extern void banchou_resolve_talk(void);
extern void rpg_bat_anim_set(s32, s32, s32, s32);
extern void rpg_bat_seq_step_next(void);
extern void rpg_bat_se_set(s32, s32, s32);
extern void rpg_bat_sprite_pos_set(s32, s32, s32);
extern void rpg_bat_seq_wait(s32);
extern void rpg_bat_effect_clear(void);


void banchou_power_seq(void)
{
    s32 n;

    switch (rpg_bat_unit1.step) {
    case 0:
        banchou_resolve_talk();
        break;
    case 1:
        rpg_bat_anim_set(59, 6, 5, 1);
        rpg_bat_seq_step_next();
        break;
    case 2:
        n = rpg_bat_unit1.cnt;
        if (n == 34) {
            rpg_bat_se_set(1289, 1, 0);
            rpg_bat_sprite_pos_set(58, 0, 0);
            rpg_bat_anim_set(58, 7, 1, 1);
            n = rpg_bat_unit1.cnt;
        }
        n = n + 1;
        rpg_bat_unit1.cnt = n;
        if (n >= 43) {
            rpg_bat_seq_step_next();
        }
        break;
    case 3:
        if ((rpg_bat_unit1.cnt & 1) == 0) {
            rpg_bat_anim_set(58, 7, 1, 0);
        } else {
            rpg_bat_anim_set(58, 7, 1, 1);
        }
        n = rpg_bat_unit1.cnt + 1;
        rpg_bat_unit1.cnt = n;
        if (n >= 61) {
            rpg_bat_anim_set(59, 8, 1, 1);
            n = rpg_bat_unit1.flag | 0x400000;
            rpg_bat_unit1.flag = n & 0xFFDFFFFF;
            rpg_bat_seq_step_next();
        }
        break;
    case 4:
        rpg_bat_seq_wait(20);
        break;
    case 5:
        rpg_bat_effect_clear();
        break;
    }
}
