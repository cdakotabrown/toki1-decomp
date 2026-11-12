/* @SymbolStart: 0x80136414 */
/* @RodataBase: 0x8015C078 */
/* Address: 0x8013641C */
/* Size: 900 bytes (225 insns) */
/* @SymbolSize: 908 */
/* @PostProc: patch_insn_reg_field 0x348:rt:t7:t8 0x350:rt:t5:t6 0x358:rs:t5:t6 0x358:rt:t6:t7 0x360:rt:t6:t7 0x364:rs:t7:t8 0x364:rt:t7:t9 0x36c:rs:t7:t9 0x36c:rt:t8:t0 0x370:rt:t8:t0 */
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
extern void bunkasai_girl4_srf_bank_set(void);

extern s16 now_serifu;
extern u8  now_year;
extern u8  girl4_flag_hi;
extern u8  now_girl;

extern u8 now_sub_sub_step;
void bunkasai_girl4_club0_95_96_scene(void) {
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
            bustup_cd_read(26303);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            bustup_ptr_init();
            now_girl = 4;
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
        cd_read_start2(10, 0x801E8000, 19271);
        sub_sub_step_up();
        break;
    case 7:
        fade_up_main();
        break;
    case 8:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(4);
            switch (v0) {

                break;
            case 0: case 1:
                now_serifu = 5;
                break;
            case 2: case 3:
                now_serifu = 6;
                break;
            case 4:
                now_serifu = 7;
                break;

                break;
            }
            if (now_year == 96) {
                now_serifu = now_serifu + 3;
            }
            bunkasai_girl4_srf_bank_set();
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        v0 = get_g_zyotai_low(4);
        if (v0 == 4) {
            goto_sub_sub_step(12);
            break;
        }

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
    case 11:
        serifu_run();
        break;
    case 12:
        now_serifu = 86;
        fade_down_main();
        break;
    case 13:
        v0 = get_g_zyotai_low(4);
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
    case 16:
        serifu_run();
        break;
    case 17:
        if (get_g_zyotai_low(4) == 4) goto L16_call;
        if (now_year != 95) goto L16_b;
        girl4_flag_hi |= 0x10;
        goto L16_call;
L16_b:
        girl4_flag_hi |= 0x20;
L16_call:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
