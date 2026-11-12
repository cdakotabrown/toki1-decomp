/* @SymbolStart: 0x80142B34 */
/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:58,v0:71 */
/* @SymbolSize: 1660 */
/* @RodataBase: 0x8014D10C */
/* @PostProc: patch_stack_complete --old 56 --new 80 */
/* @PostProc: swap_instructions --offset1 0x260 --offset2 0x264 */
/* @PostProc: swap_instructions --offset1 0x290 --offset2 0x294 */
/* @PostProc: patch_insn_reg_field 0x264:rt:v0:v1 0x26c:rs:v0:v1 0x278:rt:v0:v1 0x27c:rt:v0:v1 0x28c:rs:v0:v1 0x28c:rt:v0:v1 */
/* @PostProc: remap_stack_slots --map 48:60,44:60 */
/* Address: 0x80142B3C (window starts 0x80142B34, cross-prologue head) */
/* Size: 1652 bytes (413 instructions) + 8-byte hoisted head = 1660 */
#include "types.h"

extern void prim_init(void);
extern void sub_sub_step_up(void);
extern void goto_step(s32);
extern void draw2d3d(s32, s32);
extern void back_clear_switch(s32);
extern void palette_upload_enqueue(u8 *, s32, s32, s32, s32);
extern void device_cmd_send(s32);
extern s32 get_CD_State2(void);
extern void cd_read_start2(s32, u8 *, s32);
extern void tpage_buf_clear(void);
extern void load_tpage_buf_lock(s32, u8 *, s32);
extern void class_clear_all(s32);
extern void clut_gray_set(u8 *, u8 *);
extern void etc_sprite_pair_init(void);
extern s32 etc_girls_all_check(void);

extern u8 back_clear_r;
extern u8 back_clear_g;
extern u8 back_clear_b;
extern u8 screen_bri;
extern u8 now_girl;
extern s32 pad_data;
extern u8 step_mode;
extern u8 option_on;
extern u8 cursor_work_bak[];
extern s32 etc_roll_pic;
extern s32 etc_roll_allgirl;
extern s32 etc_roll_load_flag;
extern s32 etc_fade_dir;

