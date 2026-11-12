/* @SymbolSize: 76 */
/* name_ent_file_error_disp -- NAME_ENT overlay, Address: 0x80140E6C, Size: 76 bytes (19 insns) */
#include "types.h"

extern void text_sys_init(void);
extern void set_kanji_string(s32 a0, s32 a1, s32 a2, void *a3, s32 a4);
extern void k_disp_start(s32 a0);
extern void sub_sub_step_up(void);

extern u8 name_ent_disp0_str0;

void name_ent_file_error_disp(void)
{
    text_sys_init();
    set_kanji_string(-64, 32, 15, &name_ent_disp0_str0, 0);
    k_disp_start(2);
    sub_sub_step_up();
}
