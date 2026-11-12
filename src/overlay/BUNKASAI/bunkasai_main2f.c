/* @SymbolStart: 0x801325F4 */
/* @SymbolSize: 736 */
/* @RodataBase: 0x8015B928 */
/* Address: 0x801325F4 (absorbed head)  Size: 736 bytes (184 insns) */
/* @PostProc: patch_insn_reg_field 0x29c:rt:t5:t6 0x2a4:rt:t3:t4 0x2ac:rt:t4:t5 0x2ac:rs:t3:t4 0x2b4:rt:t4:t5 0x2b8:rt:t5:t7 0x2b8:rs:t5:t6 0x2c0:rt:t6:t8 0x2c0:rs:t5:t7 0x2c4:rt:t6:t8 */
#include "types.h"

extern void sub_sub_step_up(void);
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
extern void bunkasai_girl2_vst_bank_init(void);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);

extern u8 now_sub_sub_step;
extern u8 now_year;
extern u8 girl2_flag_hi;
extern s16 now_serifu;
extern u8 now_girl;

void bunkasai_main2f(void) {
    s32 dispatch;
    s32 ret;

    dispatch = now_sub_sub_step;
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
            bustup_cd_read(21038);
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
            now_girl = 2;
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
        cd_read_start2(26, 0x801E8000, 18962);
        sub_sub_step_up();
        break;
    case 7:
        fade_up_main();
        break;
    case 8:
        ret = get_CD_State2();
        if (ret & 1) {
            ret = get_g_zyotai_low(2);
            switch (ret) {
            case 0: case 1:
                now_serifu = 1;
                break;
            case 2:
                now_serifu = 3;
                break;
            case 3:
                now_serifu = 5;
                break;
            case 4:
                now_serifu = 7;
                break;

                break;
            }
            if (now_year == 96) {
                now_serifu = now_serifu + 8;
            }
            bunkasai_girl2_vst_bank_init();
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        fade_down_main();
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        fade_out_step();
        break;
    case 13:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 14:
        serifu_run();
        break;
    case 15:
        ret = get_g_zyotai_low(2);
        if ((u32)ret < 4) {
            if (now_year == 95) {
                girl2_flag_hi = girl2_flag_hi | 0x10;
            } else {
                girl2_flag_hi = girl2_flag_hi | 0x20;
            }
        }
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
