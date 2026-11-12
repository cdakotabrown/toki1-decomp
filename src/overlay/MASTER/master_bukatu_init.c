/* master_bukatu_init - MASTER overlay, Address: 0x801391E4, Size: 1580 bytes */
/* @RodataBase: 0x8013C12C */
/* @PostProc: patch_stack_complete --old 48 --new 72 */
/* @PostProc: remap_stack_slots --map 40:60 */
/* @PostProc: swap_instructions --offset1 0x38c --offset2 0x394 */
/* @PostProc: swap_instructions --offset1 0x390 --offset2 0x398 */
/* @PostProc: swap_instructions --offset1 0x39c --offset2 0x3a0 */
/* @PostProc: patch_insn_reg_field 0x11c:rt:v0:v1 0x120:rs:v0:v1 0x120:rt:v0:v1 0x128:rt:v0:v1 0x264:rt:t2:t0 0x384:rt:t8:t7 0x388:rt:t9:t8 0x394:rs:t9:t8 0x394:rt:t9:t8 0x398:rs:t8:t7 0x398:rt:t8:t7 0x39c:rd:t7:v0 0x3a0:rd:t0:v1 0x3a0:rs:t8:t7 0x3a0:rt:t9:t8 0x3a4:rs:t7:v0 0x3a4:rt:t0:v1 0x3ac:rt:t1:t9 0x3c0:rt:t1:t9 0x3dc:rs:t2:t0 0x3dc:rt:t2:t0 0x3e0:rt:t3:t1 0x3e4:rs:t2:t0 0x3ec:rt:t5:t2 0x3f8:rt:t3:t1 0x400:rt:t5:t2 0x404:rt:v0:t3 0x408:rs:v0:t3 0x408:rt:v0:t3 0x410:rs:v0:t3 0x418:rt:v0:t3 0x41c:rt:t4:t5 0x420:rt:t6:t4 0x424:rd:t6:t4 0x424:rs:t6:t4 0x428:rd:t4:t5 0x428:rs:t4:t5 0x430:rs:t4:t5 0x430:rt:t4:t5 0x434:rs:t6:t4 0x434:rt:t6:t4 0x448:rt:t4:t5 0x450:rt:t6:t4 0x474:rt:t8:t6 0x478:rs:t8:t6 0x478:rt:t8:t6 0x480:rs:t8:t6 0x4e0:rt:t9:t7 0x4e4:rs:t9:t7 0x504:rt:t7:t8 0x508:rs:t7:t8 0x508:rt:t7:t8 0x510:rt:t0:t9 0x514:rs:t0:t9 0x514:rt:t0:t9 0x518:rt:t7:t8 0x520:rt:t1:t0 0x524:rs:t1:t0 0x524:rt:t1:t0 0x528:rt:v0:v1 0x52c:rt:t0:t9 0x530:rs:v0:v1 0x530:rt:v0:v1 0x538:rt:t1:t0 0x540:rd:t2:t1 0x540:rt:v0:v1 0x544:rs:t2:t1 0x548:rt:t3:t2 0x550:rs:t2:t1 0x554:rt:t5:t3 0x55c:rs:t2:t1 0x560:rt:t4:t5 0x568:rs:t2:t1 0x56c:rt:t6:t4 0x580:rt:t3:t2 0x58c:rt:t5:t3 0x598:rt:t4:t5 0x5a4:rt:t6:t4 0x5e8:rd:v1:t7 0x5ec:rs:v1:t7 0x5fc:rt:v1:t7 */
#include "types.h"

typedef struct {
    u8 pad00[8];
    union {
        s32 w;
        u8 b[4];
    } f08;
    u8 pad0C[44];
} MasterRec;

extern u8 now_sub_sub_step;
extern s32 vblnk_counter;
extern u16 basyo_pack;
extern u8 denwa_cnt;
extern u8 girl11_tel_cnt;
extern u8 girl0_basyo;
extern s16 girl0_tokimeki;
extern s16 girl0_yuukou;
extern s16 girl10_tokimeki;
extern s16 girl10_yuukou;
extern MasterRec screen_w[];

extern s32 girl_head_tbl[];
extern s32 girl_body_tbl[];
extern s32 girl_seq_tbl[];

extern s16 now_serifu;
extern s16 now_serifu_in;
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;

extern u8 master_bukatu_init_str0[];
extern u8 master_bukatu_init_str1[];
extern u8 master_bukatu_init_str2[];
extern u8 master_bukatu_init_str3[];
extern u8 master_bukatu_init_str4[];
extern s32 master_bukatu_res_tbl[];
extern u8 master_bukatu_girl_no;
extern s16 master_bukatu_serifu_no;

extern void bustup_ani(void);
extern void device_cmd_send(s32);
extern void device_load_start(s32, s32, s32, s32, s32, s32);
extern void date_x_taku_set(s32, s32);
extern void dec_bg_reset(void);
extern void set_dec_bri(s32);
extern void message_window_init(void);
extern void k_reset(s32);
extern void basyo_init(u8 *);
extern void mes_window_switch(s32);
extern void hizuke_init(void);
extern void parameter_disp_switch(s32);
extern void icon_disp_switch(s32);
extern void shared_system_state_reset(void);
extern void srand(s32);
extern void sub_sub_step_up(void);
extern s32 device_load_poll(void);
extern void value_cache_find_or_load(s32, s32);
extern void game_reset(void);
extern s32 get_CD_State2(void);
extern s32 get_top_girl(void);
extern u32 get_g_zyotai_low(s32);
extern s32 dec_bg_init(s32);
extern void indexed_byte_lsb_set(s32, s32);
extern void cd_read_start2(s32, void *, s32);
extern void fade_in_step(void);
extern void date_name_set(s32);
extern void yazirushi_off(void);
extern void master_serifu_tbl_set(void);
extern void serifu_run(void);
extern s32 rand(void);
extern void sub_step_up(void);
extern void common_disp(void);

