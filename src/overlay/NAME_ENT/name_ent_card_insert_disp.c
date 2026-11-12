/* name_ent_card_insert_disp - NAME_ENT overlay, Address: 0x8013C9C0, Size: 200 bytes (50 insns) */
#include "types.h"

extern void text_sys_init();
extern void set_kanji_string();
extern s32 get_pad_str();
extern void card_state_clear();
extern void k_disp_start();
extern void sub_sub_step_up();

extern u8 name_ent_disp6_str0[];
extern u8 name_ent_disp6_str1[];
extern u8 name_ent_disp6_str2[];
extern u8 name_ent_disp6_str3[];

void name_ent_card_insert_disp(void)
{
    s32 v0;

    text_sys_init();

    set_kanji_string(-64, 32, 15, (void *)name_ent_disp6_str0, 0);
    set_kanji_string(-64, 48,  1, (void *)name_ent_disp6_str1, 0);
    set_kanji_string(-64, 48, 15, (void *)name_ent_disp6_str2, 0);
    set_kanji_string(-64, 64, 15, (void *)name_ent_disp6_str3, 0);

    v0 = get_pad_str();
    set_kanji_string(-64, 64, 1, (void *)v0, 0);

    card_state_clear();
    k_disp_start(5);
    sub_sub_step_up();
}
