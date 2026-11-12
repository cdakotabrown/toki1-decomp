/* @SymbolStart: 0x8014C3B4 */
/* @RodataBase: 0x801554C8 */
/* @SymbolSize: 2008 */
/* Address: 0x8014C3BC */
/* Size: 2000 bytes (500 insns) */
/* @PostProc: swap_instructions --offset1 0x364 --offset2 0x368 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rt:v0:v1 0x210:rt:t8:t9 0x214:rd:t7:t8 0x218:rs:t7:t8 0x230:rs:t8:t9 0x230:rt:t8:t0 0x234:rt:t0:t2 0x238:rs:t8:t0 0x238:rt:t9:t1 0x23c:rt:t9:t1 0x248:rt:t0:t2 0x280:rt:t2:t4 0x28c:rt:t3:t5 0x298:rt:t1:t3 0x2a4:rt:t1:t3 0x2b0:rt:t2:t4 0x2b8:rt:t3:t5 0x2cc:rt:t4:t6 0x2d0:rs:t4:t6 0x2d0:rt:t4:t6 0x2d8:rs:t4:t6 0x320:rt:t5:t7 0x324:rs:t5:t7 0x324:rt:t5:t7 0x32c:rs:t5:t7 0x354:rt:t6:t3 0x358:rs:t6:t3 0x358:rt:t6:t3 0x360:rt:t7:t4 0x364:rt:t6:t3 0x368:rs:t7:t4 0x368:rt:t7:t4 0x36c:rt:at:v0 0x370:rt:t9:at 0x374:rs:t9:v0 0x374:rt:t9:t8 0x378:rt:t7:t4 0x380:rs:v0:v1 0x380:rt:t8:t5 0x384:rt:t8:t5 0x388:rs:t9:t8 0x38c:rt:t1:t2 0x394:rt:t1:t2 0x3a8:rt:t2:t6 0x3b0:rt:t3:t7 0x3b4:rs:t3:t7 0x3b4:rt:t3:t7 0x3b8:rt:t2:t6 0x3c0:rt:t3:t7 0x3c8:rs:v0:v1 0x3c8:rt:t4:t8 0x3cc:rt:t4:t8 0x3e8:rt:t5:t9 0x3ec:rt:t6:t0 0x3f0:rt:t6:t0 0x3f4:rt:t5:t9 0x420:rt:t7:t1 0x42c:rt:t7:t1 0x4ac:rt:t8:t2 0x4b0:rs:t8:t2 0x52c:rt:t9:t3 0x530:rs:t9:t3 0x530:rt:t9:t3 0x538:rs:t9:t3 0x568:rt:t0:t4 0x56c:rs:t0:t4 0x598:rt:t2:t6 0x5a4:rt:t3:t7 0x5b0:rt:t1:t5 0x5bc:rt:t1:t5 0x5c8:rt:t2:t6 0x5d0:rt:t3:t7 0x5e4:rt:t4:t8 0x5e8:rs:t4:t8 0x5e8:rt:t4:t8 0x5f0:rs:t4:t8 0x5f4:rt:t5:t9 0x600:rt:t5:t9 0x628:rt:t9:t3 0x63c:rt:t7:t1 0x648:rt:t8:t2 0x654:rt:t6:t0 0x660:rt:t6:t0 0x66c:rt:t7:t1 0x674:rt:t8:t2 0x678:rs:t9:t3 0x678:rt:t9:t3 0x67c:rt:t0:t4 0x680:rs:t9:t3 0x68c:rs:t0:t4 0x68c:rt:t0:t4 0x694:rs:t0:t4 0x694:rt:t1:t5 0x698:rt:t1:t5 0x6ac:rt:t2:t6 0x6b0:rs:t2:t6 0x6b0:rt:t2:t6 0x6b8:rs:t2:t6 0x6fc:rt:t4:t8 0x708:rt:t5:t9 0x714:rt:t3:t7 0x720:rt:t3:t7 0x72c:rt:t4:t8 0x734:rt:t5:t9 0x768:rt:t6:t0 0x76c:rs:t6:t0 0x76c:rt:t6:t0 0x774:rs:t6:t0 */
#include "types.h"

extern void sub_sub_step_up(void);
extern void goto_sub_step(s32);
extern void goto_sub_sub_step(s32);
extern void device_cmd_send(s32);
extern void device_load_start(s32, s32, s32, s32, s32, s32);
extern s32  device_load_poll(void);
extern s32  get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void game_reset(void);
extern void set_dec_bri(s32);
extern void dec_bg_reset(void);
extern void indexed_byte_lsb_set(s32, s32);
extern void value_cache_find_or_load(s32, s32);
extern s32  dec_bg_init(s32);
extern void bustup_cd_read(s32);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void bustup_ptr_init(void);
extern void fade_up_main(void);
extern void fade_down_main(void);
extern void fade_in_step(void);
extern void fade_in(void);
extern void fade_out_step(void);
extern void fade_out(void);
extern void zyotai_sprite_set_chk(void);
extern void date_name_set(s32);
extern void yazirushi_off(void);
extern void serifu_run(void);
extern void date_x_taku_set(s32, s32);
extern s32  get_g_zyotai_low(s32);
extern void bustup_load_step(s32);
extern void bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);
extern void bunkaken_param_set2(void);

extern s16 now_serifu;
extern u8  girl0_flag;
extern u8  ret_step;
extern u8  ret_sub_step;
extern u8  ret_sub_sub_step;
extern u8  now_girl;
extern u8  now_step;
extern u8  now_sub_step;
extern u8  now_sub_sub_step;
extern u8  tt_result;
extern u8  bunkaken_flags;
extern s32 common_disp_off;
extern s32 bunka_girl2;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

