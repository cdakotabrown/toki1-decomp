/* @RodataBase: 0x8014C4BC */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:8 */
/* @SymbolStart: 0x8014200C */
/* @SymbolSize: 712 */
/* @PostProc: patch_insn_reg_field 0x10:rt:a0:v0 0x14:rs:a0:v0 0x14:rt:a0:v0 0x1c:rs:a0:v0 0x28:rt:a0:v0 0x90:rt:v0:v1 0x94:rs:v0:v1 0x94:rt:v0:v1 0x9c:rs:v0:v1 0xa8:rs:a0:v0 0x180:rt:v0:v1 0x184:rs:v0:v1 0x184:rt:v0:v1 0x18c:rs:v0:v1 0x194:rs:v0:v1 0x1d4:rs:a0:v0 0x2a8:rt:v0:v1 0x2ac:rs:v0:v1 0x2ac:rt:v0:v1 0x2b4:rs:v0:v1 */
/* name_ent_card_format - NAME_ENT overlay, Address: 0x8014200C, 712 bytes (178 insns) */
#include "types.h"

extern void text_sys_init(void);
extern void set_kanji_string(s32, s32, s32, s32, s32);
extern void k_disp_start(s32);
extern s32 get_pad_str(void);
extern s32 get_pad_str2(void);
extern s32 card_format_run(void);
extern s32 goto_sub_step(s32);

extern u8 step_mode;
extern s32 pad_trig;

extern u8 name_ent_card_format_str0[];
extern u8 name_ent_card_format_str1[];
extern u8 save_format_main_str6[];
extern u8 save_format_main_str7[];
extern u8 name_ent_card_format_str2[];
extern u8 name_ent_card_format_str3[];
extern u8 name_ent_card_format_str4[];
extern u8 name_ent_card_format_str5[];

s32 name_ent_card_format(void)
{
    u32 w;
    s32 flags;
    s32 r;

    switch (step_mode) {
    case 0:
        text_sys_init();
        set_kanji_string(-64, 32, 15, (s32)name_ent_card_format_str0, 0);
        k_disp_start(1);
        w = (u8)(step_mode + 1);
        step_mode = w;
        return w;
    case 1:
        if ((pad_trig & 0x860) == 0) {
            return;
        }
        w = (u8)(step_mode + 1);
        step_mode = w;
        return w;
    case 2:
        text_sys_init();
        set_kanji_string(-64, 32, 15, (s32)name_ent_card_format_str1, 0);
        set_kanji_string(-64, 48, 15, (s32)save_format_main_str6, 0);
        r = get_pad_str();
        set_kanji_string(20, 48, 1, r, 0);
        set_kanji_string(-64, 64, 15, (s32)save_format_main_str7, 0);
        r = get_pad_str2();
        set_kanji_string(20, 64, 2, r, 0);
        k_disp_start(5);
        w = (u8)(step_mode + 1);
        step_mode = w;
        return w;
    case 3:
        flags = pad_trig;
        if (flags & 0x20) {
            if (card_format_run() == 1) {
                return goto_sub_step(2);
            }
            return goto_sub_step(1);
        }
        if ((flags & 0x40) == 0) {
            return;
        }
        w = (u8)(step_mode + 1);
        step_mode = w;
        return w;
    case 4:
        text_sys_init();
        set_kanji_string(-64, 32, 15, (s32)name_ent_card_format_str2, 0);
        set_kanji_string(-64, 48, 1, (s32)name_ent_card_format_str3, 0);
        set_kanji_string(-64, 48, 15, (s32)name_ent_card_format_str4, 0);
        set_kanji_string(-64, 64, 15, (s32)name_ent_card_format_str5, 0);
        r = get_pad_str();
        set_kanji_string(-64, 64, 1, r, 0);
        k_disp_start(5);
        w = (u8)(step_mode + 1);
        step_mode = w;
        return w;
    case 5:
        if ((pad_trig & 0x20) == 0) {
            return;
        }
        return goto_sub_step(1);
    }
}
