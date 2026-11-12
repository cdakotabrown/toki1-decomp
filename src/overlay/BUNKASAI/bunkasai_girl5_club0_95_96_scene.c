/* @SymbolStart: 0x8014AD34 */
/* @RodataBase: 0x8015E5D8 */
/* Address: 0x8014AD3C */
/* Size: 1056 bytes (264 insns) */
/* @SymbolSize: 1064 */
/* @PostProc: patch_insn_reg_field 0x3e4:rt:t5:t6 0x3ec:rt:t3:t4 0x3f4:rs:t3:t4 0x3f4:rt:t4:t5 0x3fc:rt:t4:t5 0x400:rs:t5:t6 0x400:rt:t5:t7 0x408:rs:t5:t7 0x408:rt:t6:t8 0x40c:rt:t6:t8 */
#include "types.h"

extern u8 now_sub_sub_step;
extern void sub_sub_step_up(void);
extern s32 get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void game_reset(void);
extern void indexed_byte_lsb_set(s32, s32);
extern void value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);
extern void bustup_cd_read(s32);
extern void bustup_ptr_init(void);
extern void fade_up_main(void);
extern void fade_down_main(void);
extern void fade_in_step(void);
extern void fade_out_step(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32 get_g_zyotai_low(s32);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);
extern void bunkasai_girl5_srf_bank_set(void);

extern s16 now_serifu;
extern u8  now_year;
extern u8  girl5_flag_hi;
extern u8  now_girl;

void bunkasai_girl5_club0_95_96_scene(void) {
    s32 idx;

    s32 v0;

    idx = now_sub_sub_step;
    switch (idx) {
    case 0:
        value_cache_find_or_load(16618, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out_step();
        break;
    case 2:
        if (get_CD_State2() & 1) {
            bunkasai_basyo_init();
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            bustup_cd_read(24503);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            bustup_ptr_init();
            now_girl = 5;
            zyotai_sprite_set_chk();
            if (now_year == 95) {
                now_serifu = 36;
            } else {
                now_serifu = 37;
            }
        }
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        cd_read_start2(13, 0x801E8000, 19045);
        sub_sub_step_up();
        break;
    case 7:
        fade_up_main();
        break;
    case 8:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(5);
            switch (v0) {

                break;
            case 0: case 1:
                now_serifu = 1;
                break;
            case 2:
                now_serifu = 2;
                break;
            case 3:
                now_serifu = 3;
                break;
            case 4:
                now_serifu = 4;
                break;
            }
            bunkasai_girl5_srf_bank_set();
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        v0 = get_g_zyotai_low(5);
        switch (v0) {

            break;
        case 0: case 1:
            now_serifu = 16;
            break;
        case 2:
            now_serifu = 17;
            break;
        case 3:
            now_serifu = 18;
            break;
        case 4:
            now_serifu = 19;
            break;
        }
        if (now_year == 96) {
            now_serifu = now_serifu + 4;
        }
        sub_sub_step_up();
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        v0 = get_g_zyotai_low(5);
        switch (v0) {

            break;
        case 0: case 1:
            now_serifu = 8;
            break;
        case 2:
            now_serifu = 9;
            break;
        case 3:
            now_serifu = 10;
            break;
        case 4:
            now_serifu = 54;
            sub_sub_step_up();
            break;
        }
        sub_sub_step_up();
        break;
    case 13:
        serifu_run();
        break;
    case 14:
        fade_down_main();
        break;
    case 15:
        v0 = get_g_zyotai_low(5);
        if (v0 == 4) {
            serifu_run();
        } else {
            sub_sub_step_up();
        }
        break;
    case 16:
        sub_sub_step_up();
        break;
    case 17:
        fade_out_step();
        break;
    case 18:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 19:
        serifu_run();
        break;
    case 20:
        if (get_g_zyotai_low(5) != 4) {
            if (now_year == 95) {
                girl5_flag_hi |= 0x10;
            } else {
                girl5_flag_hi |= 0x20;
            }
        }
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