void bunkaken_girl0_cl_bk3_96_tt_main(void) {
    s32 n;

    switch (now_sub_sub_step) {
    case 0:
        value_cache_find_or_load(16645, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out_step();
        break;
    case 2:
        if (get_CD_State2() & 1) {
            bunkaken_basyo_init3();
            n = dec_bg_init(1);
            indexed_byte_lsb_set(1, n);
            bustup_cd_read(21578);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            bustup_ptr_init();
            now_girl = 0;
            zyotai_sprite_set_chk();
            cd_read_start2(23, 0x801E8000, 18923);
        }
        break;
    case 5:
        if (get_g_zyotai_low(0) == 4 || bunka_girl2 == 0) {
            sub_sub_step_up();
        } else {
            fade_up_main();
        }
        break;
    case 6:
        if (get_CD_State2() & 1) {
            switch (get_g_zyotai_low(0)) {
            case 0:
            case 1:
                now_serifu = 1;
                break;
            case 2:
                now_serifu = 2;
                break;
            case 3:
                now_serifu = 3;
                break;
            }
            if (get_g_zyotai_low(0) == 4 || bunka_girl2 == 0) {
                now_serifu = 105;
            }
            bunkaken_param_set2();
        }
        break;
    case 7:
        serifu_run();
        break;
    case 8:
        if ((*(s32 *)&girl0_flag << 29) < 0) {
            goto_sub_sub_step(10);
        } else {
            girl0_flag |= 4;
            now_serifu = 4;
            sub_sub_step_up();
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        switch (get_g_zyotai_low(0)) {
        case 0:
        case 1:
            now_serifu = 90;
            break;
        case 2:
            now_serifu = 95;
            break;
        case 3:
            now_serifu = 100;
            break;
        }
        if (get_g_zyotai_low(0) == 4 || bunka_girl2 == 0) {
            goto_sub_sub_step(12);
        } else {
            sub_sub_step_up();
        }
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        if (get_g_zyotai_low(0) == 4 || bunka_girl2 == 0) {
            sub_sub_step_up();
        } else {
            fade_down_main();
        }
        break;
    case 13:
        ret_step = now_step;
        ret_sub_step = now_sub_step;
        ret_sub_sub_step = now_sub_sub_step + 1;
        bunkaken_flags = (bunkaken_flags | 2) & 0xFFE3;
        goto_sub_step(141);
        break;
    case 14:
        common_disp_off = 1;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_sub_step(140);
        break;
    case 15:
        device_cmd_send(191);
        device_load_start(1, 0xC486, 0xC459, 0xD266, 0xD216, 0xD20E);
        date_x_taku_set(1, 0);
        bunkaken_basyo_init3();
        mes_base_y = 192;
        message_window_init();
        hizuke_init();
        yazirushi_off();
        date_name_set(8);
        common_disp_off = 0;
        sub_sub_step_up();
        break;
    case 16:
        if (device_load_poll() == 1) {
            device_cmd_send(512);
            dec_bg_reset();
            set_dec_bri(0);
            value_cache_find_or_load(16645, 1);
            sub_sub_step_up();
        }
        break;
    case 17:
        if (get_CD_State2() & 1) {
            n = dec_bg_init(1);
            indexed_byte_lsb_set(1, n);
            sub_sub_step_up();
        }
        break;
    case 18:
        bustup_load_step(21578);
        break;
    case 19:
        bustup_ptr_init();
        now_girl = 0;
        zyotai_sprite_set_chk();
        cd_read_start2(23, 0x801E8000, 18923);
        break;
    case 20:
        if (get_g_zyotai_low(0) == 4 || bunka_girl2 == 0) {
            fade_in_step();
        } else {
            fade_in();
        }
        break;
    case 21:
        if (get_CD_State2() & 1) {
            bunkaken_param_set2();
            switch (get_g_zyotai_low(0)) {
            case 0:
            case 1:
                now_serifu = 91;
                break;
            case 2:
                now_serifu = 96;
                break;
            case 3:
                now_serifu = 101;
                break;
            }
            if (get_g_zyotai_low(0) == 4 || bunka_girl2 == 0) {
                now_serifu = 106;
            }
        }
        break;
    case 22:
        serifu_run();
        break;
    case 23:
        sub_sub_step_up();
        switch (get_g_zyotai_low(0)) {
        case 0:
        case 1:
            now_serifu = 92;
            break;
        case 2:
            now_serifu = 97;
            break;
        case 3:
            now_serifu = 102;
            break;
        }
        if (tt_result < 2U) {
            now_serifu = now_serifu + 1;
        }
        if (get_g_zyotai_low(0) == 4 || bunka_girl2 == 0) {
            goto_sub_sub_step(27);
        }
        break;
    case 24:
        serifu_run();
        break;
    case 25:
        switch (get_g_zyotai_low(0)) {
        case 0:
        case 1:
            now_serifu = 94;
            break;
        case 2:
            now_serifu = 99;
            break;
        case 3:
            now_serifu = 104;
            break;
        }
        sub_sub_step_up();
        break;
    case 26:
        serifu_run();
        break;
    case 27:
        if (get_g_zyotai_low(0) == 4 || bunka_girl2 == 0) {
            fade_out_step();
        } else {
            fade_out();
        }
        break;
    case 28:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 29:
        serifu_run();
        break;
    case 30:
        bunkaken_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
