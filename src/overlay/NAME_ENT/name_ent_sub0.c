/* @SymbolStart: 0x8013C634 */
/* @SymbolSize: 104 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 0x30:rs:v0:v1 */
/* name_ent_sub0 - NAME_ENT overlay, Address: 0x8013C63C, Size: 96 bytes (24 insns) */
#include "types.h"

extern void name_ent_scene_init();
extern void name_ent_vram_load();
extern void name_ent_grid_init();
extern u8 now_sub_sub_step;

void name_ent_sub0(void)
{
    switch (now_sub_sub_step) {
        case 0: name_ent_scene_init(); break;
        case 1: name_ent_vram_load(); break;
        case 2: name_ent_grid_init(); break;
    }
}
