/* @CompileEnv: IDO52_FORCE_WEB_COLOR=2:v1 */
/* @SymbolStart: 0x801527A8 */
/* Address: 0x801527B0 */
/* Size: 1880 bytes (470 insns) */
/* @SymbolSize: 1888 */
/* @RodataBase: 0x8015F460 */
/* @PostProc: swap_instructions --offset1 0x428 --offset2 0x42c */
/* @PostProc: swap_instructions --offset1 0x430 --offset2 0x434 */
/* @PostProc: patch_insn_reg_field 0x418:rt:t2:t0 0x41c:rs:t2:t0 0x41c:rt:t2:t0 0x424:rt:t3:t1 0x428:rt:t2:t0 0x42c:rs:t3:t1 0x42c:rt:t3:t1 0x430:rt:t5:v0 0x438:rs:t5:v0 0x43c:rt:t3:t1 0x444:rt:t4:t2 0x448:rt:t4:t2 0x44c:rt:t6:t8 0x450:rs:t6:t8 0x450:rt:t8:t9 0x458:rt:t8:t9 0x46c:rt:t7:t3 0x474:rt:t9:t4 0x478:rs:t9:t4 0x478:rt:t9:t4 0x47c:rt:t7:t3 0x484:rt:t9:t4 0x48c:rt:t0:t5 0x490:rt:t0:t5 0x4ac:rt:t1:t6 0x4b0:rt:t2:t8 0x4b4:rt:t2:t8 0x4b8:rt:t1:t6 0x4e4:rt:t3:t7 0x4f0:rt:t3:t7 0x570:rt:t4:t9 0x574:rs:t4:t9 0x61c:rt:t5:t0 0x620:rs:t5:t0 0x638:rd:t6:t1 0x640:rt:t6:t1 0x644:rt:t6:t1 0x64c:rs:t6:t1 0x654:rt:t8:t2 0x660:rt:t8:t2 0x664:rt:t7:t3 0x670:rt:t7:t3 0x674:rt:t9:t4 0x680:rt:t9:t4 0x684:rt:t0:t5 0x68c:rt:t0:t5 0x690:rt:t1:t6 0x694:rs:t1:t6 0x694:rt:t1:t6 0x69c:rs:t1:t6 0x6b8:rt:t2:t8 0x6bc:rs:t2:t8 0x6bc:rt:t2:t8 0x6c4:rs:t2:t8 0x6c4:rt:t3:t7 0x6c8:rt:t3:t7 0x720:rt:t4:t9 0x72c:rt:t4:t9 */
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
extern void set_kanji_string(s32, s32, s32, s32, s32);
extern void k_disp_start(s32);
extern void k_reset(s32);
extern s32 check_end_k(void);
extern void set_dec_bri(s32);
extern void dec_bg_reset(void);
extern void indexed_byte_lsb_set(s32, s32);
extern void value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);
extern void hizuke_init(void);
extern void message_window_init(void);
extern void bustup_ptr_init(void);
extern void fade_down_main(void);
extern void fade_in_step(void);
extern void fade_in(void);
extern void fade_out_step(void);
extern void fade_out(void);
extern void menu2_main(void);
extern void zyotai_sprite_set_chk(void);
extern void date_name_set(s32);
extern void yazirushi_off(void);
extern void serifu_run(void);
extern void date_x_taku_set(s32, s32);
extern s32 get_g_zyotai_low(s32);
extern void bustup_load_step(s32);
extern s32 strlen(s32);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);
extern void bunkasai_girl0_home_srf_bank_set(void);

extern s16 now_serifu;
extern u8  ret_step;
extern u8  ret_sub_step;
extern u8  ret_sub_sub_step;
extern u8  now_girl;
extern s32 pad_trig;
extern u8  now_step;
extern u8  now_sub_step;
extern u8  step_mode;
extern u8  tt_result;
extern u8  bunkaken_flags;
extern u8  yazirushi_cw_disp;
extern s32 serifu_no;
extern s32 common_disp_off;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;
extern u8  bunkasai_main8_str0[];
extern u8  bunkasai_yazirushi_cnt;
extern u8  p_sei[];
extern u8  p_sei_b[];

