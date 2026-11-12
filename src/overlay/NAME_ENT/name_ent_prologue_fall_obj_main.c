/* @PostProc: patch_insn_reg_field 0x24:rt:t7:t6 0x2c:rs:t7:t6 */
/* name_ent_prologue_fall_obj_main - NAME_ENT overlay, Address: 0x80145EB0, Size: 60 bytes (15 insns) */
#include "types.h"

extern void name_ent_fall_obj_move();
extern void name_ent_bg_change_step();

void name_ent_prologue_fall_obj_main(void)
{
    name_ent_fall_obj_move(1);
    if (*(u8 *)(0x800F0000 - 1942) == 0) {
        name_ent_bg_change_step();
    }
}
