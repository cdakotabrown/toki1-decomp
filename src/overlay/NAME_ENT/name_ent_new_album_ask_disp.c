/* @SymbolSize: 224 */
/* name_ent_new_album_ask_disp - NAME_ENT overlay, Address: 0x8013DA00, Size: 224 bytes (56 insns) */
#include "types.h"

extern u8 name_ent_disp40_str0[];
extern u8 name_ent_disp40_str1[];
extern u8 name_ent_disp40_str2[];
extern u8 name_ent_disp40_str3[];

extern void text_sys_init();
extern void set_kanji_string();
extern s32 get_pad_str();
extern s32 get_pad_str2();
extern void k_disp_start();
extern void sub_sub_step_up();

void name_ent_new_album_ask_disp(void)
{
    s32 v0;

    text_sys_init();

    set_kanji_string(-64, 32, 15, (void *)name_ent_disp40_str0, 0);
    set_kanji_string(-64, 48, 15, (void *)name_ent_disp40_str1, 0);
    set_kanji_string(-64, 64, 15, (void *)name_ent_disp40_str2, 0);
    set_kanji_string(-64, 80, 15, (void *)name_ent_disp40_str3, 0);

    v0 = get_pad_str();
    set_kanji_string(6, 80, 1, (void *)v0, 0);

    v0 = get_pad_str2();
    set_kanji_string(90, 80, 2, (void *)v0, 0);

    k_disp_start(6);
    sub_sub_step_up();
}
