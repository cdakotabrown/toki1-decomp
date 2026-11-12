/* @SymbolStart: 0x8015706C */
/* @RodataBase: 0x8015FC70 */
/* Address: 0x80157074 */
/* Size: 2028 bytes (507 insns) */
/* @SymbolSize: 2036 */
/* @PostProc: swap_instructions --offset1 0x2e8 --offset2 0x2ec */
/* @PostProc: swap_instructions --offset1 0x2f0 --offset2 0x2f4 */
/* @PostProc: patch_insn_reg_field 0x2d8:rt:t1:t6 0x2dc:rs:t1:t6 0x2dc:rt:t1:t6 0x2e4:rt:t2:t7 0x2e8:rt:t1:t6 0x2ec:rs:t2:t7 0x2ec:rt:t2:t7 0x2f0:rt:t4:v1 0x2f8:rs:t4:v1 0x2f8:rt:t4:t1 0x2fc:rt:t2:t7 0x304:rt:t3:t8 0x308:rt:t3:t8 0x30c:rs:t4:t1 0x30c:rt:t5:t3 0x310:rs:t5:t3 0x310:rt:t6:t5 0x318:rt:t6:t5 0x32c:rt:t7:t9 0x334:rt:t8:t0 0x338:rs:t8:t0 0x338:rt:t8:t0 0x33c:rt:t7:t9 0x344:rt:t8:t0 0x34c:rt:t9:t1 0x350:rt:t9:t1 0x36c:rt:t0:t2 0x370:rt:t1:t3 0x374:rt:t1:t3 0x378:rt:t0:t2 0x3a4:rt:t2:t4 0x3b0:rt:t2:t4 0x430:rt:t3:t5 0x434:rs:t3:t5 0x478:rt:t4:t6 0x484:rt:t4:t6 0x4e0:rt:t5:t7 0x4e4:rs:t5:t7 0x4fc:rd:t6:t8 0x504:rt:t6:t8 0x508:rt:t6:t8 0x510:rs:t6:t8 0x518:rt:t7:t9 0x524:rt:t7:t9 0x528:rt:t8:t0 0x534:rt:t8:t0 0x538:rt:t9:t1 0x544:rt:t9:t1 0x548:rt:t0:t2 0x550:rt:t0:t2 0x57c:rt:t1:t3 0x580:rs:t1:t3 0x580:rt:t1:t3 0x588:rs:t1:t3 0x588:rt:t2:t4 0x58c:rs:t2:t4 0x5a4:rd:t3:t5 0x5ac:rt:t3:t5 0x5b0:rt:t3:t5 0x5b8:rs:t3:t5 0x5c0:rt:t4:t6 0x5cc:rt:t4:t6 0x5d0:rt:t5:t7 0x5dc:rt:t5:t7 0x5e0:rt:t6:t8 0x5ec:rt:t6:t8 0x610:rd:t7:t9 0x618:rt:t7:t9 0x61c:rt:t7:t9 0x624:rs:t7:t9 0x62c:rt:t8:t0 0x638:rt:t8:t0 0x63c:rt:t9:t1 0x648:rt:t9:t1 0x64c:rt:t0:t2 0x658:rt:t0:t2 0x67c:rt:t1:t3 0x680:rs:t1:t3 0x680:rt:t1:t3 0x684:rt:t3:t5 0x688:rs:t1:t3 0x688:rt:t2:t4 0x68c:rs:t2:t4 0x690:rt:t7:t0 0x6a4:rs:t3:t5 0x6a4:rt:t3:t5 0x6a8:rt:t5:t8 0x6ac:rd:t4:t7 0x6ac:rt:t3:t5 0x6b0:rs:t4:t7 0x6b4:rt:t9:t2 0x6b8:rs:t5:t8 0x6b8:rt:t5:t8 0x6c0:rs:t5:t8 0x6c0:rt:t6:t9 0x6c4:rt:t6:t9 0x6c8:rs:t7:t0 0x6c8:rt:t7:t0 0x6d0:rd:t8:t1 0x6d0:rt:t7:t0 0x6d4:rs:t8:t1 0x6dc:rs:t9:t2 0x6dc:rt:t9:t2 0x6e4:rs:t9:t2 0x6e4:rt:t0:t3 0x6e8:rt:t0:t3 0x6fc:rt:t1:t4 0x700:rs:t1:t4 0x700:rt:t1:t4 0x708:rs:t1:t4 0x708:rt:t2:t5 0x70c:rs:t2:t5 0x724:rt:t4:t7 0x728:rt:t3:t6 0x734:rt:t3:t6 0x73c:rt:t4:t7 0x760:rt:t5:t8 0x764:rs:t5:t8 0x764:rt:t5:t8 0x76c:rs:t5:t8 0x76c:rt:t6:t9 0x770:rs:t6:t9 0x7cc:rt:t7:v1 0x7d0:rs:t7:v1 0x7d0:rt:t7:t0 0x7d8:rs:t7:t0 0x7d8:rt:t8:t2 0x7e0:rt:t8:t2 */
#include "types.h"

