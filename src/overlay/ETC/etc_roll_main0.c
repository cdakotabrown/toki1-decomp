/* @SymbolStart: 0x801435C4 */
/* @SymbolSize: 88 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 */
/* Address: 0x801435CC */
/* Size: 80 bytes (20 instructions) */
#include "types.h"

extern void etc_sprite_fade(void);
extern void etc_roll_girl_load(void);
extern void etc_init1(void);

extern u8 now_sub_sub_step;

void etc_roll_main0(void)
{
    s32 v1 = now_sub_sub_step;

    switch (v1) {
    case 0:
        etc_roll_girl_load();
        break;
    case 1:
        etc_init1();
        break;
    }
    etc_sprite_fade();
}
