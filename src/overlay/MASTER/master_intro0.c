/* @SymbolStart: 0x80133AA0 */
/* @SymbolSize: 1248 */
/* @RodataBase: 0x8013BCB8 */
/* @CompileFlags: -Wo,-loopunroll,0 -Wc,-pic1 */
/* @PostProc: patch_stack_complete --old 96 --new 56 */
/* @PostProc: swap_instructions --offset1 0x11c --offset2 0x128 */
/* @PostProc: swap_instructions --offset1 0x120 --offset2 0x12c */
/* @PostProc: swap_instructions --offset1 0x148 --offset2 0x150 */
/* @PostProc: swap_instructions --offset1 0x14c --offset2 0x150 */
/* @PostProc: swap_instructions --offset1 0x154 --offset2 0x160 */
/* @PostProc: swap_instructions --offset1 0x158 --offset2 0x174 */
/* @PostProc: swap_instructions --offset1 0x15c --offset2 0x160 */
/* @PostProc: swap_instructions --offset1 0x160 --offset2 0x174 */
/* @PostProc: swap_instructions --offset1 0x164 --offset2 0x174 */
/* @PostProc: swap_instructions --offset1 0x168 --offset2 0x174 */
/* @PostProc: swap_instructions --offset1 0x16c --offset2 0x174 */
/* @PostProc: swap_instructions --offset1 0x170 --offset2 0x174 */
/* @PostProc: swap_instructions --offset1 0x490 --offset2 0x494 */
/* @PostProc: swap_instructions --offset1 0x49c --offset2 0x4a0 */
/* @PostProc: patch_insn_reg_field 0x128:rt:t0:v0 0x12c:rt:v0:a0 0x130:rs:t0:a0 0x130:rt:t0:a0 0x140:rt:a0:t9 0x144:rt:a1:t3 0x148:rd:t9:t1 0x148:rt:a0:t9 0x14c:rt:a2:t9 0x150:rt:a3:t6 0x154:rd:t1:t2 0x154:rt:t9:t1 0x158:rt:t1:t2 0x15c:rd:t2:t4 0x15c:rt:a1:t3 0x160:rd:t4:t1 0x160:rt:a2:t9 0x164:rd:t6:t7 0x164:rt:a3:t6 0x168:rd:t3:t8 0x168:rt:t2:t7 0x16c:rd:t5:t2 0x16c:rt:t4:t1 0x170:rd:t7:t5 0x170:rt:t6:t4 0x178:rt:t3:t5 0x17c:rt:t5:t2 0x180:rt:t7:t8 0x184:rt:t0:a0 0x1c8:rt:v0:v1 0x1d0:rs:v0:v1 0x1d8:rs:v0:v1 0x1d8:rt:t8:t3 0x1e4:rt:t8:t3 0x1fc:rt:t9:t4 0x200:rs:t9:t4 0x268:rt:t1:t5 0x26c:rs:t1:t5 0x2a0:rt:t2:t6 0x2a4:rs:t2:t6 0x2a4:rt:t2:t6 0x2b0:rt:t3:t7 0x2b4:rs:t3:t7 0x2b4:rt:t3:t7 0x2b8:rt:t2:t6 0x2c0:rt:t4:t8 0x2c4:rs:t4:t8 0x2c4:rt:t4:t8 0x2c8:rt:t3:t7 0x2d4:rt:t4:t8 0x314:rt:t5:t9 0x318:rs:t5:t9 0x374:rt:t6:t1 0x378:rs:t6:t1 0x388:rd:v1:t0 0x390:rs:v1:t0 0x394:rt:t7:t2 0x39c:rs:t7:t2 0x3a8:rt:v1:t0 0x3b0:rt:v1:t0 0x3b4:rt:v1:t0 0x3bc:rs:v1:t0 0x3bc:rt:v1:t0 0x3c0:rs:v1:t0 0x3d0:rt:t8:t3 0x3d8:rt:t8:t3 0x3e0:rt:t9:t4 0x3e4:rt:t9:t4 0x3ec:rt:t1:t5 0x3f0:rt:t1:t5 0x3f8:rt:t2:t6 0x3fc:rt:t2:t6 0x404:rt:t3:t7 0x408:rt:t3:t7 0x410:rt:t4:t8 0x414:rt:t4:t8 0x41c:rt:t5:t9 0x420:rt:t5:t9 0x428:rt:t6:t1 0x42c:rt:t6:t1 0x434:rt:t7:t2 0x438:rt:t8:t3 0x43c:rs:t8:t3 0x43c:rt:t8:t3 0x440:rt:t7:t2 0x448:rt:t9:t4 0x44c:rs:t9:t4 0x44c:rt:t9:t4 0x450:rt:t8:t3 0x458:rt:t1:t5 0x45c:rs:t1:t5 0x45c:rt:t1:t5 0x460:rt:t9:t4 0x468:rt:t2:t6 0x46c:rs:t2:t6 0x46c:rt:t2:t6 0x470:rt:t1:t5 0x47c:rt:t2:t6 0x488:rt:t3:t7 0x48c:rs:t3:t7 0x48c:rt:t3:t7 0x498:rd:t4:t8 0x498:rt:t3:t7 0x4a4:rd:t5:t9 0x4a4:rt:t4:t8 0x4a8:rt:v1:t4 0x4ac:rs:t5:t9 0x4ac:rt:t6:t1 0x4b0:rd:t7:t2 0x4b0:rt:t6:t1 0x4b4:rs:t7:t2 0x4b4:rt:t8:t3 0x4b8:rs:v1:t4 0x4b8:rt:t9:t5 0x4bc:rd:t1:t6 0x4bc:rs:t8:t3 0x4bc:rt:t9:t5 0x4c4:rt:t1:t6 */
/* Address: 0x80133AA8 (window starts at the absorbed cross-prologue head */
/* Size: 1240 bytes (310 insns) + 8-byte head = 1248 */
#include "types.h"
#include "tokimeki/class_w.h"

