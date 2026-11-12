/* @SymbolStart: 0x8014788C */
/* @SymbolSize: 184 */
/* @PostProc: patch_insn_reg_field 0x28:rd:t6:v0 0x2c:rd:t7:v0 0x2c:rs:t6:v0 0x30:rd:t8:v0 0x30:rt:t7:v0 0x38:rs:t8:v0 0x44:rt:t9:t6 0x48:rt:t0:t7 0x4c:rt:t0:t7 0x50:rt:t9:t6 0x74:rt:t1:t8 0x78:rt:t2:t9 0x7c:rt:t2:t9 0x80:rt:t1:t8 0x9c:rt:t3:t0 0xa0:rt:t3:t0 */
#include "types.h"

extern s32 name_ent_step_cnt;
extern void _sprite_set_box_shade();
extern void dtd_on_tpage();

void name_ent_fade_box_show(void)
{
    s32 v1_hidden;
    u32 color;

    v1_hidden = name_ent_step_cnt;
    if (v1_hidden < 256) {
        color = (v1_hidden << 8) + v1_hidden;
        color = (color << 8) + v1_hidden;
    } else {
        color = 0x00FFFFFF;
    }
    _sprite_set_box_shade(-160, -120, 160, 160, 11, color, 129);
    _sprite_set_box_shade(0, -120, 160, 160, 11, color, 129);
    dtd_on_tpage(0, 0, 11, 1, 1);
}
