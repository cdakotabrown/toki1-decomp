/* @SymbolStart: 0x801438E4 */
/* @RodataBase: 0x8015D808 */
/* Address: 0x801438EC */
/* Size: 844 bytes (211 insns) */
/* @SymbolSize: 852 */
/* @PostProc: patch_insn_reg_field 0x310:rt:t9:t0 0x318:rt:t7:t8 0x320:rs:t7:t8 0x320:rt:t8:t9 0x328:rt:t8:t9 0x32c:rs:t9:t0 0x32c:rt:t9:t1 0x334:rs:t9:t1 0x334:rt:t0:t2 0x338:rt:t0:t2 */
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
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);
extern void bunkasai_girl7_srf_bank_set(void);

extern s16 now_serifu;
extern u8  now_year;
extern u8  girl7_flag_hi;
extern u8  now_girl;

extern u8 now_sub_sub_step;
void bunkasai_step7a(void) {
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
            bustup_cd_read(23333);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            bustup_ptr_init();
            now_girl = 7;
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
        cd_read_start2(15, 0x801E8000, 19009);
        sub_sub_step_up();
        break;
    case 7:
        fade_up_main();
        break;
    case 8:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(7);
            switch (v0) {

                break;
            case 0: case 1:
                now_serifu = 5;
                break;
            case 2:
                now_serifu = 6;
                break;
            case 3:
                now_serifu = 7;
                break;
            case 4:
                now_serifu = 8;
                break;
            }
            if (now_year == 96) {
                now_serifu = now_serifu + 4;
            }
            bunkasai_girl7_srf_bank_set();
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        v0 = get_g_zyotai_low(7);
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
            sub_sub_step_up();
            break;
        }
        sub_sub_step_up();
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        fade_down_main();
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
        if (get_g_zyotai_low(7) == 4) goto L16_call;
        if (now_year != 95) goto L16_b;
        girl7_flag_hi |= 0x10;
        goto L16_call;
L16_b:
        girl7_flag_hi |= 0x20;
L16_call:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
