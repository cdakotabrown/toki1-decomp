/* @RodataBase: 0x8014D438 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:8 */
/* @SymbolStart: 0x801452AC */
/* @SymbolSize: 664 */
/* @PostProc: patch_insn_reg_field 0x10:rt:a0:v0 0x14:rs:a0:v0 0x14:rt:a0:v0 0x1c:rs:a0:v0 0x28:rt:a0:v0 0xa0:rt:v0:v1 0xa4:rs:v0:v1 0xa4:rt:v0:v1 0xac:rs:v0:v1 0xb8:rs:a0:v0 0x1ac:rt:v0:v1 0x1b0:rs:v0:v1 0x1b0:rt:v0:v1 0x1b8:rs:v0:v1 0x1c0:rs:v0:v1 0x1c4:rs:a0:v0 0x274:rt:v0:v1 0x278:rs:v0:v1 0x278:rt:v0:v1 0x280:rs:v0:v1 */
/* save_card_full - ETC overlay, Address: 0x801452AC, 664 bytes (166 insns) */
#include "types.h"

extern void card_state_clear(void);
extern void device_cmd_send(s32);
extern void k_sub_reset(void);
extern void set_kanji_string(s32, s32, s32, s32, s32);
extern void k_sub_disp_start(s32);
extern s32 get_etc_yes_button(void);
extern s32 get_etc_no_button(void);
extern s32 goto_sub_step(s32);
extern void card_step_init(void);
extern s32 goto_sub_sub_step(s32);

extern u8 step_mode;
extern s32 pad_trig;

extern u8 save_card_full_str0[];
extern u8 save_card_full_str1[];
extern u8 save_card_full_str2[];
extern u8 etc_yes_str[];
extern u8 etc_no_str[];
extern u8 save_card_full_str3[];
extern u8 save_card_full_str4[];
extern u8 save_card_full_str5[];

s32 save_card_full(void)
{
    u32 w;
    s32 flags;
    s32 r;

    switch (step_mode) {
    case 0:
        card_state_clear();
        device_cmd_send(1282);
        k_sub_reset();
        set_kanji_string(-128, 50, 1, (s32)save_card_full_str0, 0);
        k_sub_disp_start(1);
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
        k_sub_reset();
        set_kanji_string(-128, 50, 1, (s32)save_card_full_str1, 0);
        set_kanji_string(-128, 66, 1, (s32)save_card_full_str2, 0);
        set_kanji_string(-24, 4, 15, (s32)etc_yes_str, 0);
        r = get_etc_yes_button();
        set_kanji_string(60, 4, 1, r, 0);
        set_kanji_string(-24, 20, 15, (s32)etc_no_str, 0);
        r = get_etc_no_button();
        set_kanji_string(60, 20, 2, r, 0);
        k_sub_disp_start(6);
        w = (u8)(step_mode + 1);
        step_mode = w;
        return w;
    case 3:
        flags = pad_trig;
        if (flags & 0x20) {
            w = (u8)(step_mode + 1);
            step_mode = w;
            return w;
        }
        if ((flags & 0x40) == 0) {
            return;
        }
        return goto_sub_step(2);
    case 4:
        k_sub_reset();
        set_kanji_string(-128, 50, 1, (s32)save_card_full_str3, 0);
        set_kanji_string(-128, 66, 1, (s32)save_card_full_str4, 0);
        set_kanji_string(-128, 82, 1, (s32)save_card_full_str5, 0);
        k_sub_disp_start(3);
        w = (u8)(step_mode + 1);
        step_mode = w;
        return w;
    case 5:
        if (pad_trig == 0) {
            return;
        }
        card_step_init();
        return goto_sub_sub_step(0);
    }
}
