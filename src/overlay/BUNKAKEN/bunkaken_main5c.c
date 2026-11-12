/* @SymbolStart: 0x80147954 */
/* @RodataBase: 0x80154E70 */
/* Address: 0x8014795C */
/* Size: 1912 bytes (478 insns) */
/* @SymbolSize: 1920 */
/* @PostProc: swap_instructions --offset1 0x1b4 --offset2 0x1b8 */
/* @PostProc: swap_instructions --offset1 0x1b0 --offset2 0x1b4 */
/* @PostProc: swap_instructions --offset1 0x1d4 --offset2 0x1d8 */
/* @PostProc: swap_instructions --offset1 0x42c --offset2 0x430 */
/* @PostProc: swap_instructions --offset1 0x434 --offset2 0x438 */
/* @PostProc: patch_insn_reg_field 0x1a0:rt:t4:t0 0x1a4:rs:t4:t0 0x1a4:rt:t4:t0 0x1ac:rt:t5:t1 0x1b0:rt:t4:t0 0x1b4:rs:t5:t1 0x1b4:rt:t5:t1 0x1b8:rt:t7:v1 0x1bc:rs:t7:v1 0x1bc:rt:t7:t4 0x1c4:rt:t5:t1 0x1cc:rt:t6:t2 0x1d0:rs:t7:t4 0x1d0:rt:t8:t6 0x1d4:rs:t8:t6 0x1d4:rt:t9:t7 0x1d8:rt:t6:t2 0x1dc:rs:t9:t7 0x1dc:rt:t0:t9 0x1e4:rt:t0:t9 0x1f8:rt:t1:t3 0x200:rt:t2:t4 0x204:rs:t2:t4 0x204:rt:t2:t4 0x208:rt:t1:t3 0x210:rt:t2:t4 0x218:rt:t3:t5 0x21c:rt:t3:t5 0x238:rt:t4:t6 0x23c:rt:t5:t7 0x240:rt:t5:t7 0x244:rt:t4:t6 0x270:rt:t6:t8 0x27c:rt:t6:t8 0x2fc:rt:t7:t9 0x300:rs:t7:t9 0x344:rt:t8:t0 0x350:rt:t8:t0 0x388:rt:t9:t1 0x38c:rs:t9:t1 0x3b0:rt:t1:t3 0x3bc:rt:t2:t4 0x3c8:rt:t0:t2 0x3d4:rt:t0:t2 0x3e0:rt:t1:t3 0x3e8:rt:t2:t4 0x41c:rt:t3:t0 0x420:rs:t3:t0 0x420:rt:t3:t0 0x428:rt:t4:t1 0x42c:rt:t3:t0 0x430:rs:t4:t1 0x430:rt:t4:t1 0x434:rt:t6:v1 0x43c:rs:t6:v1 0x43c:rt:t6:t5 0x440:rt:t4:t1 0x448:rt:t5:t2 0x44c:rt:t5:t2 0x450:rs:t6:t5 0x454:rt:t8:t9 0x45c:rt:t8:t9 0x470:rt:t9:t3 0x478:rt:t0:t4 0x47c:rs:t0:t4 0x47c:rt:t0:t4 0x480:rt:t9:t3 0x488:rt:t0:t4 0x490:rt:t1:t5 0x494:rt:t1:t5 0x4b0:rt:t2:t6 0x4b4:rt:t3:t7 0x4b8:rt:t3:t7 0x4bc:rt:t2:t6 0x4e8:rt:t4:t8 0x4f4:rt:t4:t8 0x574:rt:t5:t9 0x578:rs:t5:t9 0x5bc:rt:t6:t0 0x5c8:rt:t6:t0 0x600:rt:t7:t1 0x604:rs:t7:t1 0x618:rt:t1:t5 0x62c:rt:t9:t3 0x638:rt:t0:t4 0x644:rt:t8:t2 0x650:rt:t8:t2 0x65c:rt:t9:t3 0x664:rt:t0:t4 0x668:rs:t1:t5 0x668:rt:t1:t5 0x66c:rt:t2:t6 0x670:rs:t1:t5 0x67c:rs:t2:t6 0x67c:rt:t2:t6 0x684:rs:t2:t6 0x684:rt:t3:t7 0x688:rt:t3:t7 0x6c8:rt:t5:t9 0x6d4:rt:t6:t0 0x6e0:rt:t4:t8 0x6ec:rt:t4:t8 0x6f8:rt:t5:t9 0x700:rt:t6:t0 0x758:rt:t7:v1 0x75c:rs:t7:v1 0x75c:rt:t7:t1 0x764:rs:t7:t1 0x764:rt:t8:t3 0x76c:rt:t8:t3 */
#include "types.h"

extern u8 now_sub_sub_step;

extern s16 now_serifu;
extern u8  basyo_pack;
extern u8  ret_step;
extern u8  ret_sub_step;
extern u8  ret_sub_sub_step;
extern u8  now_girl;
extern u8  now_step;
extern u8  now_sub_step;
extern u8  tt_result;
extern u8  bunkaken_flags;
extern s32 common_disp_off;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

