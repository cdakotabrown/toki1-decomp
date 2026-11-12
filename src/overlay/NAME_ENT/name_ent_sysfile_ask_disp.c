/* name_ent_sysfile_ask_disp - NAME_ENT overlay, Address: 0x8013DE10, Size: 196 bytes (49 insns) */
#include "types.h"

extern u8 save_format_main_str6[];
extern u8 save_format_main_str7[];
extern u8 name_ent_disp48_str0[];

extern void text_sys_init();
extern void set_kanji_string();
extern s32 get_pad_str();
extern s32 get_pad_str2();
extern void k_disp_start();
extern void sub_sub_step_up();

void name_ent_sysfile_ask_disp(void)
{
    s32 v0;

    text_sys_init();

    set_kanji_string(-64, 32, 15, (void *)name_ent_disp48_str0, 0);
    set_kanji_string(-64, 48, 15, (void *)save_format_main_str6, 0);

    v0 = get_pad_str();
    set_kanji_string(20, 48, 1, (void *)v0, 0);

    set_kanji_string(-64, 64, 15, (void *)save_format_main_str7, 0);

    v0 = get_pad_str2();
    set_kanji_string(20, 64, 2, (void *)v0, 0);

    k_disp_start(5);
    sub_sub_step_up();
}
