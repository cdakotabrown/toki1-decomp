/* @SymbolStart: 0x801407FC */
/* @RodataBase: 0x8014C398 */
/* @CompileEnv: IDO52_FORCE_WEB_NOSPILL=100 IDO52_FORCE_WEB_COLOR_NOSPLIT=100 IDO52_AS1_PASS1_VOLATILE_OK=3 IDO52_AS1_KEEP_DEAD_RELOAD=4 IDO52_AS1_REPL_TGT_KEEP=0 */
/* @PostProc: swap_instructions --offset1 0x98 --offset2 0x9c */
/* @PostProc: swap_instructions --offset1 0x2bc --offset2 0x2c0 */
/* @PostProc: swap_instructions --offset1 0x2c0 --offset2 0x2c8 */
/* @PostProc: swap_instructions --offset1 0x390 --offset2 0x398 */
/* @PostProc: swap_instructions --offset1 0x394 --offset2 0x398 */
/* @PostProc: patch_insn_reg_field 0x144:rd:t6:t8 0x14c:rt:t7:t9 0x150:rd:t2:t3 0x154:rt:t4:t5 0x158:rs:t4:t5 0x158:rt:t4:t5 0x15c:rs:t2:t3 0x15c:rt:t3:t4 0x160:rs:t3:t4 0x160:rt:t4:t5 0x188:rt:t5:t6 0x190:rt:t5:t6 0x194:rs:t7:t9 0x194:rt:t7:t9 0x198:rs:t6:t8 0x198:rt:t7:t9 0x1c0:rt:t8:t0 0x1c8:rt:t8:t0 0x1d8:rt:t9:t1 0x1dc:rs:t9:t1 0x1dc:rt:t9:t1 0x1e0:rt:t0:t2 0x1e4:rs:t9:t1 0x1e8:rt:t1:t3 0x1ec:rs:t0:t2 0x1ec:rt:t0:t2 0x1f0:rt:t7:t1 0x1f4:rs:t0:t2 0x1fc:rs:t1:t3 0x1fc:rt:t1:t3 0x200:rd:t2:t5 0x204:rs:t1:t3 0x20c:rt:t5:t9 0x21c:rd:t3:t7 0x220:rd:t4:t8 0x220:rs:t2:t5 0x220:rt:t3:t7 0x224:rs:t5:t9 0x224:rt:t5:t9 0x228:rs:t4:t8 0x228:rt:t5:t9 0x250:rt:t6:t0 0x258:rt:t6:t0 0x25c:rs:t7:t1 0x25c:rt:t7:t1 0x260:rd:t8:t3 0x264:rs:t7:t1 0x270:rt:t0:t6 0x274:rs:t0:t6 0x274:rt:t0:t6 0x278:rs:t8:t3 0x278:rt:t9:t4 0x27c:rs:t9:t4 0x27c:rt:t0:t6 0x2a4:rt:t1:t5 0x2ac:rt:t1:t5 0x2d4:rt:t2:t7 0x2d8:rs:t2:t7 0x2dc:rd:t3:t9 0x2ec:rd:t4:t1 0x2f0:rt:t6:t3 0x2f4:rs:t6:t3 0x2f4:rt:t6:t3 0x2f8:rd:t5:t2 0x2f8:rs:t3:t9 0x2f8:rt:t4:t1 0x2fc:rs:t5:t2 0x2fc:rt:t6:t3 0x324:rt:t7:t4 0x32c:rt:t7:t4 0x330:rt:t8:t6 0x338:rt:t8:t6 0x354:rt:t9:t5 0x358:rs:t9:t5 0x368:rt:t0:t7 0x370:rt:t0:t7 0x37c:rt:t1:t8 0x380:rs:t1:t8 0x398:rt:t2:t0 0x39c:rt:t2:t0 0x3a8:rt:t3:t9 0x3ac:rs:t3:t9 0x3ac:rt:t3:t9 0x3b4:rs:t3:t9 0x3b8:rt:t4:t1 0x3bc:rs:t4:t1 0x3bc:rt:t4:t1 0x3c4:rs:t4:t1 0x3d0:rt:t5:t2 0x3d4:rs:t5:t2 0x3d4:rt:t5:t2 0x3dc:rt:t5:t2 0x3e0:rt:t6:t3 0x3e4:rs:t6:t3 0x3e4:rt:t6:t3 0x3ec:rt:t6:t3 0x40c:rt:t7:t4 0x418:rt:t7:t4 0x41c:rt:t8:t6 0x420:rs:t8:t6 0x420:rt:t8:t6 0x428:rs:t8:t6 0x46c:rt:t9:t5 0x474:rt:t9:t5 */
/* @SymbolSize: 1136 */
#include "types.h"

extern u8 back_clear_r;
extern u8 back_clear_g;
extern u8 back_clear_b;
extern u8 screen_bri;
extern u8 now_year;
extern u8 now_month;
extern u8 now_day;
extern u8 denwa_cnt;
extern u8 ev_flag4d;
extern u8 rec_idx;
extern u8 step_mode;
extern u8 cursor_flag;

