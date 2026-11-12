/* @PostProc: patch_insn_reg_field 0x44:rt:t9:t0 0x4c:rt:t9:t0 0x54:rt:t0:t1 0x5c:rt:t0:t1 */
/* Address: 0x8013F260 */
/* Size: 80 bytes (20 instructions) */
#include "types.h"

extern s32 shougatu_menu_res[];
extern volatile s32 shougatu_menu_res_2;
extern volatile s16 shougatu_menu_res_3;
extern volatile s16 shougatu_menu_res_aux_src;

void shougatu_bank_init2(void)
{
    shougatu_menu_res[0] = 0x801D6360;
    shougatu_menu_res[1] = 0x801D6384;
    shougatu_menu_res_2 = 0x801D6414;
    shougatu_menu_res_3 = shougatu_menu_res_aux_src;
    shougatu_menu_res[4] = 0x801B0000;
}
