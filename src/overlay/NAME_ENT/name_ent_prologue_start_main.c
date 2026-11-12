/* @SymbolStart: 0x80145390 */
/* @SymbolSize: 120 */
/* @PostProc: swap_registers a0:v1 --unidirectional --exclude-offsets 0x6c */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 0x30:rs:v0:v1 0x74:rt:v1:a0 */
/* name_ent_prologue_start_main - NAME_ENT overlay, Address: 0x80145398, Size: 112 bytes (28 insns) */
#include "types.h"

extern void name_ent_bgm_load();
extern void name_ent_prologue_init();
extern void name_ent_main0_2();
extern void hizuke_show();
extern void dec_bg_show();
extern u8 now_sub_sub_step;

void name_ent_prologue_start_main(void)
{
    switch (now_sub_sub_step) {
        case 0: name_ent_bgm_load(); break;
        case 1: name_ent_prologue_init(); break;
        case 2: name_ent_main0_2(); break;
    }

    hizuke_show();
    dec_bg_show(1);
}
