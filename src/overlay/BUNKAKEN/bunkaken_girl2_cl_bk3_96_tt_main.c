/* @SymbolStart: 0x80141B90 */
/* Address: 0x80141B98 */
/* Size: 1852 bytes (463 insns) + 2-insn cross-prologue tail */
/* @SymbolSize: 1860 */
/* @RodataBase: 0x801546C0 */
/* @PostProc: swap_instructions --offset1 0x368 --offset2 0x36c */
/* @PostProc: swap_instructions --offset1 0x370 --offset2 0x374 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x14:rs:v0:v1 0x14:rt:v0:v1 0x1c:rs:v0:v1 0x28:rt:v0:v1 0x214:rt:t9:t0 0x218:rd:t8:t9 0x21c:rs:t8:t9 0x234:rs:t9:t0 0x234:rt:t9:t1 0x238:rt:t1:t3 0x23c:rs:t9:t1 0x23c:rt:t0:t2 0x240:rt:t0:t2 0x24c:rt:t1:t3 0x284:rt:t3:t5 0x290:rt:t4:t6 0x29c:rt:t2:t4 0x2a8:rt:t2:t4 0x2b4:rt:t3:t5 0x2bc:rt:t4:t6 0x2d0:rt:t5:t7 0x2d4:rs:t5:t7 0x2d4:rt:t5:t7 0x2dc:rs:t5:t7 0x324:rt:t6:t8 0x328:rs:t6:t8 0x328:rt:t6:t8 0x330:rs:t6:t8 0x358:rt:t7:t4 0x35c:rs:t7:t4 0x35c:rt:t7:t4 0x364:rt:t8:t5 0x368:rt:t7:t4 0x36c:rs:t8:t5 0x36c:rt:t8:t5 0x370:rt:t0:v0 0x378:rs:t0:v0 0x378:rt:t0:t9 0x37c:rt:t8:t5 0x384:rs:v0:v1 0x384:rt:t9:t6 0x388:rt:t9:t6 0x38c:rs:t0:t9 0x390:rt:t2:t3 0x398:rt:t2:t3 0x3ac:rt:t3:t7 0x3b4:rt:t4:t8 0x3b8:rs:t4:t8 0x3b8:rt:t4:t8 0x3bc:rt:t3:t7 0x3c4:rt:t4:t8 0x3cc:rs:v0:v1 0x3cc:rt:t5:t9 0x3d0:rt:t5:t9 0x3ec:rt:t6:t0 0x3f0:rt:t7:t1 0x3f4:rt:t7:t1 0x3f8:rt:t6:t0 0x424:rt:t8:t2 0x430:rt:t8:t2 0x4b0:rt:t9:t3 0x4b4:rs:t9:t3 0x4f8:rt:t0:t4 0x504:rt:t0:t4 0x534:rt:t1:t5 0x538:rs:t1:t5 0x538:rt:t1:t5 0x540:rs:t1:t5 0x570:rt:t2:t6 0x574:rs:t2:t6 0x590:rt:t6:t0 0x5a4:rt:t4:t8 0x5b0:rt:t5:t9 0x5bc:rt:t3:t7 0x5c8:rt:t3:t7 0x5d4:rt:t4:t8 0x5dc:rt:t5:t9 0x5e0:rs:t6:t0 0x5e0:rt:t6:t0 0x5e4:rt:t7:t1 0x5e8:rs:t6:t0 0x5f4:rs:t7:t1 0x5f4:rt:t7:t1 0x5fc:rs:t7:t1 0x5fc:rt:t8:t2 0x600:rt:t8:t2 0x614:rt:t9:t3 0x618:rs:t9:t3 0x618:rt:t9:t3 0x620:rs:t9:t3 0x624:rt:t0:t4 0x630:rt:t0:t4 0x668:rt:t2:t6 0x674:rt:t3:t7 0x680:rt:t1:t5 0x68c:rt:t1:t5 0x698:rt:t2:t6 0x6a0:rt:t3:t7 0x6b4:rt:t4:t8 0x6b8:rs:t4:t8 0x6b8:rt:t4:t8 0x6c0:rs:t4:t8 */
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
extern void bustup_cd_read(s32);
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
extern void bunkaken_girl2_cl_bank_init(void);
extern void bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);

