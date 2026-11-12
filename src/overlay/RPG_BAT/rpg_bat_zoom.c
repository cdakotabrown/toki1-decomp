/* @SymbolSize: 364 */
/* @PostProc: patch_insn_reg_field 0x58:rt:t6:t7 0x64:rd:a3:t8 0x64:rs:t6:t7 0xa4:rs:a3:t8 0x138:rt:t9:t0 0x140:rt:t9:t0 0x148:rt:t0:t1 0x14c:rt:t0:t1 0x154:rt:t1:t2 0x158:rt:t1:t2 0x160:rt:t2:t3 0x168:rt:t2:t3 */
/* rpg_bat_zoom - RPG_BAT overlay, Address: 0x80142544, Size: 364 bytes (91 insns). */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"

extern volatile s16 cw139_scale_x;
extern volatile s16 cw139_scale_y;
extern volatile s16 cw140_scale_x;
extern volatile s16 cw140_scale_y;
extern RPG_BAT_UNIT rpg_bat_unit0;

extern void rpg_bat_anim_set();
extern void rpg_bat_sprite_pos_set();
extern void rpg_bat_se_set2();
extern void rpg_bat_step_next();

void rpg_bat_zoom(void)
{
    s32 val;
    s32 rem;
    s32 scaled;
    s32 product;

    rpg_bat_se_set2(1289, 0, 1);
    rpg_bat_anim_set(43, 18, 1, 1);
    rpg_bat_anim_set(44, 19, 1, 1);

    val = rpg_bat_unit0.cnt;
    rem = 10 - val;
    if (val < 200) {
        scaled = (val << 12) / 30;
        cw139_scale_x = (s16)scaled;
        cw139_scale_y = (s16)scaled;
        cw140_scale_x = (s16)scaled;
        cw140_scale_y = (s16)scaled;
    }
    if (val >= 10) {
        product = rem * rpg_bat_unit0.cnt;
        rpg_bat_unit0.cnt = rpg_bat_unit0.cnt;
        rpg_bat_sprite_pos_set(43, 0, product / 100);
        rpg_bat_sprite_pos_set(44, 0, rpg_bat_unit0.cnt - 10);
    }
    rpg_bat_unit0.cnt = rpg_bat_unit0.cnt + 1;
    if (rpg_bat_unit0.cnt >= 129) {
        rpg_bat_anim_set(43, 255, 1, 0);
        rpg_bat_anim_set(44, 255, 1, 0);
        cw139_scale_x = 4096;
        cw139_scale_y = 4096;
        cw140_scale_x = 4096;
        cw140_scale_y = 4096;
        rpg_bat_step_next();
    }
}