extern s32 sub_sub_step_up(void);
extern s32 goto_step(s32);
extern s32 goto_sub_step(s32);
extern s32 device_cmd_send(s32);
extern s32 device_load_start(s32, s32, s32, s32, s32, s32);
extern s32 device_load_poll(void);
extern s32 get_CD_State2(void);
extern s32 cd_read_start2(s32, s32, s32);
extern s32 game_reset(void);
extern s32 set_dec_bri(s32);
extern s32 dec_bg_reset(void);
extern s32 indexed_byte_lsb_set(s32, s32);
extern s32 value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);
extern s32 hizuke_init(void);
extern s32 message_window_init(void);
extern s32 bustup_ptr_init(void);
extern s32 fade_down_main(void);
extern s32 fade_in(void);
extern s32 fade_out(void);
extern s32 zyotai_sprite_set_chk(void);
extern s32 yazirushi_off(void);
extern s32 serifu_run(void);
extern s32 date_x_taku_set(s32, s32);
extern s32 get_g_zyotai_low(s32);
extern s32 bustup_load_step(s32);
extern s32 date_name_set(s32);
extern s32 bunkaken_sub_reset(void);
extern s32 bunkaken_basyo_init3(void);
extern s32 bunkaken_param_set(void);

void bunkaken_main5c(void)
{
    s32 v0;

    switch ((u32)now_sub_sub_step) {
    case 0:
        value_cache_find_or_load(16645, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out();
        break;
    case 2:
        v0 = get_CD_State2();
        if (v0 & 1) {
            bunkaken_basyo_init3();
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            cd_read_start2(16, 0x801E8000, 19235);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in();
        break;
    case 4:
        v0 = get_CD_State2();
        if (v0 & 1) {
            v0 = get_g_zyotai_low(6);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 63;
                break;
            case 2:
                now_serifu = 64;
                break;
            case 3:
                now_serifu = 65;
                break;
            }
            bunkaken_param_set();
        }
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        now_serifu = now_serifu + 2;
        sub_sub_step_up();
        break;
    case 7:
        serifu_run();
        break;
    case 8:
        fade_out();
        break;
    case 9:
        ret_step = now_step;
        ret_sub_step = now_sub_step;
        ret_sub_sub_step = now_sub_sub_step + 1;
        bunkaken_flags = ((bunkaken_flags | 2) & 0xFFE3) | 8;
        goto_step(208);
        break;
    case 10:
        common_disp_off = 1;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_sub_step(140);
        break;
    case 11:
        device_cmd_send(191);
        device_load_start(1, 0xC486, 0xC459, 0xD266, 0xD216, 0xD20E);
        date_x_taku_set(1, 0);
        bunkaken_basyo_init3();
        mes_base_y = 192;
        message_window_init();
        hizuke_init();
        yazirushi_off();
        date_name_set(6);
        common_disp_off = 0;
        sub_sub_step_up();
        break;
    case 12:
        v0 = device_load_poll();
        if (v0 == 1) {
            device_cmd_send(512);
            dec_bg_reset();
            set_dec_bri(0);
            value_cache_find_or_load(16645, 1);
            sub_sub_step_up();
        }
        break;
    case 13:
        v0 = get_CD_State2();
        if (v0 & 1) {
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            sub_sub_step_up();
        }
        break;
    case 14:
        bustup_load_step(25133);
        break;
    case 15:
        bustup_ptr_init();
        now_girl = 6;
        zyotai_sprite_set_chk();
        cd_read_start2(16, 0x801E8000, 19235);
        break;
    case 16:
        fade_in();
        break;
    case 17:
        v0 = get_CD_State2();
        if (v0 & 1) {
            v0 = get_g_zyotai_low(6);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 72;
                break;
            case 2:
                now_serifu = 73;
                break;
            case 3:
                now_serifu = 74;
                break;
            }
            bunkaken_param_set();
        }
        break;
    case 18:
        serifu_run();
        break;
    case 19:
        fade_down_main();
        break;
    case 20:
        ret_step = now_step;
        ret_sub_step = now_sub_step;
        ret_sub_sub_step = now_sub_sub_step + 1;
        bunkaken_flags = (bunkaken_flags | 2) & 0xFFE3;
        goto_sub_step(141);
        break;
    case 21:
        common_disp_off = 1;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_sub_step(140);
        break;
    case 22:
        device_cmd_send(191);
        device_load_start(1, 0xC486, 0xC459, 0xD266, 0xD216, 0xD20E);
        date_x_taku_set(1, 0);
        bunkaken_basyo_init3();
        mes_base_y = 192;
        message_window_init();
        hizuke_init();
        yazirushi_off();
        date_name_set(6);
        common_disp_off = 0;
        sub_sub_step_up();
        break;
    case 23:
        v0 = device_load_poll();
        if (v0 == 1) {
            device_cmd_send(512);
            dec_bg_reset();
            set_dec_bri(0);
            value_cache_find_or_load(16645, 1);
            sub_sub_step_up();
        }
        break;
    case 24:
        v0 = get_CD_State2();
        if (v0 & 1) {
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            sub_sub_step_up();
        }
        break;
    case 25:
        bustup_load_step(25133);
        break;
    case 26:
        bustup_ptr_init();
        now_girl = 6;
        zyotai_sprite_set_chk();
        cd_read_start2(16, 0x801E8000, 19235);
        break;
    case 27:
        fade_in();
        break;
    case 28:
        v0 = get_CD_State2();
        if (v0 & 1) {
            v0 = get_g_zyotai_low(6);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 75;
                break;
            case 2:
                now_serifu = 77;
                break;
            case 3:
                now_serifu = 79;
                break;
            }
            if ((u32)tt_result < 2U) {
                now_serifu = now_serifu + 1;
            }
            bunkaken_param_set();
        }
        break;
    case 29:
        serifu_run();
        break;
    case 30:
        v0 = get_g_zyotai_low(6);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 81;
            break;
        case 2:
            now_serifu = 82;
            break;
        case 3:
            now_serifu = 83;
            break;
        }
        sub_sub_step_up();
        break;
    case 31:
        serifu_run();
        break;
    case 32:
        fade_out();
        break;
    case 33:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 34:
        serifu_run();
        break;
    case 35:
        bunkaken_flags = bunkaken_flags | 1;
        bunkaken_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
