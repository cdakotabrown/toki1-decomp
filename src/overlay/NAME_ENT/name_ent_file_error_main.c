/* @SymbolStart: 0x80141020 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 0x30:rs:v0:v1 0x38:rs:v0:v1 */
/* @SymbolSize: 128 */
/* name_ent_file_error_main - NAME_ENT overlay, Address: 0x80141028, 30 insns. */
#include "types.h"

extern void name_ent_file_error_disp(void);
extern void name_ent_file_error_select(void);
extern void name_ent_card_swap_disp(void);
extern void name_ent_card_swap_select(void);
extern u8 now_sub_sub_step;

void name_ent_file_error_main(void)
{
    switch (now_sub_sub_step) {
    case 0:
        name_ent_file_error_disp();
        break;
    case 1:
        name_ent_file_error_select();
        break;
    case 2:
        name_ent_card_swap_disp();
        break;
    case 3:
        name_ent_card_swap_select();
        break;
    }
}
