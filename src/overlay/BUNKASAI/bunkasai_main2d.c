/* @SymbolStart: 0x80135C4C */
/* Address: 0x80135C54 */
/* Size: 1500 bytes (375 insns) */
/* @SymbolSize: 1508 */
/* @RodataBase: 0x8015BFC0 */
/* @CompileFlags: -cckr */
#include "types.h"

extern void sub_sub_step_up(void);
extern void goto_step(s32);
extern void goto_sub_sub_step(s32);
extern s32 get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void game_reset(void);
extern void dec_bg_reset(void);
extern void set_dec_bri(s32);
extern void indexed_byte_lsb_set(s32, s32);
extern void value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);
extern void bustup_ptr_init(void);
extern void fade_down_main(void);
extern void fade_in(void);
extern void fade_out_step(void);
extern void fade_out(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32 get_g_zyotai_low(s32);
extern void bustup_load_step(s32);
extern void bunkasai_girl1_cl_bank_init(void);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init2(void);

extern s16  now_serifu;
extern u8   now_year;
extern u16  bukatu_pt1;
extern u8   now_girl;
extern u8   now_sub_step;
extern s32  bunka_ret_sub_step;
extern s32  bunka_ret_sub_sub_step;
extern s16  mes_base_y;

extern u8 now_sub_sub_step;
void bunkasai_main2d(void) {
    switch (now_sub_sub_step) {
    case 0:
        value_cache_find_or_load(16628, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out_step();
        break;
    case 2:
        if (get_CD_State2() & 1) {
            bustup_load_step(23918);
        }
        break;
    case 3:
        indexed_byte_lsb_set(1, dec_bg_init(1));
        bunkasai_basyo_init2();
        bustup_ptr_init();
        now_girl = 1;
        zyotai_sprite_set_chk();
        cd_read_start2(11, 0x801E8000, 19113);
        break;
    case 4:
        fade_in();
        break;
    case 5:
        if (get_CD_State2() & 1) {
            if (now_year == 96) {
                switch (get_g_zyotai_low(1)) {
                case 0:
                case 1:
                    now_serifu = 38;
                    break;
                case 2:
                case 3:
                    now_serifu = 39;
                    break;
                case 4:
                    now_serifu = 40;
                    break;
                }
            } else {
                switch (get_g_zyotai_low(1)) {
                case 0:
                case 1:
                    now_serifu = 34;
                    break;
                case 2:
                    now_serifu = 35;
                    break;
                case 3:
                    now_serifu = 36;
                    break;
                case 4:
                    now_serifu = 37;
                    break;
                }
                if (now_year == 97) {
                    now_serifu += 7;
                }
            }
            bunkasai_girl1_cl_bank_init();
        }
        break;
    case 6:
        serifu_run();
        break;
    case 7:
        if (now_year == 97 && get_g_zyotai_low(1) == 4) {
            now_serifu = 62;
            fade_down_main();
        } else {
            fade_out();
        }
        break;
    case 8:
        if (now_year == 97 && get_g_zyotai_low(1) == 4) {
            serifu_run();
        } else {
            goto_sub_sub_step(10);
        }
        break;
    case 9:
        fade_out_step();
        break;
    case 10:
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_step(128);
        break;
    case 11:
        dec_bg_reset();
        set_dec_bri(0);
        value_cache_find_or_load(16628, 1);
        sub_sub_step_up();
        break;
    case 12:
        if (get_CD_State2() & 1) {
            bustup_load_step(23918);
        }
        break;
    case 13:
        indexed_byte_lsb_set(1, dec_bg_init(1));
        bunkasai_basyo_init2();
        mes_base_y = 192;
        bustup_ptr_init();
        now_girl = 1;
        zyotai_sprite_set_chk();
        cd_read_start2(11, 0x801E8000, 19113);
        break;
    case 14:
        fade_in();
        break;
    case 15:
        if (get_CD_State2() & 1) {
            switch (get_g_zyotai_low(1)) {
            case 0:
            case 1:
                now_serifu = 45;
                break;
            case 2:
                now_serifu = 47;
                break;
            case 3:
                now_serifu = 49;
                break;
            case 4:
                now_serifu = 51;
                break;
            }
            if ((u32)bukatu_pt1 < (u32)((s32)now_year * 100 - 9400)) {
                now_serifu++;
            }
            if (now_year == 97) {
                now_serifu += 8;
            }
            bunkasai_girl1_cl_bank_init();
        }
        break;
    case 16:
        serifu_run();
        break;
    case 17:
        fade_down_main();
        break;
    case 18:
        if (get_g_zyotai_low(1) == 4) {
            if ((u32)now_year < 97U
                && !((u32)bukatu_pt1 < (u32)((s32)now_year * 100 - 9400))) {
                now_serifu = 62;
                sub_sub_step_up();
            } else if (now_year == 97) {
                if ((u32)bukatu_pt1 >= 300) {
                    now_serifu = 62;
                    sub_sub_step_up();
                } else {
                    now_serifu = 63;
                    sub_sub_step_up();
                }
            } else {
                goto_sub_sub_step(20);
            }
        } else {
            goto_sub_sub_step(20);
        }
        break;
    case 19:
        serifu_run();
        break;
    case 20:
        fade_out_step();
        break;
    case 21:
        now_serifu = 1;
        sub_sub_step_up();
        break;
    case 22:
        serifu_run();
        break;
    case 23:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
