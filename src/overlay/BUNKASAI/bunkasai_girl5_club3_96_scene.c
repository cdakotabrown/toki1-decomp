/* @RodataBase: 0x8015E8F0 */
/* @SymbolStart: 0x8014C528 */
/* @SymbolSize: 2412 */
/* @PostProc: swap_instructions --offset1 0x300 --offset2 0x304 */
/* @PostProc: swap_instructions --offset1 0x2fc --offset2 0x300 */
/* @PostProc: swap_instructions --offset1 0x320 --offset2 0x324 */
/* @PostProc: swap_instructions --offset1 0x59c --offset2 0x5a0 */
/* @PostProc: swap_instructions --offset1 0x5a4 --offset2 0x5a8 */
/* @PostProc: patch_insn_reg_field 0x2ec:rt:t2:t8 0x2f0:rs:t2:t8 0x2f0:rt:t2:t8 0x2f8:rt:t3:t9 0x2fc:rt:t2:t8 0x300:rs:t3:t9 0x300:rt:t3:t9 0x304:rt:t5:v1 0x308:rs:t5:v1 0x308:rt:t5:t2 0x310:rt:t3:t9 0x318:rt:t4:t0 0x31c:rs:t5:t2 0x31c:rt:t6:t4 0x320:rs:t6:t4 0x320:rt:t7:t5 0x324:rt:t4:t0 0x328:rs:t7:t5 0x328:rt:t8:t7 0x330:rt:t8:t7 0x344:rt:t9:t1 0x34c:rt:t0:t2 0x350:rs:t0:t2 0x350:rt:t0:t2 0x354:rt:t9:t1 0x35c:rt:t0:t2 0x364:rt:t1:t3 0x368:rt:t1:t3 0x384:rt:t2:t4 0x388:rt:t3:t5 0x38c:rt:t3:t5 0x390:rt:t2:t4 0x3bc:rt:t4:t6 0x3c8:rt:t4:t6 0x448:rt:t5:t7 0x44c:rs:t5:t7 0x490:rt:t6:t8 0x49c:rt:t6:t8 0x4d4:rt:t7:t9 0x4d8:rs:t7:t9 0x4fc:rt:t9:t1 0x508:rt:t0:t2 0x514:rt:t8:t0 0x520:rt:t8:t0 0x52c:rt:t9:t1 0x534:rt:t0:t2 0x58c:rt:t1:t8 0x590:rs:t1:t8 0x590:rt:t1:t8 0x598:rt:t2:t9 0x59c:rt:t1:t8 0x5a0:rs:t2:t9 0x5a0:rt:t2:t9 0x5a4:rt:t4:v1 0x5ac:rs:t4:v1 0x5ac:rt:t4:t3 0x5b0:rt:t2:t9 0x5b8:rt:t3:t0 0x5bc:rt:t3:t0 0x5c0:rs:t4:t3 0x5c4:rt:t6:t7 0x5cc:rt:t6:t7 0x5e0:rt:t7:t1 0x5e8:rt:t8:t2 0x5ec:rs:t8:t2 0x5ec:rt:t8:t2 0x5f0:rt:t7:t1 0x5f8:rt:t8:t2 0x600:rt:t9:t3 0x604:rt:t9:t3 0x620:rt:t0:t4 0x624:rt:t1:t5 0x628:rt:t1:t5 0x62c:rt:t0:t4 0x658:rt:t2:t6 0x664:rt:t2:t6 0x6e4:rt:t3:t7 0x6e8:rs:t3:t7 0x72c:rt:t4:t8 0x738:rt:t4:t8 0x794:rt:t5:t9 0x798:rs:t5:t9 0x7b0:rd:t6:t0 0x7b8:rt:t6:t0 0x7bc:rt:t6:t0 0x7c4:rs:t6:t0 0x7cc:rt:t7:t1 0x7d8:rt:t7:t1 0x7dc:rt:t8:t2 0x7e8:rt:t8:t2 0x7ec:rt:t9:t3 0x7f8:rt:t9:t3 0x7fc:rt:t0:t4 0x804:rt:t0:t4 0x840:rd:t1:t5 0x848:rt:t1:t5 0x84c:rt:t1:t5 0x854:rs:t1:t5 0x85c:rt:t2:t6 0x868:rt:t2:t6 0x86c:rt:t3:t7 0x878:rt:t3:t7 0x87c:rt:t4:t8 0x888:rt:t4:t8 0x8ac:rt:t5:t9 0x8b8:rt:t5:t9 0x944:rt:t6:v1 0x948:rs:t6:v1 0x948:rt:t6:t0 0x950:rs:t6:t0 0x950:rt:t7:t2 0x958:rt:t7:t2 */
/* Address: 0x8014C528 (absorbed head) / 0x8014C530 (prologue) */
/* Size: 2412 bytes (603 insns) incl. the absorbed 2-word cross-prologue head. */
#include "types.h"

