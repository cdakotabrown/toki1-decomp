/* @CompileFlags: -Wc,-pic0 */
/* @PostProc: swap_instructions --offset1 0x214 --offset2 0x224 */
/* @PostProc: swap_instructions --offset1 0x218 --offset2 0x220 */
/* @PostProc: swap_instructions --offset1 0x244 --offset2 0x24c */
/* @PostProc: swap_instructions --offset1 0x248 --offset2 0x250 */
/* @PostProc: swap_instructions --offset1 0x24c --offset2 0x254 */
/* @PostProc: swap_instructions --offset1 0x250 --offset2 0x254 */
/* @PostProc: swap_instructions --offset1 0x254 --offset2 0x258 */
/* @PostProc: swap_instructions --offset1 0x258 --offset2 0x25c */
/* @PostProc: swap_instructions --offset1 0x25c --offset2 0x260 */
/* @PostProc: swap_instructions --offset1 0x260 --offset2 0x264 */
/* @PostProc: swap_instructions --offset1 0x264 --offset2 0x268 */
/* @PostProc: swap_instructions --offset1 0x268 --offset2 0x26c */
/* @PostProc: swap_instructions --offset1 0x26c --offset2 0x270 */
/* @PostProc: swap_instructions --offset1 0x270 --offset2 0x274 */
/* @PostProc: swap_instructions --offset1 0x274 --offset2 0x278 */
/* @PostProc: swap_instructions --offset1 0x278 --offset2 0x27c */
/* @PostProc: swap_instructions --offset1 0x2f0 --offset2 0x2f4 */
/* @PostProc: swap_instructions --offset1 0x2f4 --offset2 0x2fc */
/* @PostProc: patch_insn_reg_field 0x150:rt:t1:t0 0x158:rt:t1:t0 0x160:rt:t2:t1 0x164:rt:t2:t1 0x174:rt:t3:t2 0x178:rt:t3:t2 0x17c:rt:t4:t3 0x180:rs:t4:t3 0x180:rt:t4:t3 0x188:rt:t4:t3 0x18c:rt:t5:t4 0x190:rs:t5:t4 0x190:rt:t5:t4 0x198:rt:t5:t4 0x19c:rt:t6:t5 0x1a0:rs:t6:t5 0x1a0:rt:t6:t5 0x1a8:rt:t6:t5 0x1b8:rt:t7:t6 0x1bc:rt:t7:t6 0x1c4:rt:t8:t7 0x1c8:rt:t8:t7 0x1d8:rt:t9:t8 0x1dc:rt:t9:t8 0x1e4:rt:t1:t9 0x1ec:rt:t1:t9 0x238:rt:t0:v1 0x244:rt:t5:t3 0x248:rt:t6:t4 0x24c:rt:t7:t5 0x250:rt:t2:t0 0x254:rt:t3:t1 0x258:rt:t4:t2 0x25c:rs:t5:t3 0x25c:rt:t5:t3 0x260:rs:t6:t4 0x260:rt:t6:t4 0x264:rs:t7:t5 0x264:rt:t7:t5 0x268:rt:t8:t6 0x26c:rt:t9:t7 0x270:rt:t1:t8 0x274:rt:t2:t9 0x27c:rt:t2:t0 0x280:rt:t3:t1 0x288:rt:t4:t2 0x28c:rt:t5:t3 0x290:rt:t6:t4 0x294:rt:t7:t5 0x298:rt:t8:t6 0x29c:rt:t9:t7 0x2a0:rt:t1:t8 0x2a4:rt:t2:t9 0x2a8:rt:t0:v1 0x2e8:rd:t3:t0 0x2ec:rt:t3:t0 0x324:rs:t0:v1 0x330:rd:t5:t2 0x334:rt:t5:t2 0x354:rd:t6:t2 0x358:rt:t7:t4 0x35c:rs:t7:t4 0x35c:rt:t7:t4 0x360:rs:t0:v1 0x364:rd:t8:t5 0x364:rt:t7:t4 0x368:rd:t9:t6 0x368:rt:t8:t5 0x36c:rs:t9:t6 0x370:rt:t3:t9 0x37c:rd:t1:t7 0x380:rt:t1:t7 0x398:rs:t3:t9 0x39c:rd:t4:t0 0x3a0:rd:t5:t1 0x3a0:rt:t4:t0 0x3a8:rs:t5:t1 0x3ac:rd:t6:t2 0x3ac:rs:t6:t2 0x3b0:rd:t6:t2 0x3b0:rt:t6:t2 0x3b4:rt:v1:t3 0x3b8:rd:v1:t3 0x3b8:rs:v1:t3 0x3b8:rt:t6:t2 0x3bc:rs:v1:t3 0x3bc:rt:v1:t3 0x3c0:rs:t0:v1 0x3c4:rd:t7:t5 0x3c4:rt:v1:t3 0x3c8:rs:t7:t5 0x3cc:rt:t2:t9 0x3d0:rd:t1:t8 0x3d8:rt:t1:t8 0x3e0:rd:t8:t6 0x3e4:rt:t8:t6 0x3fc:rs:t2:t9 0x400:rd:t3:t0 0x404:rd:t4:t1 0x404:rt:t3:t0 0x408:rs:t4:t1 */
/* @CompileEnv: IDO52_AS1_PEEPREG_KEEP=0 */
/* Address: 0x801478F8 */
/* Size: 1128 bytes (282 instructions) */
/* `@PostProc: pad_trailing_nop --count 280` -- the entire body was fake. */
#include "types.h"

