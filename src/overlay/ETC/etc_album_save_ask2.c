/* @SymbolStart: 0x80141590 */
/* Address: 0x80141598 */
/* Size: 340 bytes (85 instructions) */
/* @SymbolSize: 348 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rs:v0:v1 0x2c:rt:t8:t7 0x120:rt:t6:v1 0x124:rs:t6:v1 0x124:rt:t6:v1 0x12c:rs:t6:v1 0x12c:rt:t7:t6 0x134:rt:t7:t6 0x138:rs:t8:t7 0x138:rt:t8:t7 0x140:rs:t8:t7 */
#include "types.h"

extern void sub_sub_step_up(void);
extern void set_kanji_string(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern void k_sub_reset_point_set(void);
extern void k_sub_disp_start(s32 a0);
extern void dec_bg_show(s32 a0);
extern void printf(s32 a0);
extern s32 get_etc_yes_button(void);
extern s32 get_etc_no_button(void);
extern u8 step_mode;
extern s32 step_counter;
extern u8 etc_album_save_ask2_str0[];
extern u8 etc_album_save_ask2_str1[];
extern u8 etc_album_save_ask2_str2[];
extern u8 etc_yes_str[];
extern u8 etc_no_str[];
extern u8 etc_album_save_ask2_str3[];

void etc_album_save_ask2(void)
{
    s32 v1 = step_mode;

    s32 r;

    switch (v1) {
    case 0:
        k_sub_reset_point_set();
        set_kanji_string(-120, 48, 0, (s32)etc_album_save_ask2_str0, 0);
        set_kanji_string(-120, 62, 0, (s32)etc_album_save_ask2_str1, 0);
        set_kanji_string(-120, 76, 0, (s32)etc_album_save_ask2_str2, 0);
        set_kanji_string(-136, 90, 0, (s32)etc_yes_str, 0);
        r = get_etc_yes_button();
        set_kanji_string(-52, 90, 1, r, 0);
        set_kanji_string(8, 90, 0, (s32)etc_no_str, 0);
        r = get_etc_no_button();
        set_kanji_string(92, 90, 2, r, 0);
        k_sub_disp_start(7);
        printf((s32)etc_album_save_ask2_str3);
        step_mode++;
        break;
    case 1:
        if ((u32)step_counter < 65) {
            break;
        }
        sub_sub_step_up();
        break;
    }

    dec_bg_show(2);
}
