/* @SymbolSize: 268 */
/* @PostProc: patch_stack_complete --old 296 --new 312 */
/* @PostProc: remap_stack_slots --from 40 --to 52 */
/* name_ent_dispe2 - NAME_ENT overlay, Address: 0x8013FE2C, Size: 268 bytes (67 insns) */
#include "types.h"

extern u8 now_album;
extern u32 album_info_disp_tbl0[];
extern u8 name_ent_dispe2_str0[];
extern u8 name_ent_dispe2_str1[];
extern u8 save_format_main_str6[];
extern u8 save_format_main_str7[];

extern void text_sys_init(void);
extern void strcpy(u8 *, u8 *);
extern void strcat(u8 *, u8 *);
extern void set_kanji_string(s32, s32, s32, u8 *, s32);
extern u8 *get_pad_str(void);
extern u8 *get_pad_str2(void);
extern void k_disp_start(s32);
extern void sub_sub_step_up(void);

void name_ent_dispe2(void)
{
    u8 textBuf[256];
    u8 idx;
    u8 *str;

    text_sys_init();

    idx = now_album;
    strcpy(textBuf, (u8 *)album_info_disp_tbl0[idx]);
    strcat(textBuf, name_ent_dispe2_str0);

    set_kanji_string(-64, 32, 15, textBuf, 0);
    set_kanji_string(-64, 48, 15, name_ent_dispe2_str1, 0);
    set_kanji_string(-64, 64, 15, save_format_main_str6, 0);

    str = get_pad_str();
    set_kanji_string(20, 64, 1, str, 0);

    set_kanji_string(-64, 80, 15, save_format_main_str7, 0);

    str = get_pad_str2();
    set_kanji_string(20, 80, 2, str, 0);

    k_disp_start(6);
    sub_sub_step_up();
}
