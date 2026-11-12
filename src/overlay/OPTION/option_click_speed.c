/* @CompileEnv: IDO52_COPYPROP_SITE=lod:1 IDO52_AS1_NO_COPY_PROP=3,4 */
/* @CompileEnv: IDO52_AS1_NO_LA_PULL=2,9,11 */
/* @PostProc: swap_instructions --offset1 0x54 --offset2 0x5c */
/* @PostProc: swap_instructions --offset1 0x58 --offset2 0x5c */
/* @PostProc: swap_instructions --offset1 0x64 --offset2 0x68 */
/* @PostProc: swap_instructions --offset1 0x100 --offset2 0x104 */
/* @PostProc: swap_instructions --offset1 0x130 --offset2 0x198 */
/* @PostProc: swap_instructions --offset1 0x1a8 --offset2 0x1b4 */
/* @PostProc: patch_insn_reg_field 0xe4:rt:v0:v1 0x100:rt:t7:v0 0x108:rt:t7:v0 0x124:rs:v0:v1 0x124:rt:v0:v1 0x128:rt:t6:t7 0x12c:rs:v0:v1 0x134:rs:t6:t7 0x134:rt:t6:t7 0x138:rt:t9:t8 0x140:rd:v1:v0 0x140:rs:t9:t8 0x140:rt:v0:v1 0x144:rs:t6:t7 0x144:rt:t8:t6 0x148:rt:t8:t6 0x14c:rs:v1:v0 0x14c:rt:t0:t9 0x150:rs:t0:t9 0x158:rd:a0:v0 0x158:rs:t0:t9 0x15c:rt:a0:v0 0x164:rt:a0:v0 0x168:rs:a0:v0 0x170:rt:t1:v0 0x178:rt:t1:v0 0x198:rt:t2:v1 0x19c:rt:t2:v1 0x1a0:rt:t3:t0 0x1a4:rs:t3:t0 0x1a4:rt:t3:t0 0x1a8:rt:t5:t2 0x1ac:rs:t3:t0 0x1ac:rt:t4:t1 0x1b0:rs:t4:t1 0x1b8:rs:t5:t2 0x1b8:rt:t5:t2 0x1c0:rs:t5:t2 0x1c0:rt:t7:t3 0x1c8:rt:t7:t3 0x1d4:rt:v0:v1 0x1d8:rs:v0:v1 0x1d8:rt:v0:v1 0x1e0:rs:v0:v1 0x1e4:rs:v0:v1 0x1e4:rt:t6:t4 0x1e8:rt:t6:t4 */
/* Address: 0x80135110, Size: 652 bytes (163 instructions) */
#include "types.h"
#include "tokimeki/class_w.h"

extern void menu_check();
extern void menu_cursor_move();
extern void option_menu_box_show();
extern void device_cmd_send();
extern void sub_sub_step_up();

extern s16 cursor_cw_x;
extern s16 cursor_cw_y;
extern s8 menu_csr;
extern u8 cw96_active[];
extern u8 click_speed;
extern u8 mouse_btn_mode;
extern u8 click_flags;
extern u32 pad_trig;
extern u32 frame_counter;
extern u8 option_on;
extern u8 option_click_cnt;
extern CLASS_W class_w[];

void option_click_icon_anim(void);

void option_click_speed(void)
{
    s32 a2;
    s32 v1;
    s32 f;
    u32 x;
    u32 t;
    u8 *v0;
    u8 *a0;
    u8 *end;

    menu_check(1, cursor_cw_x, cursor_cw_y);
    menu_cursor_move(1);
    option_menu_box_show();

    a2 = menu_csr;

    v0 = &cw96_active[0];
    a0 = v0 + a2 * 68;
    end = &cw96_active[136];
    v1 = 0;
    do {
        if (v0 == a0) {
            *(s16 *)(v0 + 1248) = (s16)(v1 + 41);
        } else {
            *(s16 *)(v0 + 1248) = (s16)(v1 + 42);
        }
        v0 += 68;
        v1 += 2;
    } while (v0 != end);

    f = mouse_btn_mode;
    if (((f != 0) && (click_flags & 4)) || ((f == 0) && (click_flags & 8))) {
        switch (a2) {
        case 0:
            option_on = (u8)(option_on | 0x20);
            x = 32;
            click_speed = (u8)x;
            device_cmd_send(1281);
            sub_sub_step_up();
            break;
        case 1:
            v1 = option_click_cnt;
            if (v1 != 0) {
                option_on = (u8)(option_on | 0x20);
                t = (u32)(255 - v1) & 0xFF;
                x = t;
                if (t < 16U) {
                    x = 16;
                }
                click_speed = (u8)x;
                if (x >= 65U) {
                    x = 64;
                    click_speed = (u8)x;
                }
                device_cmd_send(1281);
                sub_sub_step_up();
                option_click_cnt = 0;
            } else {
                option_click_cnt = 255;
            }
            break;
        }
    }

    if (pad_trig & 0x40) {
        class_w[116].disp_flag = (u8)(class_w[116].disp_flag & 0xFF7F);
        sub_sub_step_up();
    }

    option_click_icon_anim();

    v1 = option_click_cnt;
    if (v1 != 0) {
        option_click_cnt = (u8)(v1 - 1);
    }
}

void option_click_icon_anim(void)
{
    u32 d;
    u32 n;

    d = click_speed;
    if (d == 0) {
        return;
    }
    n = frame_counter;
    if (((n / d) & 3) >= 2U) {
        return;
    }
    if ((n % d) < 2U) {
        if (mouse_btn_mode == 0) {
            class_w[116].frame = 120;
        } else {
            class_w[116].frame = 121;
        }
    } else {
        class_w[116].frame = 119;
    }
}