extern void sub_sub_step_up(void);
extern void device_port_init(s32, s32);
extern s32 get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void class_clear_f(void);
extern void class_clear_n(s32);
extern void text_sys_init(void);
extern void set_kanji_string();
extern void k_sub_reset_point_set(void);
extern void k_sub_disp_start(s32);
extern void gnsx();
extern void sndisp();
extern void parameter_disp_switch(s32);
extern void hizuke_disp_switch(s32);
extern void hizuke_init(void);
extern void icon_disp_switch(s32);
extern void basyo_init();
extern void strncpy();
extern void printf();
extern void strncat();
extern void etc_week_rank_calc(void);
extern void etc_shade_disp(void);
extern void etc_box_shade_show(void);
extern void etc_clut_load(void);

typedef struct {
    u8 b0;
    u8 b1;
    u8 b2;
    u8 b3;
    u8 b4;
    u8 b5;
    u8 b6;
    u8 b7;
    s32 w8;
    s32 w12;
    s32 w16;
    s32 w20;
    s16 h24;
    s16 h26;
    s16 h28;
    s16 h30;
    s16 h32;
    s16 h34;
    s16 h36;
    s16 h38;
    s16 h40;
    s16 h42;
    s16 h44;
    s16 h46;
    s16 h48;
    s16 h50;
    s32 w52;
    s32 w56;
    s32 w60;
    s32 w64;
} Rec;

extern Rec class_w[];
extern Rec class_w_r[];
extern u8 player_rec[];
extern u8 p_sei[];
extern u8 p_mei[];
typedef struct {
    s32 w0;
    s32 pad[13];
} Rec56;

extern Rec56 girl0_flag[];
extern s32 girl11_flag;
extern s32 step_counter;
extern u8 step_mode;
extern s32 girl_full_name_tbl[];
extern s32 girl_full_name_tbl_11;
extern s32 telephone_choice_menu_init_tbl2;
extern s16 etc_rank_x[];
extern s16 etc_rank_y[];
extern s32 etc_rank_order[];
extern u8 etc_week_rank_disp_str0[];
extern u8 etc_week_rank_disp_str1[];
extern u8 etc_rank_player_fmt[];

s32 etc_week_rank_disp(void)
{
    u8 name[268];
    Rec *r;
    s16 *xs;
    s16 *ys;
    s32 *ks;
    s32 i;
    s32 x;
    s32 y;
    s32 k;
    s32 f;
    s32 j;

    k = step_mode;
    switch (k) {
    case 0:
        text_sys_init();
        basyo_init(etc_week_rank_disp_str0);
        gnsx(p_sei);
        sndisp(etc_week_rank_disp_str1, 0, 31);
        k_sub_reset_point_set();
        k_sub_disp_start(0);
        device_port_init(1, 514);
        hizuke_init();
        class_clear_f();
        step_mode = step_mode + 1;
        cd_read_start2(13, 0x801E0000, 0x9CA8);
        return;

    case 1:
        if (step_counter >= 257U) {
            step_mode++;
        }
        return;

    case 2:
        if ((get_CD_State2() & 1) == 0) {
            return;
        }
        text_sys_init();
        icon_disp_switch(0);
        parameter_disp_switch(0);
        hizuke_disp_switch(0);
        class_clear_n(96);
        class_w[96].b1 = 4;
        class_w[96].b2 = 64;
        class_w[96].w56 = 0;
        class_w[96].b3 = 164;
        class_w[96].w12 = 0x801E64A0;
        class_w[96].w16 = 0x801E6520;
        class_w[96].w52 = 0x801E6494;
        class_w[96].h24 = 0;
        class_w[96].b5 = 1;
        class_w[96].b4 = 8;
        class_w[96].h38 = 0;
        class_w[96].h42 = -40;
        class_w[96].b7 = 128;
        etc_shade_disp();
        etc_box_shade_show();
        etc_week_rank_calc();
        r = class_w_r;
        xs = etc_rank_x;
        ys = etc_rank_y;
        ks = etc_rank_order;
        for (i = 0; i != 14; i++) {
            x = *xs;
            y = *ys;
            k = *ks;
            r[97].b1 = 3;
            r[97].b2 = 64;
            r[97].w56 = 0;
            r[97].b3 = 36;
            r[97].w12 = 0x801E64A0;
            r[97].w16 = 0x801E6520;
            r[97].w52 = 0x801E6494;
            r[97].h24 = 31;
            r[97].b5 = 1;
            r[97].b4 = 8;
            r[97].b7 = 128;
            r[97].h38 = x;
            r[97].h42 = y;
            if (k == 13) {
                strncpy(name, p_sei, 8);
                strncat(name, p_mei, 8);
                set_kanji_string((s16)(*xs + 24), *ys, 3, name, 0);
                printf(etc_rank_player_fmt, player_rec);
            } else if (k == 10) {
                set_kanji_string((s16)(x + 24), y, 3, telephone_choice_menu_init_tbl2, 0);
            } else if (k == 11) {
                j = i + 97;
                if ((u32)(girl11_flag << 20) >> 29) {
                    set_kanji_string((s16)(x + 24), y, 3, girl_full_name_tbl_11, 0);
                } else {
                    class_w[j].b3 = 164;
                }
            } else {
                f = girl0_flag[k].w0;
                j = i + 97;
                if ((f << 30) < 0) {
                    set_kanji_string((s16)(x + 24), y, 3, girl_full_name_tbl[k], 0);
                } else {
                    class_w[j].b3 = 164;
                }
            }
            r++;
            xs++;
            ys++;
            ks++;
        }
        k_sub_reset_point_set();
        k_sub_disp_start(0);
        etc_clut_load();
        sub_sub_step_up();
        return;
    }
}
