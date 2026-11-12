/* @SymbolStart: 0x8014DFF8 */
/* @RodataBase: 0x801556F8 */
/* @SymbolSize: 1060 */
/* @PostProc: patch_insn_reg_field 0x1a4:rt:t6:t7 0x1a8:rd:t5:t6 0x1ac:rs:t5:t6 0x1c4:rt:t6:t7 0x1e8:rt:t7:t8 0x1f4:rt:t7:t8 0x218:rt:t8:t9 0x21c:rs:t8:t9 0x254:rt:t9:t0 0x258:rs:t9:t0 0x29c:rt:t0:t1 0x2a0:rs:t0:t1 0x2a0:rt:t0:t1 0x2a8:rs:t0:t1 0x2ec:rt:t2:t3 0x2f8:rt:t3:t4 0x304:rt:t1:t2 0x310:rt:t1:t2 0x31c:rt:t2:t3 0x324:rt:t3:t4 0x338:rt:t6:t7 0x33c:rt:t4:t5 0x340:rs:t4:t5 0x340:rt:t4:t5 0x348:rs:t4:t5 0x34c:rt:t5:t6 0x354:rt:t5:t6 0x358:rt:t7:t8 0x35c:rs:t7:t8 0x35c:rt:t7:t8 0x360:rs:t6:t7 0x360:rt:t6:t7 0x364:rd:t8:t9 0x364:rt:t7:t8 0x368:rd:t9:t0 0x368:rs:t6:t7 0x368:rt:t8:t9 0x36c:rs:t9:t0 0x36c:rt:t0:t1 0x378:rt:t0:t1 0x3a4:rt:t1:t2 0x3a8:rs:t1:t2 0x3a8:rt:t1:t2 0x3b0:rs:t1:t2 */
/* Address: 0x8014E000 */
/* Size: 1052 bytes (263 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void goto_step(s32);
extern void goto_sub_sub_step(s32);
extern s32  get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void game_reset(void);
extern void indexed_byte_lsb_set(s32, s32);
extern void value_cache_find_or_load(s32, s32);
extern s32  dec_bg_init(s32);
extern void bustup_cd_read(s32);
extern void bustup_ptr_init(void);
extern void fade_down_main(void);
extern void fade_in_step(void);
extern void fade_in(void);
extern void fade_out_step(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32  get_g_zyotai_low(s32);
extern void bunkaken_basyo_init(void);
extern void bunkaken_sub_reset(void);
extern void bunkaken_basyo_init2(void);
extern void bunkaken_basyo_init3(void);
extern void bunkaken_param_set2(void);

extern s32 bunka_girl2;
extern s32 bunka_sd_kind;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 now_serifu;
extern u8  now_year;
extern u8  now_girl;
extern u8  now_sub_step;
extern s32 girl0_flag;
extern u8 now_sub_sub_step;

void bunkaken_main2b(void)
{
    s32 v0;
    s32 ret;

    switch (now_sub_sub_step) {
    case 0:
        value_cache_find_or_load(18044, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out_step();
        break;
    case 2:
        if (get_CD_State2() & 1) {
            bunkaken_basyo_init3();
            ret = dec_bg_init(1);
            indexed_byte_lsb_set(1, ret);
            cd_read_start2(23, 0x801E8000, 18923);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            now_serifu = 158;
            bunkaken_param_set2();
        }
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        fade_out_step();
        break;
    case 7:
        bunka_sd_kind = 1;
        bunka_ret_sub_step = now_sub_step;

        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_step(128);
        break;
    case 8:
        bunkaken_basyo_init2();
        cd_read_start2(23, 0x801E8000, 18923);
        sub_sub_step_up();
        break;
    case 9:
        if (get_CD_State2() & 1) {
            s32 t4;
            bunkaken_param_set2();
            t4 = girl0_flag << 29;
            if (t4 < 0) {
                goto_sub_sub_step(11);
            } else {
                now_serifu = 5;
            }
            bustup_cd_read(21578);
        }
        break;
    case 10:
        serifu_run();
        break;
    case 11:
        now_serifu = 6;
        sub_sub_step_up();
        break;
    case 12:
        serifu_run();
        break;
    case 13:
        if (get_CD_State2() & 1) {
            bustup_ptr_init();
            now_girl = 0;
            zyotai_sprite_set_chk();
            value_cache_find_or_load(16628, 1);
        }
        break;
    case 14:
        if (get_CD_State2() & 1) {
            bunkaken_basyo_init();
            ret = dec_bg_init(1);
            indexed_byte_lsb_set(1, ret);
            sub_sub_step_up();
        }
        break;
    case 15:
        v0 = get_g_zyotai_low(0);
        if (v0 == 4 || bunka_girl2 == 0) {
            fade_in_step();
        } else {
            fade_in();
        }
        break;
    case 16:
        v0 = get_g_zyotai_low(0);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 159;
            break;
        case 2:
            now_serifu = 160;
            break;
        case 3:
            now_serifu = 161;
            break;
        }
        v0 = get_g_zyotai_low(0);
        if (v0 == 4 || bunka_girl2 == 0) {
            now_serifu = 162;
        }
        now_serifu = (s16)(now_serifu + now_year * 4 - 380);
        sub_sub_step_up();
        break;
    case 17:
        serifu_run();
        break;
    case 18:
        v0 = get_g_zyotai_low(0);
        if (v0 == 4 || bunka_girl2 == 0) {
            sub_sub_step_up();
        } else {
            fade_down_main();
        }
        break;
    case 19:
        fade_out_step();
        break;
    case 20:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 21:
        serifu_run();
        break;
    case 22:
        bunkaken_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
