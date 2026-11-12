/* @SymbolStart: 0x80151A10 */
/* @RodataBase: 0x8015F2DC */
/* @SymbolSize: 1280 */
/* @PostProc: patch_insn_reg_field 0x11c:rt:t0:v1 0x120:rs:t0:v1 0x120:rt:t0:v1 0x128:rs:t0:v1 0x140:rd:t1:t0 0x148:rt:t1:t0 0x14c:rt:t1:t0 0x154:rs:t1:t0 0x15c:rt:t2:v0 0x168:rt:t2:v0 0x16c:rt:t3:v0 0x178:rt:t3:v0 0x17c:rt:t4:v0 0x188:rt:t4:v0 0x18c:rt:t5:v0 0x198:rt:t5:v0 0x1ac:rd:t6:t1 0x1b4:rt:t6:t1 0x1b8:rt:t6:t1 0x1c0:rs:t6:t1 0x1c8:rt:t7:v0 0x1d4:rt:t7:v0 0x1d8:rt:t8:v0 0x1e4:rt:t8:v0 0x1e8:rt:t9:v0 0x1f0:rt:t9:v0 0x1f4:rt:t0:v1 0x1f8:rs:t0:v1 0x1f8:rt:t0:v1 0x204:rd:t1:t2 0x204:rt:t0:v1 0x208:rd:t1:t2 0x208:rs:t1:t2 0x208:rt:t0:v1 0x20c:rd:t2:v0 0x20c:rs:t1:v0 0x20c:rt:v0:t2 0x210:rs:t2:v0 0x210:rt:t3:t4 0x218:rt:t3:t4 0x23c:rt:t4:v1 0x240:rs:t4:v1 0x240:rt:t4:v1 0x248:rs:t4:v1 0x260:rt:t5:v0 0x26c:rt:t5:v0 0x288:rt:t6:v1 0x28c:rs:t6:v1 0x28c:rt:t6:v1 0x294:rs:t6:v1 0x2e0:rt:t7:t5 0x2e4:rs:t7:t5 0x2e4:rt:t7:t5 0x2ec:rt:t7:t5 0x2f4:rt:t8:t6 0x2f8:rt:t8:t6 0x340:rt:t9:t7 0x344:rs:t9:t7 0x378:rt:t0:t8 0x384:rt:t0:t8 0x3c8:rt:t1:t9 0x3cc:rs:t1:t9 0x3f0:rt:t2:t0 0x3f4:rd:t3:t1 0x3f8:rd:t3:t1 0x3f8:rs:t3:t1 0x3fc:rd:t3:t1 0x3fc:rt:t3:t1 0x400:rd:t3:t1 0x400:rs:t3:t1 0x404:rs:t2:t0 0x404:rt:t2:t0 0x408:rd:t3:t1 0x408:rt:t3:t1 0x40c:rs:t3:t1 0x40c:rt:t4:t2 0x410:rs:t2:t0 0x410:rt:t4:t2 0x430:rt:t5:t3 0x434:rd:t6:t4 0x438:rd:t6:t4 0x438:rs:t6:t4 0x43c:rd:t6:t4 0x43c:rt:t6:t4 0x440:rd:t6:t4 0x440:rs:t6:t4 0x444:rs:t5:t3 0x444:rt:t5:t3 0x448:rd:t6:t4 0x448:rt:t6:t4 0x44c:rs:t6:t4 0x44c:rt:t7:t5 0x450:rs:t5:t3 0x450:rt:t7:t5 0x474:rt:t8:t7 0x47c:rt:t8:t7 0x4c0:rt:t9:v0 0x4cc:rt:t9:v0 */
/* Address: 0x80151A18 */
/* Size: 1272 bytes (318 insns) */
#include "types.h"

extern u8 now_sub_sub_step;
extern void sub_sub_step_up(void);
extern void goto_step(s32);
extern void goto_sub_sub_step(s32);
extern s32  get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void game_reset(void);
extern void set_dec_bri(s32);
extern void dec_bg_reset(void);
extern void indexed_byte_lsb_set(s32, s32);
extern void value_cache_find_or_load(s32, s32);
extern s32  dec_bg_init(s32);
extern void bustup_cd_read(s32);
extern void bustup_ptr_init(void);
extern void fade_down_main(void);
extern void fade_in(void);
extern void fade_out_step(void);
extern void fade_out(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32  get_g_zyotai_low(s32);
extern void bustup_load_step(s32);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init2(void);
extern void bunkasai_girl0_home_srf_bank_set(void);

extern volatile s16 now_serifu;
extern u8  now_year;
extern u16 bukatu_pt1;
extern u8  now_girl;
extern u8  now_sub_step;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

void bunkasai_girl0_home_club1_scene(void) {
    s32 v0;
    s32 n;
    s32 b;
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
            bustup_cd_read(21623);
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            sub_sub_step_up();
        }
        break;
    case 3:
        if (get_CD_State2() & 1) {
            bunkasai_basyo_init2();
            bustup_ptr_init();
            now_girl = 0;
            zyotai_sprite_set_chk();
            cd_read_start2(24, 0x801E8000, 18896);
        }
        break;
    case 4:
        fade_in();
        break;
    case 5:
        if (get_CD_State2() & 1) {
            if (now_year == 95) {
                v0 = get_g_zyotai_low(0);
                switch (v0) {
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
            } else {
                v0 = get_g_zyotai_low(0);
                switch (v0) {
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
                b = now_year;
                n = now_serifu;
                now_serifu = n + b * 3 - 288;
            }
            bunkasai_girl0_home_srf_bank_set();
        }
        break;
    case 6:
        serifu_run();
        break;
    case 7:
        if (now_year == 96 && get_g_zyotai_low(0) == 4) {
            now_serifu = 177;
            fade_down_main();
            break;
        }
        fade_out();
        break;
    case 8:
        if (now_year == 96 && get_g_zyotai_low(0) == 4) {
            serifu_run();
            break;
        }
        goto_sub_sub_step(10);
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
            bustup_load_step(21623);
        }
        break;
    case 13:
        v0 = dec_bg_init(1);
        indexed_byte_lsb_set(1, v0);
        bunkasai_basyo_init2();
        mes_base_y = 192;
        bustup_ptr_init();
        now_girl = 0;
        zyotai_sprite_set_chk();
        cd_read_start2(24, 0x801E8000, 18896);
        break;
    case 14:
        fade_in();
        break;
    case 15:
        if (get_CD_State2() & 1) {
            if (get_g_zyotai_low(0) == 4) {
                b = now_year;
                if ((u32) bukatu_pt1 >= (u32) (b * 100 - 9400)) {
                    n = 46;
                } else {
                    n = 47;
                }
            } else {
                b = now_year;
                if ((u32) bukatu_pt1 >= (u32) (b * 100 - 9400)) {
                    n = 44;
                } else {
                    n = 45;
                }
            }
            now_serifu = n;
            if (b == 97) {
                now_serifu = n + 4;
            }
            bunkasai_girl0_home_srf_bank_set();
        }
        break;
    case 16:
        serifu_run();
        break;
    case 17:
        fade_down_main();
        break;
    case 18:
        fade_out_step();
        break;
    case 19:
        now_serifu = 1;
        sub_sub_step_up();
        break;
    case 20:
        serifu_run();
        break;
    case 21:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
