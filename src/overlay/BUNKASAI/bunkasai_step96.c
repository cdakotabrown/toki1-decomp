/* @SymbolStart: 0x80159FBC */
/* @RodataBase: 0x80160190 */
/* Address: 0x80159FC4 */
/* Size: 1560 bytes (390 insns) */
/* @SymbolSize: 1568 */
/* @PostProc: swap_instructions --offset1 0x270 --offset2 0x274 */
/* @PostProc: swap_instructions --offset1 0x278 --offset2 0x27c */
/* @PostProc: patch_insn_reg_field 0x260:rt:t6:t1 0x264:rs:t6:t1 0x264:rt:t6:t1 0x26c:rt:t7:t2 0x270:rt:t6:t1 0x274:rs:t7:t2 0x274:rt:t7:t2 0x278:rt:t9:v1 0x280:rs:t9:v1 0x280:rt:t9:t6 0x284:rt:t7:t2 0x28c:rt:t8:t3 0x290:rt:t8:t3 0x294:rs:t9:t6 0x294:rt:t0:t8 0x298:rs:t0:t8 0x298:rt:t1:t0 0x2a0:rt:t1:t0 0x2b4:rt:t2:t4 0x2bc:rt:t3:t5 0x2c0:rs:t3:t5 0x2c0:rt:t3:t5 0x2c4:rt:t2:t4 0x2cc:rt:t3:t5 0x2d4:rt:t4:t6 0x2d8:rt:t4:t6 0x2f4:rt:t5:t7 0x2f8:rt:t6:t8 0x2fc:rt:t6:t8 0x300:rt:t5:t7 0x32c:rt:t7:t9 0x338:rt:t7:t9 0x3b8:rt:t8:t0 0x3bc:rs:t8:t0 0x400:rt:t9:t1 0x40c:rt:t9:t1 0x468:rt:t0:t2 0x46c:rs:t0:t2 0x484:rd:t1:t3 0x48c:rt:t1:t3 0x490:rt:t1:t3 0x498:rs:t1:t3 0x4a0:rt:t2:t4 0x4ac:rt:t2:t4 0x4b0:rt:t3:t5 0x4bc:rt:t3:t5 0x4c0:rt:t4:t6 0x4cc:rt:t4:t6 0x4d0:rt:t5:t7 0x4d8:rt:t5:t7 0x514:rd:t6:t8 0x51c:rt:t6:t8 0x520:rt:t6:t8 0x528:rs:t6:t8 0x530:rt:t7:t9 0x53c:rt:t7:t9 0x540:rt:t8:t0 0x54c:rt:t8:t0 0x550:rt:t9:t1 0x55c:rt:t9:t1 0x5f8:rt:t0:v1 0x5fc:rs:t0:v1 0x5fc:rt:t0:t2 0x604:rs:t0:t2 0x604:rt:t1:t4 0x60c:rt:t1:t4 */
#include "types.h"

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
extern void bunkasai_girl9_srf_bank_set(void);

extern s16 now_serifu;
extern u8  ret_step;
extern u8  ret_sub_step;
extern u8  ret_sub_sub_step;
extern u8  now_girl;
extern u8  now_step;
extern u8  now_sub_step;
extern u8  bunkaken_flags;
extern s32 common_disp_off;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

extern u8 now_sub_sub_step;
void bunkasai_step96(void) {
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
        bustup_load_step(25808);
        break;
    case 4:
        bustup_ptr_init();
        now_girl = 9;
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
        cd_read_start2(10, 0x801E8000, 19251);
        sub_sub_step_up();
        break;
    case 8:
        fade_up_main();
        break;
    case 9:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(9);
            switch (v0) {

                break;
            case 0: case 1:
                now_serifu = 35;
                break;
            case 2:
                now_serifu = 36;
                break;
            case 3:
                now_serifu = 37;
                break;
            case 4:
                now_serifu = 38;
                break;
            }
            bunkasai_girl9_srf_bank_set();
        }
        break;
    case 10:
        serifu_run();
        break;
    case 11:
        v0 = get_g_zyotai_low(9);
        if (v0 == 4) {
            fade_down_main();
        } else {
            goto_sub_sub_step(14);
        }
        break;
    case 12:
        serifu_run();
        break;
    case 13:
        sub_sub_step_up();
        break;
    case 14:
        v0 = get_g_zyotai_low(9);
        if (v0 == 4) {
            sub_sub_step_up();
        } else {
            fade_down_main();
        }
        break;
    case 15:
        ret_step = now_step;
        ret_sub_step = now_sub_step;
        ret_sub_sub_step = now_sub_sub_step + 1;
        bunkaken_flags = (bunkaken_flags | 2) & 0xFFE3;
        goto_sub_step(164);
        break;
    case 16:
        common_disp_off = 1;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_sub_step(163);
        break;
    case 17:
        device_cmd_send(191);
        device_load_start(1, 0xC486, 0xC459, 0xD266, 0xD216, 0xD20E);
        date_x_taku_set(1, 0);
        bunkasai_basyo_init();
        mes_base_y = 192;
        message_window_init();
        hizuke_init();
        yazirushi_off();
        date_name_set(9);
        common_disp_off = 0;
        sub_sub_step_up();
        break;
    case 18:
        if (device_load_poll() == 1) {
            device_cmd_send(512);
            dec_bg_reset();
            set_dec_bri(0);
            value_cache_find_or_load(16645, 1);
            sub_sub_step_up();
        }
        break;
    case 19:
        if (get_CD_State2() & 1) {
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            sub_sub_step_up();
        }
        break;
    case 20:
        bustup_load_step(25808);
        break;
    case 21:
        bustup_ptr_init();
        now_girl = 9;
        zyotai_sprite_set_chk();
        cd_read_start2(10, 0x801E8000, 19251);
        break;
    case 22:
        v0 = get_g_zyotai_low(9);
        if (v0 == 4) {
            fade_in_step();
            break;
        }

        fade_in();
        break;
    case 23:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(9);
            switch (v0) {

                break;
            case 0: case 1:
                now_serifu = 40;
                break;
            case 2:
                now_serifu = 41;
                break;
            case 3:
                now_serifu = 42;
                break;
            case 4:
                now_serifu = 43;
                break;
            }
            bunkasai_girl9_srf_bank_set();
        }
        break;
    case 24:
        serifu_run();
        break;
    case 25:
        sub_sub_step_up();
        v0 = get_g_zyotai_low(9);
        switch (v0) {

            break;
        case 0: case 1:
            now_serifu = 1;
            break;
        case 2:
            now_serifu = 3;
            break;
        case 3:
            now_serifu = 4;
            break;
        case 4:
            goto_sub_sub_step(29);
            break;
        }
        break;
    case 26:
        serifu_run();
        break;
    case 27:
        fade_down_main();
        break;
    case 28:
        if ((u32)get_g_zyotai_low(9) < 2) {
            serifu_run();
        } else {
            sub_sub_step_up();
        }
        break;
    case 29:
        fade_out_step();
        break;
    case 30:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 31:
        serifu_run();
        break;
    case 32:
        bunkaken_flags |= 1;
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
