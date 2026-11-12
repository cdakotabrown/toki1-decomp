/* @SymbolStart: 0x80138BE4 */
/* @RodataBase: 0x8015C598 */
/* Address: 0x80138BEC */
/* Size: 780 bytes (195 insns) */
/* @SymbolSize: 788 */
/* @PostProc: patch_insn_reg_field 0x2d0:rt:t5:t6 0x2d8:rt:t3:t4 0x2e0:rs:t3:t4 0x2e0:rt:t4:t5 0x2e8:rt:t4:t5 0x2ec:rs:t5:t6 0x2ec:rt:t5:t7 0x2f4:rs:t5:t7 0x2f4:rt:t6:t8 0x2f8:rt:t6:t8 */
#include "types.h"

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
extern void bunkasai_girl8_srf_bank_set(void);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);

extern s16 now_serifu;
extern u8  now_year;
extern u8  girl8_flag_hi;
extern u8  now_girl;

extern u8 now_sub_sub_step;
void bunkasai_talk_b2(void) {
    s32 idx = now_sub_sub_step;

    s32 v0;

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
            bustup_cd_read(20498);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            bustup_ptr_init();
            now_girl = 8;
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
        sub_sub_step_up();
        break;
    case 7:
        cd_read_start2(20, 0x801E8000, 18833);
        sub_sub_step_up();
        break;
    case 8:
        fade_up_main();
        break;
    case 9:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(8);
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
            if (now_year == 96) {
                now_serifu = now_serifu + 5;
            }
            bunkasai_girl8_srf_bank_set();
        }
        break;
    case 10:
        serifu_run();
        break;
    case 11:
        fade_down_main();
        break;
    case 12:
        v0 = get_g_zyotai_low(8);
        if (v0 == 4) {
            serifu_run();
        } else {
            sub_sub_step_up();
        }
        break;
    case 13:
        fade_out_step();
        break;
    case 14:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 15:
        serifu_run();
        break;
    case 16:
        if (get_g_zyotai_low(8) != 4) {
            if (now_year == 95) {
                girl8_flag_hi |= 0x10;
            } else {
                girl8_flag_hi |= 0x20;
            }
        }
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
