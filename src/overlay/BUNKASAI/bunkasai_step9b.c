/* @SymbolStart: 0x8013FEB4 */
/* @SymbolSize: 820 */
/* @RodataBase: 0x8015D258 */
/* Address: 0x8013FEBC */
/* Size: 812 bytes (203 insns) */
/* @PostProc: patch_insn_reg_field 0x30c:rt:t0:t1 0x314:rs:t0:t1 0x314:rt:t1:t2 0x318:rt:t1:t2 */
#include "types.h"

extern void sub_sub_step_up(void);
extern void goto_sub_sub_step(s32 a0);
extern s32 get_CD_State2(void);
extern void cd_read_start2(s32 a0, s32 a1, s32 a2);
extern void game_reset(void);
extern void indexed_byte_lsb_set(s32 a0, s32 a1);
extern void value_cache_find_or_load(s32 a0, s32 a1);
extern s32 dec_bg_init(s32 a0);
extern void bustup_cd_read(s32 a0);
extern void bustup_ptr_init(void);
extern void fade_up_main(void);
extern void fade_down_main(void);
extern void fade_in_step(void);
extern void fade_out_step(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32 get_g_zyotai_low(s32 a0);
extern void bunkasai_girl10_srf_bank_set(void);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);

extern s16 now_serifu;
extern u8 now_girl;
extern u8 girl10_flag_hi;

extern u8 now_sub_sub_step;
void bunkasai_step9b(void) {
    s32 dispatch = now_sub_sub_step;

    s32 ret;

    switch (dispatch) {
    case 0:
        value_cache_find_or_load(16618, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out_step();
        break;
    case 2:
        ret = get_CD_State2();
        if (ret & 1) {
            bunkasai_basyo_init();
            ret = dec_bg_init(1);
            indexed_byte_lsb_set(1, ret);
            bustup_cd_read(26933);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        ret = get_CD_State2();
        if (ret & 1) {
            bustup_ptr_init();
            now_girl = 10;
            zyotai_sprite_set_chk();
            now_serifu = 37;
        }
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        cd_read_start2(19, 0x801E8000, 19294);
        sub_sub_step_up();
        break;
    case 7:
        fade_up_main();
        break;
    case 8:
        ret = get_CD_State2();
        if (ret & 1) {
            ret = get_g_zyotai_low(10);
            switch (ret) {
            case 0: case 1:
                now_serifu = 7;
                break;
            case 2:
                now_serifu = 8;
                break;
            case 3:
                now_serifu = 9;
                break;
            case 4:
                now_serifu = 10;
                break;

                break;
            }
            bunkasai_girl10_srf_bank_set();
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        ret = get_g_zyotai_low(10);
        if (ret == 4) {
            goto_sub_sub_step(12);
        } else {
            ret = get_g_zyotai_low(10);
            switch (ret) {
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
            sub_sub_step_up();
        }
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        fade_down_main();
        break;
    case 13:
        ret = get_g_zyotai_low(10);
        if (ret == 4) {
            serifu_run();
        } else {
            sub_sub_step_up();
        }
        break;
    case 14:
        fade_out_step();
        break;
    case 15:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 16:
        serifu_run();
        break;
    case 17:
        ret = get_g_zyotai_low(10);
        if (ret != 4) {
            girl10_flag_hi = girl10_flag_hi | 0x20;
        }
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
