/* @SymbolStart: 0x80137788 */
/* Address: 0x80137790 */
/* Size: 1608 bytes (402 insns) */
/* @SymbolSize: 1616 */
/* @RodataBase: 0x8015C30C */
/* @PostProc: swap_instructions --offset1 0x260 --offset2 0x264 */
/* @PostProc: swap_instructions --offset1 0x268 --offset2 0x26c */
/* @PostProc: patch_insn_reg_field 0x250:rt:t5:t0 0x254:rs:t5:t0 0x254:rt:t5:t0 0x25c:rt:t6:t1 0x260:rt:t5:t0 0x264:rs:t6:t1 0x264:rt:t6:t1 0x268:rt:t8:v1 0x270:rs:t8:v1 0x270:rt:t8:t5 0x274:rt:t6:t1 0x27c:rt:t7:t2 0x280:rt:t7:t2 0x284:rs:t8:t5 0x284:rt:t9:t7 0x288:rs:t9:t7 0x288:rt:t0:t9 0x290:rt:t0:t9 0x2a4:rt:t1:t3 0x2ac:rt:t2:t4 0x2b0:rs:t2:t4 0x2b0:rt:t2:t4 0x2b4:rt:t1:t3 0x2bc:rt:t2:t4 0x2c4:rt:t3:t5 0x2c8:rt:t3:t5 0x2e4:rt:t4:t6 0x2e8:rt:t5:t7 0x2ec:rt:t5:t7 0x2f0:rt:t4:t6 0x31c:rt:t6:t8 0x328:rt:t6:t8 0x3a8:rt:t7:t9 0x3ac:rs:t7:t9 0x3f0:rt:t8:t0 0x3fc:rt:t8:t0 0x458:rt:t9:t1 0x45c:rs:t9:t1 0x474:rd:t0:t2 0x47c:rt:t0:t2 0x480:rt:t0:t2 0x488:rs:t0:t2 0x490:rt:t1:t3 0x49c:rt:t1:t3 0x4a0:rt:t2:t4 0x4ac:rt:t2:t4 0x4b0:rt:t3:t5 0x4b8:rt:t3:t5 0x4bc:rt:t4:t6 0x4c0:rs:t4:t6 0x4c0:rt:t4:t6 0x4c4:rt:t5:t7 0x4c8:rs:t4:t6 0x4d4:rs:t5:t7 0x4d4:rt:t5:t7 0x4dc:rs:t5:t7 0x4dc:rt:t6:t8 0x4e0:rt:t6:t8 0x51c:rd:t7:t9 0x524:rt:t7:t9 0x528:rt:t7:t9 0x530:rs:t7:t9 0x538:rt:t8:t0 0x544:rt:t8:t0 0x548:rt:t9:t1 0x554:rt:t9:t1 0x594:rt:t1:t3 0x5ac:rt:t0:t2 0x5b8:rt:t0:t2 0x5c0:rt:t1:t3 0x628:rt:t2:v1 0x62c:rs:t2:v1 0x62c:rt:t2:t4 0x634:rs:t2:t4 0x634:rt:t3:t6 0x63c:rt:t3:t6 */
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
extern void dec_bg_reset(void);
extern void set_dec_bri(s32);
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
extern void yazirushi_off(void);
extern void serifu_run(void);
extern void date_x_taku_set(s32, s32);
extern s32 get_g_zyotai_low(s32);
extern void bustup_load_step(s32);
extern void date_name_set(s32);
extern void bunkasai_girl4_srf_bank_set(void);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);

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
void bunkasai_main5c(void) {
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
        bustup_load_step(26303);
        break;
    case 4:
        bustup_ptr_init();
        now_girl = 4;
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
        cd_read_start2(10, 0x801E8000, 19271);
        sub_sub_step_up();
        break;
    case 8:
        fade_up_main();
        break;
    case 9:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(4);
            switch (v0) {

                break;
            case 0: case 1:
                now_serifu = 47;
                break;
            case 2: case 3:
                now_serifu = 51;
                break;
            case 4:
                now_serifu = 55;
                break;

                break;
            }
            bunkasai_girl4_srf_bank_set();
        }
        break;
    case 10:
        serifu_run();
        break;
    case 11:
        v0 = get_g_zyotai_low(4);
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
        v0 = get_g_zyotai_low(4);
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
        date_name_set(4);
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
        bustup_load_step(26303);
        break;
    case 21:
        bustup_ptr_init();
        now_girl = 4;
        zyotai_sprite_set_chk();
        cd_read_start2(10, 0x801E8000, 19271);
        break;
    case 22:
        v0 = get_g_zyotai_low(4);
        if (v0 == 4) {
            fade_in_step();
        } else {
            fade_in();
        }
        break;
    case 23:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(4);
            switch (v0) {

                break;
            case 0: case 1:
                now_serifu = 48;
                break;
            case 2: case 3:
                now_serifu = 52;
                break;
            case 4:
                now_serifu = 57;
                break;

                break;
            }
            if ((u32)tt_result < 2) {
                now_serifu = now_serifu + 1;
            }
            bunkasai_girl4_srf_bank_set();
        }
        break;
    case 24:
        serifu_run();
        break;
    case 25:
        sub_sub_step_up();
        v0 = get_g_zyotai_low(4);
        switch (v0) {

            break;
        case 0: case 1:
            now_serifu = 50;
            break;
        case 2: case 3:
            now_serifu = 54;
            break;
        case 4:
            goto_sub_sub_step(30);
            break;

            break;
        }
        break;
    case 26:
        serifu_run();
        break;
    case 27:
        v0 = get_g_zyotai_low(4);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 1;
            break;
        case 2:
        case 3:
            now_serifu = 2;
            break;
        }
        sub_sub_step_up();
        break;
    case 28:
        serifu_run();
        break;
    case 29:
        fade_down_main();
        break;
    case 30:
        fade_out_step();
        break;
    case 31:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 32:
        serifu_run();
        break;
    case 33:
        bunkaken_flags |= 1;
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
