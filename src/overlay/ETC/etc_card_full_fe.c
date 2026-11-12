/* @SymbolStart: 0x8014237C */
/* Address: 0x80142384 */
/* Size: 476 bytes (119 instructions) */
/* @SymbolSize: 484 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=2:v1 */
#include "types.h"

extern void goto_step(s32 a0);
extern void goto_sub_sub_step(s32 a0);
extern void device_state_init(void);
extern void set_kanji_string(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);
extern void k_sub_reset(void);
extern void k_sub_disp_start(s32 a0);
extern void dec_bg_show(s32 a0);
extern s32 get_etc_yes_button(void);
extern s32 get_etc_no_button(void);
extern s32 pad_trig;
extern u8 step_mode;
extern s32 step_counter;

extern u8 etc_card_full_fe_str0[];
extern u8 etc_card_full_fe_str1[];
extern u8 etc_card_full_fe_str2[];
extern u8 etc_card_full_fe_str3[];
extern u8 etc_yes_str[];
extern u8 etc_no_str[];

void etc_card_full_fe(void)
{
    s32 v0;
    s32 r;

    switch ((u32)step_mode) {
    case 0:
        k_sub_reset();
        set_kanji_string(-120, 48, 0, (s32)etc_card_full_fe_str0, 0);
        set_kanji_string(-120, 76, 0, (s32)etc_card_full_fe_str1, 0);
        set_kanji_string(-120, 90, 0, (s32)etc_card_full_fe_str2, 0);
        k_sub_disp_start(3);
        step_mode++;
        break;
    case 1:
        if ((u32)step_counter >= 65) {
            step_mode = step_mode + 1;
        }
        break;
    case 2:
        k_sub_reset();
        set_kanji_string(-120, 48, 0, (s32)etc_card_full_fe_str3, 0);
        set_kanji_string(-136, 90, 0, (s32)etc_yes_str, 0);
        r = get_etc_yes_button();
        set_kanji_string(-52, 90, 1, r, 0);
        set_kanji_string(8, 90, 0, (s32)etc_no_str, 0);
        r = get_etc_no_button();
        set_kanji_string(92, 90, 2, r, 0);
        k_sub_disp_start(5);
        step_mode++;
        break;
    case 3:
        v0 = pad_trig;
        if (v0 & 0x20) {
            goto_sub_sub_step(4);
        } else if (v0 & 0x40) {
            device_state_init();
            goto_step(196);
        }
        break;
    }

    dec_bg_show(2);
}
