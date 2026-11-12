/* @SymbolStart: 0x8014DB74 */
/* @RodataBase: 0x801556A4 */
/* @SymbolSize: 1156 */
/* Address: 0x8014DB7C */
/* Size: 1148 bytes (287 insns) */
/* @PostProc: patch_insn_reg_field 0x210:rt:t7:t8 0x214:rd:t6:t7 0x218:rs:t6:t7 0x234:rt:t7:t8 0x26c:rt:t9:t0 0x278:rt:t0:t1 0x284:rt:t8:t9 0x290:rt:t8:t9 0x29c:rt:t9:t0 0x2a4:rt:t0:t1 0x2b8:rt:t1:t2 0x2bc:rs:t1:t2 0x2bc:rt:t1:t2 0x2c4:rs:t1:t2 0x2c8:rt:t2:t3 0x2d0:rt:t2:t3 0x304:rt:t3:t4 0x308:rs:t3:t4 0x308:rt:t3:t4 0x30c:rt:t4:t5 0x310:rs:t3:t4 0x32c:rt:t4:t5 0x358:rt:t5:t6 0x35c:rs:t5:t6 0x35c:rt:t5:t6 0x364:rs:t5:t6 0x398:rt:t7:t8 0x3a4:rt:t8:t9 0x3b0:rt:t6:t7 0x3bc:rt:t6:t7 0x3c8:rt:t7:t8 0x3d0:rt:t8:t9 0x404:rt:t9:t0 0x408:rs:t9:t0 0x408:rt:t9:t0 0x410:rs:t9:t0 */
#include "types.h"

extern void sub_sub_step_up(void);
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
extern void fade_out(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32  get_g_zyotai_low(s32);
extern void bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);
extern void bunkaken_param_set2(void);

extern s32 bunka_girl2;
extern s16 now_serifu;
extern u8  now_girl;
extern s32 girl0_flag;
extern u8 now_sub_sub_step;

void bunkaken_main2a(void)
{
    s32 idx = now_sub_sub_step;
    s32 v0;
    s32 ret;

    switch (idx) {
    case 0:
        value_cache_find_or_load(16655, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out_step();
        break;
    case 2:
        if (get_CD_State2() & 1) {
            ret = dec_bg_init(1);
            indexed_byte_lsb_set(1, ret);
            bustup_cd_read(21668);
            sub_sub_step_up();
        }
        break;
    case 3:
        if (get_CD_State2() & 1) {
            bustup_ptr_init();
            now_girl = 0;
            zyotai_sprite_set_chk();
            cd_read_start2(23, 0x801E8000, 18923);
        }
        break;
    case 4:
        bunkaken_basyo_init3();
        sub_sub_step_up();
        break;
    case 5:
        v0 = get_g_zyotai_low(0);
        if (v0 == 4 || bunka_girl2 == 0) {
            fade_in_step();
        } else {
            fade_in();
        }
        break;
    case 6:
        if (get_CD_State2() & 1) {
            bunkaken_param_set2();
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
            v0 = get_g_zyotai_low(0);
            if (v0 == 4 || bunka_girl2 == 0) {
                goto_sub_sub_step(10);
            }
        }
        break;
    case 7:
        serifu_run();
        break;
    case 8:
        {
            s32 t5 = girl0_flag;
            s32 v0b = 4;
            s32 t7 = t5 << 29;
            if (t7 < 0) {
                goto_sub_sub_step(10);
            } else {
                now_serifu = (s16)v0b;
                sub_sub_step_up();
            }
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        v0 = get_g_zyotai_low(0);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 149;
            break;
        case 2:
            now_serifu = 151;
            break;
        case 3:
            now_serifu = 153;
            break;
        }
        v0 = get_g_zyotai_low(0);
        if (v0 == 4 || bunka_girl2 == 0) {
            now_serifu = 155;
        }
        sub_sub_step_up();
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        v0 = get_g_zyotai_low(0);
        if (v0 == 4 || bunka_girl2 == 0) {
            fade_out_step();
        } else {
            now_serifu = 156;
            fade_out();
        }
        break;
    case 13:
        serifu_run();
        break;
    case 14:
        v0 = get_g_zyotai_low(0);
        if (v0 == 4 || bunka_girl2 == 0) {
            fade_in_step();
        } else {
            v0 = get_g_zyotai_low(0);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 150;
                break;
            case 2:
                now_serifu = 152;
                break;
            case 3:
                now_serifu = 154;
                break;
            }
            fade_in();
        }
        break;
    case 15:
        serifu_run();
        break;
    case 16:
        v0 = get_g_zyotai_low(0);
        if (v0 == 4 || bunka_girl2 == 0) {
            sub_sub_step_up();
        } else {
            fade_down_main();
        }
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
        bunkaken_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
