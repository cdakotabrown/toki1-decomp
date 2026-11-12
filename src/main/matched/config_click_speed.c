/* Function: config_click_speed */
/* Address: 0x8005F2DC */
/* Size: 400 bytes (100 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:30 IDO52_AS1_FORCE_NARROW_FOLD=3 IDO52_AS1_NO_COPY_PROP=2 */
/* @CompileFlags: -Wc,-pic1 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_insn_reg_field --patch 0xb4:rt:a0:v1 --patch 0xe4:rt:t2:v0 --patch 0xf0:rt:t2:v0 --patch 0xfc:rs:a0:v1 --patch 0xfc:rt:a0:v1 --patch 0x100:rt:t3:t2 --patch 0x104:rs:a0:v1 --patch 0x10c:rs:t3:t2 --patch 0x10c:rt:a0:v1 --patch 0x10c:rd:v1:v0 --patch 0x110:rs:v1:v0 --patch 0x110:rt:t4:t3 --patch 0x114:rs:t4:t3 --patch 0x11c:rs:t4:t3 --patch 0x134:rt:t5:v0 --patch 0x13c:rt:t5:v0 --patch 0x178:rt:a0:v1 --patch 0x17c:rt:a0:v1 */
#include "types.h"

extern void menu_check(s32, s16, s16);
extern void menu_bar_show(s32);
extern void menu_cursor_move(s32);
extern void k_disp_inc(void);
extern void goto_sub_sub_step(s32);
extern void k_sub_reset(void);
extern void set_kanji_string(s32, s32, s32, u8*, s32);
extern void sub_sub_step_up(void);

extern s32 pad_trig;
extern u8 mouse_btn_mode;
extern u8 click_flags;
extern s8 menu_csr;
extern u8 click_speed;
extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern u8 config_click_speed_str0[];
extern u8 config_click_speed_str1[];
extern s32 work_fuku[];

void config_click_speed(void)
{
    s32 val;
    s32 m;
    s32 mode;
    u8 vol;

    menu_check(1, cursor_cw_x, cursor_cw_y);
    menu_bar_show(1);
    menu_cursor_move(1);
    k_disp_inc();

    if (pad_trig & 0x40) {
        goto_sub_sub_step(0);
        return;
    }

    mode = mouse_btn_mode;
    if (mode != 0) {
        if (click_flags & 4) {
            goto mode_switch;
        }
    }

    if (mode != 0) {
        return;
    }

    if (!(click_flags & 8)) {
        return;
    }

mode_switch:
    switch (menu_csr) {
    case 0:
        k_sub_reset();
        set_kanji_string(-128, 50, 0, config_click_speed_str0, 0);
        vol = 32;
        click_speed = vol;
        sub_sub_step_up();
        return;
    case 1:
        val = work_fuku[5];
        if (val != 0) {
            m = (255 - val) & 0xff;
            vol = m;
            if ((u32)m < 16) {
                vol = 16;
            }
            click_speed = vol;
            if ((u32)vol >= 65) {
                vol = 64;
                click_speed = vol;
            }
            work_fuku[5] = 0;
            k_sub_reset();
            set_kanji_string(-128, 50, 0, config_click_speed_str1, 0);
            sub_sub_step_up();
            return;
        }
        work_fuku[5] = 255;
        break;
    }
}