extern void device_cmd_send(s32);
extern void device_load_start();
extern void date_x_taku_set(s32, s32);
extern void shared_system_state_reset(void);
extern void dec_bg_reset(void);
extern void set_dec_bri(s32);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void parameter_disp_switch(s32);
extern void icon_disp_switch(s32);
extern void k_reset(s32);
extern void basyo_init(void *);
extern void mes_window_switch(s32);
extern void srand();
extern void sub_sub_step_up(void);
extern s32 device_load_poll(void);
extern void value_cache_find_or_load(s32, s32);
extern s32 get_CD_State2(void);
extern s32 dec_bg_init(s32);
extern void indexed_byte_lsb_set();
extern void cd_read_start2();
extern void fade_in_step();
extern void master_serifu_set2(void);
extern void date_name_set(s32);
extern void yazirushi_off(void);
extern void sub_step_up(void);
extern void serifu_run(void);
extern void sprite_fade_main(void);
extern void master_intro_res_set(void);
extern s32 palette_upload_enqueue(s32, s32, s32, s32, s32);
extern void class_clear_n(s32);
extern void game_reset(void);
extern void common_disp(void);

extern u8  cal_cw_prio;
extern u8  cal_cw_sel;
extern u8  cal_cw_rate;
extern u8  cal_cw_bright;
extern s32 cal_cw_img;
extern s32 cal_cw_anim_tbl;
extern s16 cal_cw_aux14;
extern s16 cal_cw_x;
extern s16 cal_cw_y;
extern s32 cal_cw_hdr;
extern s32 cal_cw_attr;
extern u8  fade_bri;
extern s16 now_serifu;
extern s16 now_serifu_in;
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern u8 girl_appear[];
extern u8  screen_w[];
extern u8  girl_blk[];
extern s32 girl0_flag;
extern s32 girl1_flag;
extern s32 girl2_flag;
extern u8  girl_blk_end[];
extern s32 bukatu_pt6;
extern u8  now_girl;
extern s32 vblnk_counter;
extern u8  now_sub_sub_step;
extern u8  master_intro0_str0[];
extern s32 master_serifu_ptr[];
extern s32 master_cw_no;
extern s32 master_intro_res_tbl[];
extern s32 master_intro_pal_ptr;
extern CLASS_W class_w[];