extern u8 near_click_check_tbl0[];
extern u8 name_ent_xa_girl_fmt[];
extern u8 name_ent_xa_idx_fmt[];
extern u8 name_ent_xa_step_str0[];
extern u8 mascot_flag2[];
extern u8 mascot_flag[];

extern void prim_init(void);
extern void sub_sub_step_up(void);
extern void device_cmd_send(s32);
extern s32 get_CD_State(void);
extern void set_XAsector(u32, u32, s32);
extern s32 get_g_zyotai_s(s32);
extern void dec_bg_reset(void);
extern void indexed_byte_toggle(s32);
extern void dec_bg_show(s32);
extern void set_dec_bri(s32);
extern void vram_full_move(s32);
extern s32 set_flag_b3_high_bit(s32);
extern s32 rec_table_trim(u8 *, u8 *, s32);
extern void printf();
extern void name_ent_bri_fade(void);

void name_ent_xa_step(void)
{
    s32 t;
    u32 tmp;
    s32 n;
    volatile u8 b2;
    u8 m;
    u8 d;
    u32 b;
    s32 v1;
    s32 tmp2;
    u32 *p;

    switch ((u32)step_mode) {
    case 0:
        dec_bg_reset();
        indexed_byte_toggle(2);
        vram_full_move(240);
        set_flag_b3_high_bit(0);
        prim_init();
        rec_idx = rec_table_trim(&m, &d, 1);
        b2 = m;
        printf(name_ent_xa_girl_fmt, m);
        printf(name_ent_xa_idx_fmt, b2);
        tmp = b2;
        if (tmp >= 13) {
            b = now_year;
            if (b == 98 && now_month == 3 && now_day == 1) {
            } else if (b == 95 && now_month == 4 && now_day == 4) {
            } else {
                device_cmd_send(5);
                device_cmd_send(193);
            }
            step_mode = 4;
        } else if (tmp == 11) {
            if (ev_flag4d != 0) {
                t = tmp;
                p = (u32 *)(((s32)(tmp << 4) + 8) + (s32)near_click_check_tbl0);
                set_XAsector(p[0], p[1], t);
                device_cmd_send(768);
                step_mode = step_mode + 1;
            } else {
                t = tmp;
                p = (u32 *)&near_click_check_tbl0[tmp << 4];
                set_XAsector(p[0], p[1], t);
                device_cmd_send(768);
                step_mode = step_mode + 1;
            }
        } else {
            if (tmp == 12) {
                if (mascot_flag[12] != 0 || mascot_flag2[12] != 0) {
                    if ((u32)denwa_cnt >= 41) {
                        v1 = 0;
                    } else {
                        v1 = 1;
                    }
                    t = tmp;
                    p = (u32 *)&near_click_check_tbl0[(tmp << 4) + (v1 << 3)];
                    set_XAsector(p[0], p[1], t);
                    device_cmd_send(768);
                    step_mode = step_mode + 1;
                } else if ((u32)denwa_cnt >= 41) {
                    t = tmp;
                    p = (u32 *)(((s32)(tmp << 4) + 8) + (s32)near_click_check_tbl0);
                    set_XAsector(p[0], p[1], t);
                    device_cmd_send(768);
                    step_mode = step_mode + 1;
                } else {
                    step_mode = 4;
                }
            } else {
                tmp2 = tmp;
                b2 = tmp;
                n = get_g_zyotai_s(tmp);
                tmp = b2;
                if ((n & 0x7F) == 0) {
                    v1 = 1;
                } else {
                    v1 = 0;
                }
                p = (u32 *)&near_click_check_tbl0[(tmp << 4) + (v1 << 3)];
                set_XAsector(p[0], p[1], tmp2);
                device_cmd_send(768);
                step_mode = step_mode + 1;
            }
        }
        break;
    case 1:
        screen_bri = 128;
        set_dec_bri(128);
        prim_init();
        if (get_CD_State() & 4) {
            step_mode = step_mode + 1;
        }
        break;
    case 2:
        if (get_CD_State() & 2) {
            step_mode = step_mode + 1;
            b = now_year;
            if (b == 98 && now_month == 3 && now_day == 1) {
            } else if (b == 95 && now_month == 4 && now_day == 4) {
            } else {
                device_cmd_send(5);
                device_cmd_send(193);
            }
        }
        break;
    case 3:
        step_mode = step_mode + 1;
        break;
    case 4:
        if ((u32)screen_bri < 4) {
            printf(name_ent_xa_step_str0);
            back_clear_g = 0;
            back_clear_r = 0;
            back_clear_b = 0;
            sub_sub_step_up();
        }
        break;
    }
    name_ent_bri_fade();
    dec_bg_show(2);
    cursor_flag = 1;
}
