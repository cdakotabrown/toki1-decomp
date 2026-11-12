/* @SymbolStart: 0x8013D290 */
/* @SymbolSize: 192 */
/* @PostProc: swap_registers a0:v0 */
/* @PostProc: patch_insn_reg_field 0x88:rt:v0:v1 0x98:rt:v0:v1 */
#include "types.h"
#include "tokimeki/rpg_bat_unit.h"
#include "tokimeki/class_w.h"

extern s32 rpg_bat_girl;
extern CLASS_W class_w[];
extern RPG_BAT_UNIT rpg_bat_unit0;

extern s32 rpg_bat_step_next();

void rpg_bat_blink_step2(void)
{
    s32 mask;
    s32 newmode;

    if ((rpg_bat_unit0.cnt & 1) == 0) {
        class_w[148].disp_flag |= 0x80;
        if (rpg_bat_girl != 14) {
            class_w[125].disp_flag |= 0x80;
        }
    } else {
        mask = -129;
        class_w[148].disp_flag = class_w[148].disp_flag & mask;
        if (rpg_bat_girl != 14) {
            class_w[125].disp_flag = class_w[125].disp_flag & mask;
        }
    }
    newmode = rpg_bat_unit0.cnt + 1;
    rpg_bat_unit0.cnt = newmode;
    if (newmode >= 61) {
        rpg_bat_step_next();
    }
}