void master_intro0(void) {
    s32 sel = now_sub_sub_step;
    s32 sndarg;
    s32 t9, t3_val;
    s32 t6v;
    s32 a, b, c, d;
    u8 sv;
    s32 v0_val;
    s32 t0, t1, t2;
    s32 aidx;
    u8 aidx8;
    s32 fval;
    u32 t8;
    u8 t4;
    s32 t5;
    u8 *v0;
    u8 *v1;
    u8 *a0;

    switch ((u32)sel) {
    case 0:
        device_cmd_send(191);
        device_load_start(1, 0xC051, 0xC02B, 0xC990, 0xC943, 0xC929);
        date_x_taku_set(1, 0);
        shared_system_state_reset();
        dec_bg_reset();
        set_dec_bri(0);
        hizuke_init();
        message_window_init();
        parameter_disp_switch(0);
        icon_disp_switch(0);
        k_reset(0);
        basyo_init(master_intro0_str0);
        mes_window_switch(0);
        sndarg = vblnk_counter;
        srand(sndarg);
        t9 = girl0_flag;
        t3_val = girl1_flag;
        t6v = girl2_flag;
        girl_appear[0] = (u8)((u32)(t9 << 30) >> 31);
        girl_appear[1] = (u8)((u32)(t3_val << 30) >> 31);
        a0 = girl_blk_end;
        v0 = girl_blk;
        v1 = (&girl_appear[3]);
        girl_appear[2] = (u8)((u32)(t6v << 30) >> 31);
        do {
            a = *(s32 *)(v0 + 456);
            b = *(s32 *)(v0 + 512);
            c = *(s32 *)(v0 + 624);
            d = *(s32 *)(v0 + 568);
            v1[0] = (u8)((u32)(a << 30) >> 31);
            v1[1] = (u8)((u32)(b << 30) >> 31);
            v1[3] = (u8)((u32)(c << 30) >> 31);
            v1[2] = (u8)((u32)(d << 30) >> 31);
            v0 += 224;
            v1 += 4;
        } while (v0 != a0);
        sub_sub_step_up();
        break;
    case 1:
        if (device_load_poll() == 1) {
            value_cache_find_or_load(16808, 1);
            fade_bri = 0;
            sub_sub_step_up();
        }
        sv = fade_bri;
        if ((u32)sv >= 4U) {
            fade_bri = (u8)(sv - 4);
        } else {
            fade_bri = 0;
        }
        break;
    case 2:
        if (get_CD_State2() & 1) {
            device_cmd_send(512);
            mes_window_switch(1);
            v0_val = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0_val);
            cd_read_start2(2, (void *)0x801E8000, 0xA9BA);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            master_serifu_set2();
            now_girl = 0;
            date_name_set(0);
            yazirushi_off();
            now_serifu = 0;
            now_serifu_in = 0;
            ss_serifu = master_serifu_ptr[0];
            ss_serifu_kind = master_serifu_ptr[1];
            sss_sound = master_serifu_ptr[2];
            sub_sub_step_up();
        }
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        value_cache_find_or_load(16762, 1);
        sub_sub_step_up();
        break;
    case 7:
        if (get_CD_State2() & 1) {
            dec_bg_init(1);
            sub_sub_step_up();
        }
        break;
    case 8:
        cd_read_start2(27, (void *)0x80197000, 0xB03E);
        sub_sub_step_up();
        break;
    case 9:
        sprite_fade_main();
        break;
    case 10:
        if (get_CD_State2() & 1) {
            master_intro_res_set();
            t0 = 0;
            do {
                aidx = t0 + 16;
                aidx8 = (u8)aidx;
                aidx = aidx8;
                palette_upload_enqueue(master_intro_pal_ptr, aidx, 1, 1, t0);
                t0 = t0 + 1;
            } while (t0 != 5);
            class_clear_n(96);
            cal_cw_prio = 3;
            cal_cw_attr = 0x01000000;
            cal_cw_bright = 128;
            cal_cw_sel = 128;
            class_w[96].scale_x = 4096;
            class_w[96].scale_y = 4096;
            cal_cw_rate = 1;
            cal_cw_x = -96;
            cal_cw_y = 32;
            cal_cw_img = master_intro_res_tbl[1];
            cal_cw_anim_tbl = master_intro_res_tbl[2];
            cal_cw_hdr = master_intro_res_tbl[0];
            cal_cw_aux14 = (*(s16 *)&master_intro_res_tbl[3]);
            sub_sub_step_up();
        }
        break;
    case 11:
        fval = bukatu_pt6;
        v0 = (u8*)((u32)screen_w | 0);
        master_cw_no = 0;
        t8 = (u32)(fval << 5);
        t9 = t8 >> 30;
        t4 = v0[1671];
        t1 = (s32)t9 + 1;
        t2 = t1 << 1;
        t3_val = t2 & 0x0006;
        t5 = t4 & 0xFFF9;
        t6v = t3_val | t5;
        v0[1671] = (u8)t6v;
        sub_step_up();
        break;
    default:
        game_reset();
        break;
    }
    common_disp();
}
