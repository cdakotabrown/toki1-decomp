/* @SymbolStart: 0x801459B0 */
/* @SymbolSize: 80 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 */
/* name_ent_sub_sub_dispatch - NAME_ENT overlay, Size: 72 bytes (18 insns) */
#include "types.h"

extern void name_ent_fall_obj_load(void);
extern void name_ent_bg_scroll_step(void);
extern u8 now_sub_sub_step;

void name_ent_sub_sub_dispatch(void)
{
    switch (now_sub_sub_step) {
    case 0:
        name_ent_fall_obj_load();
        break;
    case 1:
        name_ent_bg_scroll_step();
        break;
    }
}