void master_bukatu_init(void)
{
    s32 i;
    s32 sel;
    s32 v;

    bustup_ani();

    switch ((u32)now_sub_sub_step) {
    case 0:
        device_cmd_send(191);
        device_load_start(1, 0xC051, 0xC02B, 0xC990, 0xC943, 0xC929);
        date_x_taku_set(1, 0);
        dec_bg_reset();
        set_dec_bri(0);
        message_window_init();
        k_reset(0);
        basyo_init(master_bukatu_init_str0);
        mes_window_switch(0);
        hizuke_init();
        parameter_disp_switch(0);
        icon_disp_switch(0);
        shared_system_state_reset();
        srand(vblnk_counter);
        sub_sub_step_up();
        break;
    case 1:
        if (device_load_poll() != 1) {
            break;
        }
        k_reset(0);
        switch ((u32)basyo_pack >> 12) {
        case 7:
            basyo_init(master_bukatu_init_str1);
            device_cmd_send(519);
            value_cache_find_or_load(16817, 1);
            break;
        case 8:
            basyo_init(master_bukatu_init_str2);
            device_cmd_send(521);
            value_cache_find_or_load(16826, 1);
            break;
        case 9:
            basyo_init(master_bukatu_init_str3);
            device_cmd_send(520);
            value_cache_find_or_load(16826, 1);
            break;
        case 10:
            basyo_init(master_bukatu_init_str4);
            device_cmd_send(513);
            value_cache_find_or_load(16826, 1);
            break;
        default:
            game_reset();
            break;
        }
        mes_window_switch(0);
        sub_sub_step_up();
        break;
    case 2:
        if (!(get_CD_State2() & 1)) {
            break;
        }
        sel = get_top_girl();
        if (denwa_cnt >= 65U) {
            master_bukatu_girl_no = 12;
        } else if (get_g_zyotai_low(sel) < 2) {
            master_bukatu_girl_no = (u8)sel;
            for (i = 0; i != 11; i++) {
                if (((u32)basyo_pack >> 12) == ((u32)screen_w[i + 8].f08.b[2] >> 4)) {
                    if (((u32)(screen_w[i + 8].f08.w << 30) >> 31) == 1) {
                        if (get_g_zyotai_low(i) < 2) {
                            master_bukatu_girl_no = (u8)i;
                        }
                        if (((u32)girl0_basyo >> 4) == 10) {
                            if (((u32)basyo_pack >> 12) == 10) {
                                if (get_g_zyotai_low(0) < 2 &&
                                    get_g_zyotai_low(10) >= 2) {
                                    master_bukatu_girl_no = 0;
                                    continue;
                                }
                                if (get_g_zyotai_low(0) >= 2 &&
                                    get_g_zyotai_low(10) < 2) {
                                    master_bukatu_girl_no = 10;
                                    continue;
                                }
                                if (get_g_zyotai_low(0) < 2) {
                                    if (get_g_zyotai_low(10) < 2) {
                                        if (girl10_tokimeki + girl10_yuukou <=
                                            girl0_tokimeki + girl0_yuukou) {
                                            master_bukatu_girl_no = 0;
                                        } else {
                                            master_bukatu_girl_no = 10;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        } else if ((u32)girl11_tel_cnt >= 2) {
            master_bukatu_girl_no = 11;
        } else {
            master_bukatu_girl_no = 255;
        }
        v = master_bukatu_girl_no;
        if (v != 255) {
            device_load_start(0, 0xBFA6, 0xBF87, girl_head_tbl[v], girl_body_tbl[v],
                          girl_seq_tbl[v]);
        }
        sub_sub_step_up();
        break;
    case 3:
        if (device_load_poll() != 1 && master_bukatu_girl_no != 255) {
            break;
        }
        indexed_byte_lsb_set(1, dec_bg_init(1));
        cd_read_start2(11, (void *)0x801E8000, 0xA9C5);
        mes_window_switch(1);
        sub_sub_step_up();
        break;
    case 4:
        fade_in_step();
        break;
    case 5:
        if (!(get_CD_State2() & 1)) {
            break;
        }
        date_name_set(0);
        yazirushi_off();
        master_serifu_tbl_set();
        ss_serifu = master_bukatu_res_tbl[0];
        ss_serifu_kind = master_bukatu_res_tbl[1];
        sss_sound = master_bukatu_res_tbl[2];
        switch ((u32)basyo_pack >> 12) {
        case 7:
            now_serifu = 1;
            break;
        case 8:
            now_serifu = 2;
            break;
        case 9:
            now_serifu = 3;
            break;
        case 10:
            now_serifu = 4;
            break;
        default:
            game_reset();
            break;
        }
        now_serifu_in = 0;
        sub_sub_step_up();
        break;
    case 6:
    case 8:
        serifu_run();
        break;
    case 7:
        v = rand() % 6;
        now_serifu = (s16)(v + 6);
        master_bukatu_serifu_no = (s16)v;
        sub_sub_step_up();
        break;
    case 9:
        sub_step_up();
        break;
    default:
        game_reset();
        break;
    }

    common_disp();
}