void etc_roll_girl_load(void)
{
    s32 i;
    u8 *p;
    u8 *q;

    switch (step_mode) {
    case 0:
        etc_roll_pic = 0;
        screen_bri = 128;
        device_cmd_send(209);
        back_clear_g = 0;
        back_clear_r = 0;
        etc_fade_dir = 0;
        back_clear_b = 0;
        draw2d3d(1, 0);
        prim_init();
        class_clear_all(0);
        back_clear_switch(1);
        tpage_buf_clear();
        if (now_girl >= 13U) {
            if (option_on != 0) {
                goto_step(18);
            } else {
                goto_step(196);
            }
            return;
        }
        etc_roll_allgirl = 0;
        if ((pad_data & 0x10) != 0) {
            etc_roll_allgirl = 1;
        }
        if (etc_girls_all_check() != 0) {
            etc_roll_allgirl = 1;
        }
        etc_roll_load_flag = 0;
        cd_read_start2(17, (u8 *)0x80180000, 41147);
        step_mode = 1;
        break;
    case 1:
        if ((get_CD_State2() & 1) != 0) {
            etc_fade_dir = 1;
            load_tpage_buf_lock(21, (u8 *)0x80180000, 1);
            load_tpage_buf_lock(22, (u8 *)0x80184000, 1);
            clut_gray_set((u8 *)0x80188000, cursor_work_bak);
            palette_upload_enqueue(cursor_work_bak, 31, 1, 1, 0);
            etc_sprite_pair_init();
            cd_read_start2(1, (u8 *)0x801F9000, 48683);
            step_mode = step_mode + 1;
        }
        break;
    case 2:
        if ((get_CD_State2() & 1) != 0) {
            cd_read_start2(82, (u8 *)0x801D0000, 41065);
            step_mode = step_mode + 1;
        }
        break;
    case 3:
        if ((get_CD_State2() & 1) != 0) {
            palette_upload_enqueue((u8 *)0x801F8990, 15, 1, 1, 0);
            p = (u8 *)0x801E0000;
            for (i = 0; i < 9; i++) {
                load_tpage_buf_lock(i + 37, p, 2);
                p += 8192;
            }
            q = (u8 *)0x801D0000;
            for (i = 0; i != 8; i++) {
                load_tpage_buf_lock(i + 8, q, 2);
                q += 8192;
            }
            if (etc_roll_allgirl != 0) {
                cd_read_start2(5, (u8 *)0x80180000, 31696);
            } else {
                cd_read_start2(1, (u8 *)0x80180000, 45772);
            }
            step_mode = step_mode + 1;
        }
        break;
    case 4:
        if ((get_CD_State2() & 1) != 0) {
            if (etc_roll_allgirl != 0) {
                for (i = 0; i != 14; i++) {
                    palette_upload_enqueue((u8 *)0x80180020, (u8)(i + 16), 1, 1, 0);
                }
            } else {
                for (i = 0; i != 14; i++) {
                    palette_upload_enqueue((u8 *)0x80180000, (u8)(i + 16), 1, 1, 0);
                }
            }
            if (etc_roll_allgirl != 0) {
                cd_read_start2(13, (u8 *)0x801E8000, 31727);
                cd_read_start2(13, (u8 *)0x801E0000, 31779);
                cd_read_start2(13, (u8 *)0x801D8000, 31714);
                cd_read_start2(13, (u8 *)0x801D0000, 31753);
                cd_read_start2(13, (u8 *)0x801C8000, 31831);
            } else {
                cd_read_start2(13, (u8 *)0x801E8000, 45786);
                cd_read_start2(13, (u8 *)0x801E0000, 45838);
                cd_read_start2(13, (u8 *)0x801D8000, 45773);
                cd_read_start2(13, (u8 *)0x801D0000, 45851);
                cd_read_start2(13, (u8 *)0x801C8000, 45890);
            }
            step_mode = step_mode + 1;
        }
        break;
    case 10:
        if ((get_CD_State2() & 1) != 0) {
            if (etc_roll_allgirl != 0) {
                cd_read_start2(13, (u8 *)0x801C0000, 31792);
                cd_read_start2(13, (u8 *)0x801B8000, 31805);
                cd_read_start2(13, (u8 *)0x801B0000, 31766);
                cd_read_start2(13, (u8 *)0x801A8000, 31701);
                cd_read_start2(13, (u8 *)0x801A0000, 31818);
            } else {
                cd_read_start2(13, (u8 *)0x801C0000, 45825);
                cd_read_start2(13, (u8 *)0x801B8000, 45864);
                cd_read_start2(13, (u8 *)0x801B0000, 45812);
                cd_read_start2(13, (u8 *)0x801A8000, 45759);
                cd_read_start2(13, (u8 *)0x801A0000, 45877);
            }
            step_mode = step_mode + 1;
        }
        break;
    case 16:
        if ((get_CD_State2() & 1) != 0) {
            etc_fade_dir = -1;
            if (etc_roll_allgirl != 0) {
                cd_read_start2(13, (u8 *)0x80198000, 31844);
                cd_read_start2(13, (u8 *)0x80190000, 31857);
                cd_read_start2(13, (u8 *)0x80188000, 31740);
                cd_read_start2(13, (u8 *)0x80180000, 31870);
            } else {
                cd_read_start2(13, (u8 *)0x80198000, 45903);
                cd_read_start2(13, (u8 *)0x80190000, 45916);
                cd_read_start2(13, (u8 *)0x80188000, 45799);
                cd_read_start2(13, (u8 *)0x80180000, 45929);
            }
            step_mode = step_mode + 1;
        }
        break;
    case 21:
        sub_sub_step_up();
        break;
    default:
        if ((get_CD_State2() & 1) != 0) {
            step_mode = step_mode + 1;
        }
        break;
    }
}
