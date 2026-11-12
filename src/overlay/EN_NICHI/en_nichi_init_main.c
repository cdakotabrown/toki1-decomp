/* @SymbolStart: 0x8013216C */
/* @SymbolSize: 128 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 0x30:rs:v0:v1 0x38:rs:v0:v1 */
/* en_nichi_init_main - EN_NICHI overlay, Address: 0x8013216C, Size: 128 bytes (32 insns) */
#include "types.h"

extern u8 now_sub_sub_step;
extern void en_nichi_init0();
extern void en_nichi_cd_read();
extern void en_nichi_palette_load();
extern void sub_step_up();

void en_nichi_init_main(void)
{
    u8 step = now_sub_sub_step;

    switch (step) {
    case 0: en_nichi_init0(); break;
    case 1: en_nichi_cd_read(); break;
    case 2: en_nichi_palette_load(); break;
    case 3:
    default: sub_step_up(); break;
    }
}