void bunkasai_girl0_home_club3_96_scene(void)
{
    s32 v0;
    s32 n;
    s16 len;

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
        bustup_load_step(21578);
        break;
    case 4:
        bustup_ptr_init();
        now_girl = 0;
        zyotai_sprite_set_chk();
        bunkasai_basyo_init();
        cd_read_start2(24, 0x801E8000, 18896);
        break;
    case 5:
        fade_in();
        break;
    case 6:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(0);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 103;
                break;
            case 2:
                now_serifu = 108;
                break;
            case 3:
                now_serifu = 113;
                break;
            case 4:
                now_serifu = 118;
                break;
            }
            bunkasai_girl0_home_srf_bank_set();
        }
        break;
    case 7:
        serifu_run();
        break;
    case 8:
        v0 = get_g_zyotai_low(0);
        if (v0 == 4) {
            fade_down_main();
        } else {
            goto_sub_sub_step(11);
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        goto_sub_sub_step(16);
        break;
    case 11:
        menu2_main();
        break;
    case 12:
        if (serifu_no != 0) {
            serifu_run();
        } else {
            goto_sub_sub_step(16);
        }
        break;
    case 13:
        menu2_main();
        break;
    case 14:
        if (serifu_no != 0) {
            serifu_run();
        } else {
            goto_sub_sub_step(16);
        }
        break;
    case 15:
        goto_sub_sub_step(27);
        break;
    case 16:
        v0 = get_g_zyotai_low(0);
        if (v0 == 4) {
            sub_sub_step_up();
            break;
        }

        switch (step_mode) {
        case 0:
            len = strlen((s32)p_sei);
            k_reset(1);
            set_kanji_string((s16)(-104 - len * 4), 50, 0,
                          (s32)p_sei_b, 0);
            set_kanji_string(-86, 50, 0, (s32)bunkasai_main8_str0, 0);
            k_disp_start(2);
            step_mode = step_mode + 1;
            bunkasai_yazirushi_cnt = 0;
            break;
        case 1:
            n = (u8)(bunkasai_yazirushi_cnt + 1);
            bunkasai_yazirushi_cnt = n;
            if ((u32)n >= 16) {
                if (check_end_k() != 0) {
                    yazirushi_cw_disp ^= 0x80;
                    bunkasai_yazirushi_cnt = 0;
                }
            }
            if ((pad_trig & 0xF0) != 0) {
                if (check_end_k() != 0) {
                    k_reset(1);
                    yazirushi_cw_disp &= 0x7F;
                    step_mode = step_mode + 1;
                }
            }
            break;
        case 2:
            fade_down_main();
            break;
        }
        break;
    case 17:
        ret_step = now_step;
        ret_sub_step = now_sub_step;
        ret_sub_sub_step = now_sub_sub_step + 1;
        bunkaken_flags = (bunkaken_flags | 3) & 0xFFE3;
        goto_sub_step(164);
        break;
    case 18:
        common_disp_off = 1;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_sub_step(163);
        break;
    case 19:
        device_cmd_send(191);
        device_load_start(1, 0xC486, 0xC459, 0xD266, 0xD216, 0xD20E);
        date_x_taku_set(1, 0);
        bunkasai_basyo_init();
        mes_base_y = 192;
        message_window_init();
        hizuke_init();
        yazirushi_off();
        date_name_set(0);
        common_disp_off = 0;
        sub_sub_step_up();
        break;
    case 20:
        if (device_load_poll() == 1) {
            device_cmd_send(512);
            dec_bg_reset();
            set_dec_bri(0);
            value_cache_find_or_load(16645, 1);
            sub_sub_step_up();
        }
        break;
    case 21:
        if (get_CD_State2() & 1) {
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            sub_sub_step_up();
        }
        break;
    case 22:
        bustup_load_step(21578);
        break;
    case 23:
        bustup_ptr_init();
        now_girl = 0;
        zyotai_sprite_set_chk();
        cd_read_start2(24, 0x801E8000, 18896);
        break;
    case 24:
        v0 = get_g_zyotai_low(0);
        if (v0 == 4) {
            fade_in_step();
            break;
        }

        fade_in();
        break;
    case 25:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(0);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 106;
                break;
            case 2:
                now_serifu = 111;
                break;
            case 3:
                now_serifu = 116;
                break;
            case 4:
                now_serifu = 178;
                break;
            }
            if ((u32)tt_result < 2 && (u32)get_g_zyotai_low(0) < 4) {
                now_serifu = now_serifu + 1;
            }
            bunkasai_girl0_home_srf_bank_set();
        }
        break;
    case 26:
        serifu_run();
        break;
    case 27:
        v0 = get_g_zyotai_low(0);
        if (v0 == 4) {
            fade_out_step();
        } else {
            fade_out();
        }
        break;
    case 28:
        now_serifu = 1;
        sub_sub_step_up();
        break;
    case 29:
        serifu_run();
        break;
    case 30:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