extern u8 now_sub_sub_step;

extern void sub_sub_step_up(void);
extern void goto_sub_step(s32);
extern void goto_sub_sub_step(s32);
extern void device_cmd_send(s32);
extern void device_load_start(s32, s32, s32, s32, s32, s32);
extern s32 device_load_poll(void);
extern s32 get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void game_reset(void);
extern void set_dec_bri(s32);
extern void dec_bg_reset(void);
extern void indexed_byte_lsb_set(s32, s32);
extern void value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void bustup_ptr_init(void);
extern void fade_up_main(void);
extern void fade_down_main(void);
extern void fade_in_step(void);
extern void fade_in(void);
extern void fade_out_step(void);
extern void zyotai_sprite_set_chk(void);
extern void date_name_set(s32);
extern void yazirushi_off(void);
extern void serifu_run(void);
extern void date_x_taku_set(s32, s32);
extern s32 get_g_zyotai_low(s32);
extern void bustup_load_step(s32);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);
extern void bunkasai_girl1_clubs3_5_srf_bank_set(void);

extern s16 now_serifu;
extern u8  ret_step;
extern u8  ret_sub_step;
extern u8  ret_sub_sub_step;
extern u8  now_step;
extern u8  now_sub_step;
extern u8  now_girl;
extern s16 girl1_syoushin;
extern s32 girl1_flag;
extern u8  girl1_basyo;
extern u8  bunkaken_flags;
extern s32 common_disp_off;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

