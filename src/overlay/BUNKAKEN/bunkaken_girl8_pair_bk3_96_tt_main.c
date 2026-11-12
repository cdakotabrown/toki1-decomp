/* @SymbolStart: 0x8013FD48 */
/* @SymbolSize: 1904 */
/* @RodataBase: 0x80154408 */
/* @PostProc: swap_instructions --offset1 0x194 --offset2 0x198 */
/* @PostProc: swap_instructions --offset1 0x190 --offset2 0x194 */
/* @PostProc: swap_instructions --offset1 0x1b4 --offset2 0x1b8 */
/* @PostProc: swap_instructions --offset1 0x40c --offset2 0x410 */
/* @PostProc: patch_insn_reg_field 0x180:rt:t2:t8 0x184:rs:t2:t8 0x184:rt:t2:t8 0x18c:rt:t3:t9 0x190:rt:t2:t8 0x194:rs:t3:t9 0x194:rt:t3:t9 0x198:rt:t5:v1 0x19c:rs:t5:v1 0x19c:rt:t5:t2 0x1a4:rt:t3:t9 0x1ac:rt:t4:t0 0x1b0:rs:t5:t2 0x1b0:rt:t6:t4 0x1b4:rs:t6:t4 0x1b4:rt:t7:t5 0x1b8:rt:t4:t0 0x1bc:rs:t7:t5 0x1bc:rt:t8:t7 0x1c4:rt:t8:t7 0x1d8:rt:t9:t1 0x1e0:rt:t0:t2 0x1e4:rs:t0:t2 0x1e4:rt:t0:t2 0x1e8:rt:t9:t1 0x1f0:rt:t0:t2 0x1f8:rt:t1:t3 0x1fc:rt:t1:t3 0x218:rt:t2:t4 0x21c:rt:t3:t5 0x220:rt:t3:t5 0x224:rt:t2:t4 0x250:rt:t4:t6 0x25c:rt:t4:t6 0x2dc:rt:t5:t7 0x2e0:rs:t5:t7 0x324:rt:t6:t8 0x330:rt:t6:t8 0x368:rt:t7:t9 0x36c:rs:t7:t9 0x390:rt:t9:t1 0x39c:rt:t0:t2 0x3a8:rt:t8:t0 0x3b4:rt:t8:t0 0x3c0:rt:t9:t1 0x3c8:rt:t0:t2 0x3fc:rt:t1:t8 0x400:rs:t1:t8 0x400:rt:t1:t8 0x408:rt:t2:t9 0x40c:rt:t1:t8 0x410:rs:t2:t9 0x410:rt:t2:t9 0x414:rt:at:v1 0x418:rt:t4:at 0x41c:rs:t4:v1 0x41c:rt:t4:t3 0x420:rt:t2:t9 0x428:rt:t3:t0 0x42c:rt:t3:t0 0x430:rs:t4:t3 0x434:rt:t6:t7 0x43c:rt:t6:t7 0x450:rt:t7:t1 0x458:rt:t8:t2 0x45c:rs:t8:t2 0x45c:rt:t8:t2 0x460:rt:t7:t1 0x468:rt:t8:t2 0x470:rt:t9:t3 0x474:rt:t9:t3 0x490:rt:t0:t4 0x494:rt:t1:t5 0x498:rt:t1:t5 0x49c:rt:t0:t4 0x4c8:rt:t2:t6 0x4d4:rt:t2:t6 0x554:rt:t3:t7 0x558:rs:t3:t7 0x59c:rt:t4:t8 0x5a8:rt:t4:t8 0x5e0:rt:t5:t9 0x5e4:rs:t5:t9 0x5f8:rt:t9:t3 0x60c:rt:t7:t1 0x618:rt:t8:t2 0x624:rt:t6:t0 0x630:rt:t6:t0 0x63c:rt:t7:t1 0x644:rt:t8:t2 0x648:rs:t9:t3 0x648:rt:t9:t3 0x64c:rt:t0:t4 0x650:rs:t9:t3 0x65c:rs:t0:t4 0x65c:rt:t0:t4 0x664:rs:t0:t4 0x664:rt:t1:t5 0x668:rt:t1:t5 0x6a8:rt:t3:t7 0x6b4:rt:t4:t8 0x6c0:rt:t2:t6 0x6cc:rt:t2:t6 0x6d8:rt:t3:t7 0x6e0:rt:t4:t8 0x748:rt:t5:v1 0x74c:rs:t5:v1 0x74c:rt:t5:t9 0x754:rs:t5:t9 0x754:rt:t6:t1 0x75c:rt:t6:t1 */
/* Address: 0x8013FD50 (window starts at the absorbed cross-prologue head */
/* Size: 1904 bytes (476 insns incl. head) */
#include "types.h"

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
extern s32 fade_out_step(void);
extern s32 fade_out(void);
extern s32 zyotai_sprite_set_chk(void);
extern s32 date_name_set(s32);
extern s32 yazirushi_off(void);
extern s32 serifu_run(void);
extern s32 date_x_taku_set(s32, s32);
extern s32 get_g_zyotai_low(s32);
extern s32 bustup_load_step(s32);
extern void bunkaken_girl8_pair_bank_init(void);
extern s32 bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);

