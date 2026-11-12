/* @SymbolStart: 0x80141154 */
/* @RodataBase: 0x8014D06C */
/* @PostProc: scheduler_normalizer */
/* @PostProc: patch_insn_reg_field 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rt:v0:v1 0x74:rs:t7:v1 0x74:rt:t7:v1 0x7c:rs:t7:v1 0x7c:rt:t8:t7 0x8c:rs:v1:v0 0x8c:rt:v1:v0 0x90:rs:v0:v1 0x90:rt:t0:t9 0x94:rs:v1:v0 0x94:rt:t9:t8 0xa4:rt:t8:t9 0x150:rs:t1:v1 0x150:rt:t1:v1 0x158:rs:t1:v1 0x158:rt:t2:t0 0x164:rt:t1:v0 0x168:rs:v1:v0 0x168:rt:v1:v0 0x170:rs:v1:v0 0x170:rt:t3:t1 0x174:rs:t3:t1 0x178:rs:v1:v0 0x178:rt:t4:t2 0x1b0:rs:t4:t2 0x1b4:rs:v0:v1 0x1b4:rt:t5:t3 0x1c0:rt:t5:t3 0x25c:rt:t6:v1 0x260:rs:t6:v1 0x260:rt:t6:v1 0x268:rs:t6:v1 0x268:rt:t7:t4 0x26c:rt:t2:t4 0x280:rt:t7:v0 0x284:rs:v1:v0 0x284:rt:v1:v0 0x28c:rs:v1:v0 0x28c:rt:t8:t5 0x290:rs:t9:t5 */
/* @SymbolSize: 680 */
#include "types.h"

extern u8 step_mode;
extern s32 pad_trig;

extern u8 etc_card_format_ask_str0[];
extern u8 etc_card_format_ask_str1[];
extern u8 etc_yes_str[];
extern u8 etc_no_str[];
extern u8 etc_card_format_ask_str2[];
extern u8 etc_card_format_ask_str3[];
extern u8 etc_card_format_ask_str4[];
extern u8 etc_card_format_ask_str5[];

extern void text_sys_init(void);
extern void set_kanji_string(s32 x, s32 y, s32 w, u8 *str, s32 f);
extern void k_disp_start(s32 arg);
extern s32 card_format_run(void);
extern void goto_sub_sub_step(s32 arg);
extern s32 get_etc_yes_button(void);
extern s32 get_etc_no_button(void);
extern void dec_bg_show(s32 arg);

void etc_card_format_ask(void)
{
    s32 v0;

    switch (step_mode) {
    case 0:
        text_sys_init();
        set_kanji_string(-120, 48, 15, etc_card_format_ask_str0, 0);
        k_disp_start(1);
        step_mode = step_mode + 1;
        break;
    case 1:
        {
            s32 f1 = pad_trig;
            if (f1 & 0x860) {
                step_mode = step_mode + 1;
            }
        }
        break;
    case 2:
        text_sys_init();
        set_kanji_string(-120, 48, 15, etc_card_format_ask_str1, 0);
        set_kanji_string(-136, 90, 0, etc_yes_str, 0);
        set_kanji_string(-52, 90, 1, (u8 *)get_etc_yes_button(), 0);
        set_kanji_string(8, 90, 0, etc_no_str, 0);
        set_kanji_string(92, 90, 2, (u8 *)get_etc_no_button(), 0);
        k_disp_start(5);
        step_mode = step_mode + 1;
        break;
    case 3:
        v0 = pad_trig;
        if (v0 & 0x20) {
            if (card_format_run() == 1) {
                goto_sub_sub_step(4);
            } else {
                goto_sub_sub_step(2);
            }
        } else if (v0 & 0x40) {
            step_mode = step_mode + 1;
        }
        break;
    case 4:
        text_sys_init();
        set_kanji_string(-120, 48, 15, etc_card_format_ask_str2, 0);
        set_kanji_string(-120, 62, 1, etc_card_format_ask_str3, 0);
        set_kanji_string(-120, 62, 15, etc_card_format_ask_str4, 0);
        set_kanji_string(-120, 76, 15, etc_card_format_ask_str5, 0);
        set_kanji_string(-52, 76, 1, (u8 *)get_etc_yes_button(), 0);
        step_mode = step_mode + 1;
        k_disp_start(5);
        break;
    case 5:
        {
            s32 f5 = pad_trig;
            if (f5 & 0x20) {
                goto_sub_sub_step(4);
            }
        }
        break;
    }
    dec_bg_show(2);
}