void bunkasai_girl1_club3_96_scene(void)
{
    s32 v0;

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
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            sub_sub_step_up();
        }
        break;
    case 3:
        bustup_load_step(23873);
        break;
    case 4:
        bustup_ptr_init();
        now_girl = 1;
        zyotai_sprite_set_chk();
        bunkasai_basyo_init();
        now_serifu = 42;
        break;
    case 5:
        fade_in_step();
        break;
    case 6:
        serifu_run();
        break;
    case 7:
        cd_read_start2(18, 0x801E8000, 19095);
        sub_sub_step_up();
        break;
    case 8:
        fade_up_main();
        break;
    case 9:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(1);
            switch (v0) {
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
            case 4:
                now_serifu = 5;
                break;
            }
            bunkasai_girl1_clubs3_5_srf_bank_set();
        }
        break;
    case 10:
        serifu_run();
        break;
    case 11:
        v0 = get_g_zyotai_low(1);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 49;
            break;
        case 2:
            now_serifu = 57;
            break;
        case 3:
            now_serifu = 65;
            break;
        case 4:
            now_serifu = 73;
            break;
        }
        sub_sub_step_up();
        break;
    case 12:
        serifu_run();
        break;
    case 13:
        if (get_g_zyotai_low(1) == 4) {
            fade_down_main();
        } else {
            goto_sub_sub_step(15);
        }
        break;
    case 14:
        serifu_run();
        break;
    case 15:
        if (get_g_zyotai_low(1) == 4) {
            sub_sub_step_up();
        } else {
            fade_down_main();
        }
        break;
    case 16:
        ret_step = now_step;
        ret_sub_step = now_sub_step;
        ret_sub_sub_step = now_sub_sub_step + 1;
        bunkaken_flags = (bunkaken_flags | 2) & 0xFFE3;
        goto_sub_step(164);
        break;
    case 17:
        common_disp_off = 1;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_sub_step(163);
        break;
    case 18:
        device_cmd_send(191);
        device_load_start(1, 0xC486, 0xC459, 0xD266, 0xD216, 0xD20E);
        date_x_taku_set(1, 0);
        bunkasai_basyo_init();
        mes_base_y = 192;
        message_window_init();
        hizuke_init();
        yazirushi_off();
        date_name_set(1);
        common_disp_off = 0;
        sub_sub_step_up();
        break;
    case 19:
        if (device_load_poll() == 1) {
            device_cmd_send(512);
            dec_bg_reset();
            set_dec_bri(0);
            value_cache_find_or_load(16645, 1);
            sub_sub_step_up();
        }
        break;
    case 20:
        if (get_CD_State2() & 1) {
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            sub_sub_step_up();
        }
        break;
    case 21:
        bustup_load_step(23873);
        break;
    case 22:
        bustup_ptr_init();
        now_girl = 1;
        zyotai_sprite_set_chk();
        cd_read_start2(18, 0x801E8000, 19095);
        break;
    case 23:
        if (get_g_zyotai_low(1) == 4) {
            fade_in_step();
        } else {
            fade_in();
        }
        break;
    case 24:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(1);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 50;
                break;
            case 2:
                now_serifu = 58;
                break;
            case 3:
                now_serifu = 66;
                break;
            case 4:
                now_serifu = 75;
                break;
            }
            bunkasai_girl1_clubs3_5_srf_bank_set();
        }
        break;
    case 25:
        serifu_run();
        break;
    case 26:
        sub_sub_step_up();
        if ((girl1_syoushin & 3) == 0) {
            v0 = get_g_zyotai_low(1);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 56;
                break;
            case 2:
                now_serifu = 64;
                break;
            case 3:
                now_serifu = 72;
                break;
            case 4:
                goto_sub_sub_step(31);
                break;
            }
        } else {
            v0 = get_g_zyotai_low(1);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 51;
                break;
            case 2:
                now_serifu = 59;
                break;
            case 3:
                now_serifu = 67;
                break;
            case 4:
                goto_sub_sub_step(31);
                break;
            }
        }
        break;
    case 27:
        serifu_run();
        break;
    case 28:
        if ((girl1_syoushin & 3) == 0) {
            fade_down_main();
        } else {
            if ((girl1_flag << 29) >= 0) {
                now_serifu = now_serifu + 1;
            }
            if (((u32) girl1_basyo >> 4) == 1) {
                now_serifu = now_serifu + 2;
            }
            sub_sub_step_up();
        }
        break;
    case 29:
        if ((girl1_syoushin & 3) == 0) {
            v0 = get_g_zyotai_low(1);
            if ((u32) v0 < 2) {
                now_serifu = 13;
            } else {
                now_serifu = 12;
            }
            sub_sub_step_up();
        } else {
            serifu_run();
        }
        break;
    case 30:
        if ((girl1_syoushin & 3) == 0) {
            serifu_run();
        } else {
            fade_down_main();
        }
        break;
    case 31:
        fade_out_step();
        break;
    case 32:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 33:
        serifu_run();
        break;
    case 34:
        bunkaken_flags = bunkaken_flags | 1;
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