extern s16 now_serifu;
extern union { s32 w; u8 b; } girl2_flag;
extern u8  ret_step;
extern u8  ret_sub_step;
extern u8  ret_sub_sub_step;
extern u8  now_girl;
extern u8  now_step;
extern u8  now_sub_step;
extern u8  tt_result;
extern u8  bunkaken_flags;
extern s32 common_disp_off;
extern s32 bunka_girl2;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;
extern u8 now_sub_sub_step;

void bunkaken_girl2_cl_bk3_96_tt_main(void) {
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
            bunkaken_basyo_init3();
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            bustup_cd_read(21038);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            bustup_ptr_init();
            now_girl = 2;
            zyotai_sprite_set_chk();
            cd_read_start2(9, 0x801E8000, 19000);
        }
        break;
    case 5:
        if (get_g_zyotai_low(2) == 4 || bunka_girl2 == 2) {
            sub_sub_step_up();
            break;
        }
        fade_up_main();
        break;
    case 6:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(2);
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
            }
            if (get_g_zyotai_low(2) == 4 || bunka_girl2 == 2) {
                now_serifu = 42;
            }
            bunkaken_girl2_cl_bank_init();
        }
        break;
    case 7:
        serifu_run();
        break;
    case 8:
        if ((girl2_flag.w << 29) < 0) {
            goto_sub_sub_step(10);
        } else {
            girl2_flag.b |= 4;
            now_serifu = 4;
            sub_sub_step_up();
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        v0 = get_g_zyotai_low(2);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 39;
            break;
        case 2:
            now_serifu = 40;
            break;
        case 3:
            now_serifu = 41;
            break;
        }
        if (get_g_zyotai_low(2) == 4 || bunka_girl2 == 2) {
            goto_sub_sub_step(12);
        } else {
            sub_sub_step_up();
        }
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        if (get_g_zyotai_low(2) == 4 || bunka_girl2 == 2) {
            sub_sub_step_up();
            break;
        }
        fade_down_main();
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
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            sub_sub_step_up();
        }
        break;
    case 18:
        bustup_load_step(21038);
        break;
    case 19:
        bustup_ptr_init();
        now_girl = 2;
        zyotai_sprite_set_chk();
        cd_read_start2(9, 0x801E8000, 19000);
        break;
    case 20:
        if (get_g_zyotai_low(2) == 4 || bunka_girl2 == 2) {
            fade_in_step();
            break;
        }
        fade_in();
        break;
    case 21:
        if (get_CD_State2() & 1) {
            bunkaken_girl2_cl_bank_init();
            v0 = get_g_zyotai_low(2);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 43;
                break;
            case 2:
                now_serifu = 45;
                break;
            case 3:
                now_serifu = 47;
                break;
            }
            if ((u32)tt_result < 2) {
                now_serifu = now_serifu + 1;
            }
            if (get_g_zyotai_low(2) == 4 || bunka_girl2 == 2) {
                now_serifu = 52;
            }
        }
        break;
    case 22:
        serifu_run();
        break;
    case 23:
        sub_sub_step_up();
        v0 = get_g_zyotai_low(2);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 49;
            break;
        case 2:
            now_serifu = 50;
            break;
        case 3:
            now_serifu = 51;
            break;
        }
        if (get_g_zyotai_low(2) == 4 || bunka_girl2 == 2) {
            goto_sub_sub_step(26);
        }
        break;
    case 24:
        serifu_run();
        break;
    case 25:
        fade_down_main();
        break;
    case 26:
        fade_out_step();
        break;
    case 27:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 28:
        serifu_run();
        break;
    case 29:
        bunkaken_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