extern void sub_sub_step_up(void);
extern void goto_step(s32);
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
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);
extern void bunkasai_girl5_srf_bank_set(void);

extern s16 now_serifu;
extern u8  ret_step;
extern u8  ret_sub_step;
extern u8  ret_sub_sub_step;
extern u8  now_girl;
extern u8  now_step;
extern u8  now_sub_step;
extern u8  now_sub_sub_step;
extern u8  bunkaken_flags;
extern s32 common_disp_off;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

void bunkasai_girl5_club3_96_scene(void) {
    switch ((u32)now_sub_sub_step) {
    case 0:
        value_cache_find_or_load(16645, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out_step();
        break;
    case 2:
        if (get_CD_State2() & 1) {
            indexed_byte_lsb_set(1, dec_bg_init(1));
            sub_sub_step_up();
        }
        break;
    case 3:
        bustup_load_step(24503);
        break;
    case 4:
        bustup_ptr_init();
        now_girl = 5;
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
        cd_read_start2(13, 0x801E8000, 19045);
        sub_sub_step_up();
        break;
    case 8:
        fade_up_main();
        break;
    case 9:
        if (get_CD_State2() & 1) {
            switch ((u32)get_g_zyotai_low(5)) {
            case 0:
            case 1:
                now_serifu = 5;
                break;
            case 2:
                now_serifu = 2;
                break;
            case 3:
                now_serifu = 3;
                break;
            case 4:
                now_serifu = 7;
                break;
            }
            bunkasai_girl5_srf_bank_set();
        }
        break;
    case 10:
        serifu_run();
        break;
    case 11:
        now_serifu = 70;
        sub_sub_step_up();
        break;
    case 12:
        serifu_run();
        break;
    case 13:
        switch ((u32)get_g_zyotai_low(5)) {
        case 0:
        case 1:
            now_serifu = 71;
            break;
        case 2:
            now_serifu = 72;
            break;
        case 3:
            now_serifu = 73;
            break;
        case 4:
            now_serifu = 74;
            break;
        }
        sub_sub_step_up();
        break;
    case 14:
        serifu_run();
        break;
    case 15:
        if (get_g_zyotai_low(5) == 4) {
            fade_down_main();
        } else {
            goto_sub_sub_step(18);
        }
        break;
    case 16:
        serifu_run();
        break;
    case 17:
        goto_sub_sub_step(29);
        break;
    case 18:
        fade_out();
        break;
    case 19:
        ret_step = now_step;
        ret_sub_step = now_sub_step;
        ret_sub_sub_step = now_sub_sub_step + 1;
        bunkaken_flags = ((bunkaken_flags | 2) & 0xFFE3) | 4;
        goto_step(208);
        break;
    case 20:
        common_disp_off = 1;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_sub_step(163);
        break;
    case 21:
        device_cmd_send(191);
        device_load_start(1, 0xC486, 0xC459, 0xD266, 0xD216, 0xD20E);
        date_x_taku_set(1, 0);
        bunkasai_basyo_init();
        mes_base_y = 192;
        message_window_init();
        hizuke_init();
        yazirushi_off();
        date_name_set(5);
        common_disp_off = 0;
        sub_sub_step_up();
        break;
    case 22:
        if (device_load_poll() == 1) {
            device_cmd_send(512);
            dec_bg_reset();
            set_dec_bri(0);
            value_cache_find_or_load(16645, 1);
            sub_sub_step_up();
        }
        break;
    case 23:
        if (get_CD_State2() & 1) {
            indexed_byte_lsb_set(1, dec_bg_init(1));
            sub_sub_step_up();
        }
        break;
    case 24:
        bustup_load_step(24503);
        break;
    case 25:
        bustup_ptr_init();
        now_girl = 5;
        zyotai_sprite_set_chk();
        cd_read_start2(13, 0x801E8000, 19045);
        break;
    case 26:
        fade_in();
        break;
    case 27:
        if (get_CD_State2() & 1) {
            switch ((u32)get_g_zyotai_low(5)) {
            case 0:
            case 1:
                now_serifu = 79;
                break;
            case 2:
                now_serifu = 80;
                break;
            case 3:
                now_serifu = 81;
                break;
            }
            bunkasai_girl5_srf_bank_set();
        }
        break;
    case 28:
        serifu_run();
        break;
    case 29:
        if (get_g_zyotai_low(5) == 4) {
            sub_sub_step_up();
        } else {
            fade_down_main();
        }
        break;
    case 30:
        ret_step = now_step;
        ret_sub_step = now_sub_step;
        ret_sub_sub_step = now_sub_sub_step + 1;
        bunkaken_flags = (bunkaken_flags | 2) & 0xFFE3;
        goto_sub_step(164);
        break;
    case 31:
        common_disp_off = 1;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_sub_step(163);
        break;
    case 32:
        device_cmd_send(191);
        device_load_start(1, 0xC486, 0xC459, 0xD266, 0xD216, 0xD20E);
        date_x_taku_set(1, 0);
        bunkasai_basyo_init();
        mes_base_y = 192;
        message_window_init();
        hizuke_init();
        yazirushi_off();
        date_name_set(5);
        common_disp_off = 0;
        sub_sub_step_up();
        break;
    case 33:
        if (device_load_poll() == 1) {
            device_cmd_send(512);
            dec_bg_reset();
            set_dec_bri(0);
            value_cache_find_or_load(16645, 1);
            sub_sub_step_up();
        }
        break;
    case 34:
        if (get_CD_State2() & 1) {
            indexed_byte_lsb_set(1, dec_bg_init(1));
            sub_sub_step_up();
        }
        break;
    case 35:
        bustup_load_step(24503);
        break;
    case 36:
        bustup_ptr_init();
        now_girl = 5;
        zyotai_sprite_set_chk();
        cd_read_start2(13, 0x801E8000, 19045);
        break;
    case 37:
        if (get_g_zyotai_low(5) == 4) {
            fade_in_step();
        } else {
            fade_in();
        }
        break;
    case 38:
        if (get_CD_State2() & 1) {
            switch ((u32)get_g_zyotai_low(5)) {
            case 0:
            case 1:
                now_serifu = 82;
                break;
            case 2:
                now_serifu = 83;
                break;
            case 3:
                now_serifu = 84;
                break;
            case 4:
                now_serifu = 85;
                break;
            }
            bunkasai_girl5_srf_bank_set();
        }
        break;
    case 39:
        serifu_run();
        break;
    case 40:
        sub_sub_step_up();
        switch ((u32)get_g_zyotai_low(5)) {
        case 0:
        case 1:
            now_serifu = 8;
            break;
        case 2:
            now_serifu = 9;
            break;
        case 3:
            now_serifu = 10;
            break;
        case 4:
            goto_sub_sub_step(43);
            break;
        }
        break;
    case 41:
        serifu_run();
        break;
    case 42:
        now_serifu = 111;
        fade_down_main();
        break;
    case 43:
        if (get_g_zyotai_low(5) == 4) {
            fade_out_step();
        } else {
            serifu_run();
        }
        break;
    case 44:
        now_serifu = 0;
        if (get_g_zyotai_low(5) == 4) {
            sub_sub_step_up();
        } else {
            fade_out_step();
        }
        break;
    case 45:
        serifu_run();
        break;
    case 46:
        bunkaken_flags |= 1;
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