extern s16 now_serifu;
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
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

void bunkaken_girl8_pair_bk3_96_tt_main(void) {
    s32 v0;

    switch ((s32)now_sub_sub_step) {
    case 0:
        value_cache_find_or_load(16645, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out();
        break;
    case 2:
        if (get_CD_State2() & 1) {
            bunkaken_basyo_init3();
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            cd_read_start2(16, 0x801E8000, 18855);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(8);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 60;
                break;
            case 2:
                now_serifu = 62;
                break;
            case 3:
                now_serifu = 64;
                break;
            }
            bunkaken_girl8_pair_bank_init();
        }
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        serifu_run();
        break;
    case 7:
        fade_out();
        break;
    case 8:
        ret_step = now_step;
        ret_sub_step = now_sub_step;
        ret_sub_sub_step = now_sub_sub_step + 1;
        bunkaken_flags = ((bunkaken_flags | 2) & 0xFFE3) | 0x10;
        goto_step(208);
        break;
    case 9:
        common_disp_off = 1;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_sub_step(140);
        break;
    case 10:
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
    case 11:
        if (device_load_poll() == 1) {
            device_cmd_send(512);
            dec_bg_reset();
            set_dec_bri(0);
            value_cache_find_or_load(16645, 1);
            sub_sub_step_up();
        }
        break;
    case 12:
        if (get_CD_State2() & 1) {
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            sub_sub_step_up();
        }
        break;
    case 13:
        bustup_load_step(20498);
        break;
    case 14:
        bustup_ptr_init();
        now_girl = 8;
        zyotai_sprite_set_chk();
        cd_read_start2(16, 0x801E8000, 18855);
        break;
    case 15:
        fade_in();
        break;
    case 16:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(8);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 66;
                break;
            case 2:
                now_serifu = 67;
                break;
            case 3:
                now_serifu = 68;
                break;
            }
            bunkaken_girl8_pair_bank_init();
        }
        break;
    case 17:
        serifu_run();
        break;
    case 18:
        fade_down_main();
        break;
    case 19:
        ret_step = now_step;
        ret_sub_step = now_sub_step;
        ret_sub_sub_step = now_sub_sub_step + 1;
        bunkaken_flags = (bunkaken_flags | 2) & 0xFFE3;
        goto_sub_step(141);
        break;
    case 20:
        common_disp_off = 1;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_sub_step(140);
        break;
    case 21:
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
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            sub_sub_step_up();
        }
        break;
    case 24:
        bustup_load_step(20498);
        break;
    case 25:
        bustup_ptr_init();
        now_girl = 8;
        zyotai_sprite_set_chk();
        cd_read_start2(16, 0x801E8000, 18855);
        break;
    case 26:
        fade_in();
        break;
    case 27:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(8);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 69;
                break;
            case 2:
                now_serifu = 71;
                break;
            case 3:
                now_serifu = 73;
                break;
            }
            if ((u32)tt_result < 4) {
                now_serifu = now_serifu + 1;
            }
            bunkaken_girl8_pair_bank_init();
        }
        break;
    case 28:
        serifu_run();
        break;
    case 29:
        v0 = get_g_zyotai_low(8);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 75;
            break;
        case 2:
            now_serifu = 76;
            break;
        case 3:
            now_serifu = 77;
            break;
        }
        sub_sub_step_up();
        break;
    case 30:
        serifu_run();
        break;
    case 31:
        fade_down_main();
        break;
    case 32:
        fade_out_step();
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
