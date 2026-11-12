/* @SymbolStart: 0x8013BB94 */
/* @RodataBase: 0x8015CB68 */
/* Address: 0x8013BB9C */
/* Size: 1296 bytes (324 insns) */
/* @SymbolSize: 1304 */
/* @PostProc: swap_instructions --offset1 0x218 --offset2 0x21c */
/* @PostProc: swap_instructions --offset1 0x220 --offset2 0x224 */
/* @PostProc: patch_insn_reg_field 0x208:rt:t4:t9 0x20c:rs:t4:t9 0x20c:rt:t4:t9 0x214:rt:t5:t0 0x218:rt:t4:t9 0x21c:rs:t5:t0 0x21c:rt:t5:t0 0x220:rt:t7:v1 0x228:rs:t7:v1 0x228:rt:t7:t4 0x22c:rt:t5:t0 0x234:rt:t6:t1 0x238:rt:t6:t1 0x23c:rs:t7:t4 0x23c:rt:t8:t6 0x240:rs:t8:t6 0x240:rt:t9:t8 0x248:rt:t9:t8 0x25c:rt:t0:t2 0x264:rt:t1:t3 0x268:rs:t1:t3 0x268:rt:t1:t3 0x26c:rt:t0:t2 0x274:rt:t1:t3 0x27c:rt:t2:t4 0x280:rt:t2:t4 0x29c:rt:t3:t5 0x2a0:rt:t4:t6 0x2a4:rt:t4:t6 0x2a8:rt:t3:t5 0x2d4:rt:t5:t7 0x2e0:rt:t5:t7 0x360:rt:t6:t8 0x364:rs:t6:t8 0x3a8:rt:t7:t9 0x3b4:rt:t7:t9 0x410:rt:t8:t0 0x414:rs:t8:t0 0x42c:rd:t9:t1 0x434:rt:t9:t1 0x438:rt:t9:t1 0x440:rs:t9:t1 0x448:rt:t0:t2 0x454:rt:t0:t2 0x458:rt:t1:t3 0x464:rt:t1:t3 0x468:rt:t2:t4 0x470:rt:t2:t4 0x4c8:rt:t3:t5 0x4d4:rt:t3:t5 0x4f0:rt:t4:v1 0x4f4:rs:t4:v1 0x4f4:rt:t4:t6 0x4fc:rs:t4:t6 0x4fc:rt:t5:t8 0x504:rt:t5:t8 */
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
extern s32 get_g_zyotai_low(s32);
extern void bustup_load_step(s32);
extern void bunkasai_girl2_home_srf_bank_set(void);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);

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
void bunkasai_step3f(void) {
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
        bustup_load_step(21038);
        break;
    case 4:
        bustup_ptr_init();
        now_girl = 2;
        zyotai_sprite_set_chk();
        bunkasai_basyo_init();
        cd_read_start2(10, 0x801E8000, 18988);
        break;
    case 5:
        fade_in();
        break;
    case 6:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(2);
            switch (v0) {

                break;
            case 0: case 1: case 2:
                now_serifu = 55;
                break;
            case 3:
                now_serifu = 57;
                break;
            case 4:
                now_serifu = 59;
                break;

                break;
            }
            bunkasai_girl2_home_srf_bank_set();
        }
        break;
    case 7:
        serifu_run();
        break;
    case 8:
        v0 = get_g_zyotai_low(2);
        if (v0 == 4) {
            fade_down_main();
        } else {
            goto_sub_sub_step(10);
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        v0 = get_g_zyotai_low(2);
        if (v0 == 4) {
            sub_sub_step_up();
        } else {
            fade_down_main();
        }
        break;
    case 11:
        ret_step = now_step;
        ret_sub_step = now_sub_step;
        ret_sub_sub_step = now_sub_sub_step + 1;
        bunkaken_flags = (bunkaken_flags | 2) & 0xFFE3;
        goto_sub_step(164);
        break;
    case 12:
        common_disp_off = 1;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_sub_step(163);
        break;
    case 13:
        device_cmd_send(191);
        device_load_start(1, 0xC486, 0xC459, 0xD266, 0xD216, 0xD20E);
        date_x_taku_set(1, 0);
        bunkasai_basyo_init();
        mes_base_y = 192;
        message_window_init();
        hizuke_init();
        yazirushi_off();
        date_name_set(2);
        common_disp_off = 0;
        sub_sub_step_up();
        break;
    case 14:
        if (device_load_poll() == 1) {
            device_cmd_send(512);
            dec_bg_reset();
            set_dec_bri(0);
            value_cache_find_or_load(16645, 1);
            sub_sub_step_up();
        }
        break;
    case 15:
        if (get_CD_State2() & 1) {
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            sub_sub_step_up();
        }
        break;
    case 16:
        bustup_load_step(21038);
        break;
    case 17:
        bustup_ptr_init();
        now_girl = 2;
        zyotai_sprite_set_chk();
        cd_read_start2(10, 0x801E8000, 18988);
        break;
    case 18:
        v0 = get_g_zyotai_low(2);
        if (v0 == 4) {
            fade_in_step();
            break;
        }

        fade_in();
        break;
    case 19:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(2);
            switch (v0) {

                break;
            case 0: case 1: case 2:
                now_serifu = 56;
                break;
            case 3:
                now_serifu = 58;
                break;
            case 4:
                now_serifu = 68;
                break;

                break;
            }
            bunkasai_girl2_home_srf_bank_set();
        }
        break;
    case 20:
        serifu_run();
        break;
    case 21:
        v0 = get_g_zyotai_low(2);
        if (v0 == 4) {
            fade_out_step();
        } else {
            fade_out();
        }
        break;
    case 22:
        now_serifu = 1;
        sub_sub_step_up();
        break;
    case 23:
        serifu_run();
        break;
    case 24:
        bunkaken_flags |= 1;
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
