/* @RodataBase: 0x8015F6A4 */
/* Address: 0x80153A98 (absorbed head) / 0x80153AA0 (prologue) */
/* Size: 1292 bytes (323 insns) incl. the absorbed 2-word head */
/* @SymbolStart: 0x80153A98 */
/* @SymbolSize: 1292 */
/* @PostProc: patch_insn_reg_field 0x100:rt:t9:v0 0x104:rs:t9:v0 0x104:rt:t9:v0 0x10c:rs:t9:v0 0x124:rd:t0:t9 0x12c:rt:t0:t9 0x130:rt:t0:t9 0x138:rs:t0:t9 0x140:rt:t1:t0 0x14c:rt:t1:t0 0x150:rt:t2:t1 0x15c:rt:t2:t1 0x160:rt:t3:t2 0x16c:rt:t3:t2 0x170:rt:t4:t3 0x17c:rt:t4:t3 0x190:rd:t5:t4 0x198:rt:t5:t4 0x19c:rt:t5:t4 0x1a4:rs:t5:t4 0x1ac:rt:t6:t5 0x1b8:rt:t6:t5 0x1bc:rt:t7:t6 0x1c8:rt:t7:t6 0x1cc:rt:t8:t7 0x1d8:rt:t8:t7 0x1dc:rt:t9:t8 0x1e4:rt:t9:t8 0x1e8:rt:t0:v0 0x1ec:rs:t0:v0 0x1ec:rt:t0:v0 0x1f4:rs:t0:v0 0x1f8:rt:t1:t9 0x1fc:rs:t1:t9 0x1fc:rt:t1:t9 0x204:rs:t1:t9 0x204:rt:t2:t0 0x208:rt:t2:t0 0x2a4:rt:t3:t1 0x2ac:rt:t4:t2 0x2b0:rs:t4:t2 0x2b0:rt:t4:t2 0x2b4:rt:t3:t1 0x2bc:rt:t4:t2 0x2c4:rt:t5:t3 0x2c8:rt:t5:t3 0x2f4:rt:t6:t4 0x300:rt:t6:t4 0x344:rt:t7:t5 0x348:rs:t7:t5 0x360:rd:t8:t6 0x368:rt:t8:t6 0x36c:rt:t8:t6 0x374:rs:t8:t6 0x37c:rt:t9:t7 0x388:rt:t9:t7 0x38c:rt:t0:t8 0x394:rt:t0:t8 0x3a8:rt:t3:t1 0x3ac:rt:t1:t9 0x3b0:rs:t1:t9 0x3b0:rt:t1:t9 0x3b8:rs:t1:t9 0x3b8:rt:t2:t0 0x3bc:rt:t2:t0 0x3c0:rd:t4:t2 0x3c4:rd:t4:t2 0x3c4:rs:t4:t2 0x3c8:rd:t4:t2 0x3c8:rt:t4:t2 0x3cc:rd:t4:t2 0x3cc:rs:t4:t2 0x3d0:rs:t3:t1 0x3d0:rt:t3:t1 0x3d4:rd:t4:t2 0x3d4:rt:t4:t2 0x3d8:rs:t4:t2 0x3d8:rt:t5:t3 0x3dc:rs:t3:t1 0x3dc:rt:t5:t3 0x3f8:rt:t8:t6 0x3fc:rt:t6:t4 0x400:rs:t6:t4 0x400:rt:t6:t4 0x408:rs:t6:t4 0x408:rt:t7:t5 0x410:rt:t7:t5 0x414:rs:t8:t6 0x414:rt:t8:t6 0x41c:rs:t8:t6 0x41c:rt:t9:t7 0x420:rt:t9:t7 0x4cc:rt:t0:t8 0x4d8:rt:t0:t8 */
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
extern void bustup_ptr_init(void);
extern void fade_down_main(void);
extern void fade_in(void);
extern void fade_out_step(void);
extern void fade_out(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32  get_g_zyotai_low(s32);
extern void bustup_load_step(s32);
extern void bunkasai_basyo_init2(void);
extern void bunkasai_sub_reset(void);
extern void bunkasai_girl0_home_srf_bank_set(void);

extern s16 now_serifu;
extern u8  now_sub_sub_step;
extern u8  now_year;
extern u16 bukatu_pt5;
extern u8  now_girl;
extern u8  now_sub_step;
extern s32 bunka_sd_kind;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

void bunkasai_maind(void) {
    s32 v0;
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
            bustup_load_step(21578);
        }
        break;
    case 3:
        v0 = dec_bg_init(1);
        indexed_byte_lsb_set(1, v0);
        bunkasai_basyo_init2();
        bustup_ptr_init();
        now_girl = 0;
        zyotai_sprite_set_chk();
        cd_read_start2(24, 0x801E8000, 18896);
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
                    now_serifu = 153;
                    break;
                case 2:
                    now_serifu = 154;
                    break;
                case 3:
                    now_serifu = 155;
                    break;
                case 4:
                    now_serifu = 156;
                    break;
                }
            } else {
                v0 = get_g_zyotai_low(0);
                switch (v0) {
                case 0:
                case 1:
                    now_serifu = 158;
                    break;
                case 2:
                    now_serifu = 159;
                    break;
                case 3:
                    now_serifu = 159;
                    break;
                case 4:
                    now_serifu = 160;
                    break;
                }
                if (now_year == 97) {
                    now_serifu = now_serifu + 4;
                }
            }
            bunkasai_girl0_home_srf_bank_set();
        }
        break;
    case 6:
        serifu_run();
        break;
    case 7:
        if (get_g_zyotai_low(0) == 4) {
            fade_down_main();
        } else {
            goto_sub_sub_step(9);
        }
        break;
    case 8:
        serifu_run();
        break;
    case 9:
        if (get_g_zyotai_low(0) == 4) {
            fade_out_step();
        } else {
            fade_out();
        }
        break;
    case 10:
        bunka_sd_kind = 3;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_step(128);
        break;
    case 11:
        bustup_load_step(21578);
        break;
    case 12:
        bunkasai_basyo_init2();
        mes_base_y = 192;
        bustup_ptr_init();
        now_girl = 0;
        zyotai_sprite_set_chk();
        cd_read_start2(24, 0x801E8000, 18896);
        break;
    case 13:
        fade_in();
        break;
    case 14:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(0);
            switch (v0) {
            case 0:
            case 1:
            case 2:
            case 3:
                now_serifu = 166;
                break;
            case 4:
                now_serifu = 168;
                break;
            }
            b = now_year;
            if (b == 97) {
                now_serifu = now_serifu + 6;
            }
            if ((u32) bukatu_pt5 < (u32) (b * 100 - 9400)) {
                if (get_g_zyotai_low(0) == 4) {
                    now_serifu = now_serifu + 2;
                } else {
                    now_serifu = now_serifu + 1;
                }
            }
            bunkasai_girl0_home_srf_bank_set();
        }
        break;
    case 15:
        serifu_run();
        break;
    case 16:
        if (get_g_zyotai_low(0) == 4) {
            fade_down_main();
        } else {
            goto_sub_sub_step(18);
        }
        break;
    case 17:
        serifu_run();
        break;
    case 18:
        if (get_g_zyotai_low(0) == 4) {
            sub_sub_step_up();
        } else {
            fade_down_main();
        }
        break;
    case 19:
        fade_out_step();
        break;
    case 20:
        now_serifu = 1;
        sub_sub_step_up();
        break;
    case 21:
        serifu_run();
        break;
    case 22:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
