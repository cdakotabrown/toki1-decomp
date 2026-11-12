/* @SymbolSize: 148 */
/* name_ent_album_loading_disp - NAME_ENT overlay, Address: 0x8013D7B8, Size: 148 bytes (37 insns) */
#include "types.h"

extern void text_sys_init();
extern void strcpy();
extern void strcat();
extern void set_kanji_string();
extern void k_disp_start();
extern void sub_sub_step_up();

extern u8 now_album;
extern void *album_info_disp_tbl0[];
extern u8 name_ent_disp22_str0[];
extern u8 name_ent_disp22_str1[];

void name_ent_album_loading_disp(void)
{
    u8 buf[260];

    text_sys_init();
    strcpy(buf, album_info_disp_tbl0[now_album]);
    strcat(buf, name_ent_disp22_str0);
    set_kanji_string(-64, 32, 15, buf, 0);
    set_kanji_string(-64, 48, 15, name_ent_disp22_str1, 0);
    k_disp_start(2);
    sub_sub_step_up();
}
