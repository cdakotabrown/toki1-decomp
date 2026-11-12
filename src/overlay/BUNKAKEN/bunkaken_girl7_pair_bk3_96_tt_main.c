/* @SymbolStart: 0x8014FA18 */
/* @RodataBase: 0x80155978 */
/* @SymbolSize: 1308 */
/* @PostProc: swap_instructions --offset1 0x180 --offset2 0x184 */
/* @PostProc: swap_instructions --offset1 0x188 --offset2 0x18c */
/* @PostProc: patch_insn_reg_field 0x170:rt:t2:t7 0x174:rs:t2:t7 0x174:rt:t2:t7 0x17c:rt:t3:t8 0x180:rt:t2:t7 0x184:rs:t3:t8 0x184:rt:t3:t8 0x188:rt:t5:v1 0x190:rs:t5:v1 0x190:rt:t5:t2 0x194:rt:t3:t8 0x19c:rt:t4:t9 0x1a0:rt:t4:t9 0x1a4:rs:t5:t2 0x1a4:rt:t6:t4 0x1a8:rs:t6:t4 0x1a8:rt:t7:t6 0x1b0:rt:t7:t6 0x1c4:rt:t8:t0 0x1cc:rt:t9:t1 0x1d0:rs:t9:t1 0x1d0:rt:t9:t1 0x1d4:rt:t8:t0 0x1dc:rt:t9:t1 0x1e4:rt:t0:t2 0x1e8:rt:t0:t2 0x204:rt:t1:t3 0x208:rt:t2:t4 0x20c:rt:t2:t4 0x210:rt:t1:t3 0x23c:rt:t3:t5 0x248:rt:t3:t5 0x2c8:rt:t4:t6 0x2cc:rs:t4:t6 0x310:rt:t5:t7 0x31c:rt:t5:t7 0x354:rt:t6:t8 0x358:rs:t6:t8 0x380:rt:t7:t9 0x384:rs:t7:t9 0x384:rt:t7:t9 0x388:rt:t8:t0 0x38c:rs:t7:t9 0x394:rt:t9:t1 0x3a0:rt:t8:t0 0x3a8:rt:t9:t1 0x3f8:rt:t1:t3 0x404:rt:t2:t4 0x410:rt:t0:t2 0x41c:rt:t0:t2 0x428:rt:t1:t3 0x430:rt:t2:t4 0x464:rt:t3:t5 0x480:rt:t3:t5 0x4f4:rt:t4:v1 0x4f8:rs:t4:v1 0x4f8:rt:t4:t6 0x500:rs:t4:t6 0x500:rt:t5:t8 0x508:rt:t5:t8 */
/* Address: 0x8014FA20 */
/* Size: 1308 bytes (327 insns; last 2 are next func's cross-prologue) */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern s32 goto_sub_step(s32);
extern s32 goto_sub_sub_step(s32);
extern s32 device_cmd_send(s32);
extern s32 device_load_start(s32, s32, s32, s32, s32, s32);
extern s32 device_load_poll(void);
extern s32 get_CD_State2(void);
extern s32 cd_read_start2(s32, s32, s32);
extern s32 game_reset(void);
extern s32 dec_bg_reset(void);
extern s32 set_dec_bri(s32);
extern s32 indexed_byte_lsb_set(s32, s32);
extern s32 value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);
extern s32 hizuke_init(void);
extern s32 message_window_init(void);
extern s32 zyotai_sprite_set_chk(void);
extern s32 date_name_set(s32);
extern s32 yazirushi_off(void);
extern s32 serifu_run(void);
extern s32 date_x_taku_set(s32, s32);
extern s32 get_g_zyotai_low(s32);
extern s32 bustup_load_step(s32);
extern s32 bustup_ptr_init(void);
extern s32 fade_down_main(void);
extern s32 fade_in(void);
extern s32 fade_out_step(void);
extern s32 fade_out(void);
extern s32 bunkaken_basyo_init3(void);
extern s32 bunkaken_sub_reset(void);
extern void bunkaken_girl7_pair_bank_init(void);

extern s16 now_serifu;
extern u8  ret_step;
extern u8  ret_sub_step;
extern u8  ret_sub_sub_step;
extern u8  now_step;
extern u8  now_sub_step;
extern u8  now_girl;
extern u8  tt_result;
extern u8  bunkaken_flags;
extern s32 common_disp_off;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;
extern u8 now_sub_sub_step;

void bunkaken_girl7_pair_bk3_96_tt_main(void) {
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
        if (get_CD_State2() & 1) {
            bunkaken_basyo_init3();
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            cd_read_start2(19, 0x801E8000, 19026);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(7);
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
            bunkaken_girl7_pair_bank_init();
        }
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        fade_down_main();
        break;
    case 7:
        ret_step = now_step;
        ret_sub_step = now_sub_step;
        ret_sub_sub_step = now_sub_sub_step + 1;
        bunkaken_flags = (bunkaken_flags | 2) & 0xFFE3;
        goto_sub_step(141);
        break;
    case 8:
        common_disp_off = 1;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_sub_step(140);
        break;
    case 9:
        device_cmd_send(191);
        device_load_start(1, 0xC486, 0xC459, 0xD266, 0xD216, 0xD20E);
        date_x_taku_set(1, 0);
        bunkaken_basyo_init3();
        mes_base_y = 192;
        message_window_init();
        hizuke_init();
        yazirushi_off();
        date_name_set(7);
        common_disp_off = 0;
        sub_sub_step_up();
        break;
    case 10:
        if (device_load_poll() == 1) {
            device_cmd_send(512);
            dec_bg_reset();
            set_dec_bri(0);
            value_cache_find_or_load(16645, 1);
            sub_sub_step_up();
        }
        break;
    case 11:
        if (get_CD_State2() & 1) {
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            sub_sub_step_up();
        }
        break;
    case 12:
        bustup_load_step(23333);
        break;
    case 13:
        bustup_ptr_init();
        now_girl = 7;
        zyotai_sprite_set_chk();
        cd_read_start2(19, 0x801E8000, 19026);
        break;
    case 14:
        fade_in();
        break;
    case 15:
        if (get_CD_State2() & 1) {
            bunkaken_girl7_pair_bank_init();
        }
        break;
    case 16:
        v0 = get_g_zyotai_low(7);
        if ((u32)v0 < 2) {
            if ((u32)tt_result < 2) {
                now_serifu = 67;
            } else {
                now_serifu = 66;
            }
            sub_sub_step_up();
        } else {
            goto_sub_sub_step(18);
        }
        break;
    case 17:
        serifu_run();
        break;
    case 18:
        v0 = get_g_zyotai_low(7);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 68;
            break;
        case 2:
            now_serifu = 69;
            break;
        case 3:
            now_serifu = 70;
            break;
        }
        sub_sub_step_up();
        break;
    case 19:
        serifu_run();
        break;
    case 20:
        v0 = get_g_zyotai_low(7);
        if ((u32)v0 < 2) {
            goto_sub_sub_step(22);
        } else {
            now_serifu = 21;
            fade_down_main();
        }
        break;
    case 21:
        serifu_run();
        break;
    case 22:
        v0 = get_g_zyotai_low(7);
        if ((u32)v0 < 2) {
            fade_out();
        } else {
            fade_out_step();
        }
        break;
    case 23:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 24:
        serifu_run();
        break;
    case 25:
        bunkaken_flags = bunkaken_flags | 1;
        bunkaken_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
