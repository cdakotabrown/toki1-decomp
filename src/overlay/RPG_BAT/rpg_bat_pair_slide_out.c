/* @SymbolStart: 0x80132CE4 */
/* @SymbolSize: 260 */
/* @PostProc: patch_insn_reg_field 0x50:rt:t8:v0 0x54:rs:t8:v0 0x54:rt:t8:v0 0x58:rt:t9:t8 0x68:rs:t9:t8 0x68:rt:t8:v0 0x9c:rt:t0:t9 0xac:rs:t0:t9 0xb0:rt:t1:v0 0xb4:rs:t1:v0 0xb4:rt:t1:v0 0xb8:rt:t2:t0 0xc8:rs:t2:t0 0xc8:rt:t1:v0 0xd4:rt:t3:t1 0xdc:rt:t3:t1 0xf4:rt:t4:t2 0x100:rt:t4:t2 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern volatile u8 cw155_prio;
extern volatile u8 cw148_rate;
extern RPG_BAT_UNIT rpg_bat_unit0;

extern s32 rpg_bat_anim_set2();
extern s32 rpg_bat_sprite_pos_set();
extern s32 rpg_bat_step_next();

void rpg_bat_pair_slide_out(void)
{
    s32 next;

    if (rpg_bat_unit0.cnt == 0) {
        cw155_prio = 4;
        rpg_bat_unit0.cnt += 4;
        rpg_bat_sprite_pos_set(52, 240 - rpg_bat_unit0.cnt, 88);
        rpg_bat_sprite_pos_set(51, 240 - rpg_bat_unit0.cnt, 88);
        rpg_bat_anim_set2(52, 3, 1, 1);
        rpg_bat_anim_set2(51, 4, 1, 1);
    }
    rpg_bat_sprite_pos_set(52, 240 - rpg_bat_unit0.cnt, 88);
    rpg_bat_sprite_pos_set(51, 240 - rpg_bat_unit0.cnt, 88);
    next = rpg_bat_unit0.cnt;
    cw148_rate = 130;
    next += 6;
    rpg_bat_unit0.cnt = next;
    if (next >= 336) {
        cw155_prio = 5;
        rpg_bat_step_next();
    }
}
