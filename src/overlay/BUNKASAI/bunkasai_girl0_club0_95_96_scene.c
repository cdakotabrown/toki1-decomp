/* @SymbolStart: 0x801481B0 */
/* @RodataBase: 0x8015DFF8 */
/* Address: 0x801481B8 */
/* Size: 876 bytes (219 insns) */
/* @SymbolSize: 884 */
/* @PostProc: patch_insn_reg_field 0x330:rt:t8:t9 0x338:rt:t6:t7 0x340:rs:t6:t7 0x340:rt:t7:t8 0x348:rt:t7:t8 0x34c:rs:t8:t9 0x34c:rt:t8:t0 0x354:rs:t8:t0 0x354:rt:t9:t1 0x358:rt:t9:t1 */
#include "types.h"

extern void sub_sub_step_up(void);
extern void goto_sub_sub_step(s32);
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
extern void bunkasai_param_set(void);

extern s16 now_serifu;
extern u8  now_year;
extern u8  girl0_flag_hi;
extern u8  now_girl;

extern u8 now_sub_sub_step;
void bunkasai_girl0_club0_95_96_scene(void) {
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
            bustup_cd_read(21578);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            bustup_ptr_init();
            now_girl = 0;
            zyotai_sprite_set_chk();
            if (now_year == 95) {
                now_serifu = 36;
            } else {
                now_serifu = 37;
            }
        }
        break;
    case 5: case 9: case 11: case 16:
        serifu_run();
        break;
    case 6:
        cd_read_start2(17, 0x801E8000, 18879);
        sub_sub_step_up();
        break;
    case 7:
        fade_up_main();
        break;
    case 8:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(0);
            switch (v0) {

                break;
            case 0: case 1:
                now_serifu = 10;
                break;
            case 2:
                now_serifu = 11;
                break;
            case 3:
                now_serifu = 12;
                break;
            case 4:
                now_serifu = 13;
                break;
            }
            if (now_year == 96) {
                now_serifu = now_serifu + 4;
            }
            bunkasai_param_set();
        }
        break;
    case 10:
        v0 = get_g_zyotai_low(0);
        if (v0 == 4) {
            goto_sub_sub_step(12);
            break;
        }


        v0 = get_g_zyotai_low(0);
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
        sub_sub_step_up();
        break;
    case 12:
        now_serifu = 144;
        fade_down_main();
        break;
    case 13:
        v0 = get_g_zyotai_low(0);
        if (v0 == 4) {
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


    case 17:
        if ((u32)get_g_zyotai_low(0) < 4) {
            if (now_year == 95) {
                girl0_flag_hi |= 0x10;
            } else {
                girl0_flag_hi |= 0x20;
            }
        }
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
