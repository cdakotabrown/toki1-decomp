/* @RodataBase: 0x8015F758 */
/* @SymbolStart: 0x801541D8 */
/* @SymbolSize: 1776 */
/* @PostProc: patch_insn_reg_field 0x250:rd:t5:t6 0x254:rs:t5:t6 0x31c:rt:t6:t7 0x374:rs:t6:t7 0x374:rt:t6:t7 0x37c:rs:t6:t7 0x380:rt:t8:t9 0x388:rt:t8:t9 0x3ac:rt:t9:t0 0x3b0:rs:t9:t0 0x3b0:rt:t9:t0 0x3b8:rs:t9:t0 0x3b8:rt:t0:t1 0x3bc:rs:t0:t1 0x498:rt:t1:t2 0x49c:rs:t1:t2 0x49c:rt:t1:t2 0x4a4:rs:t1:t2 0x4a4:rt:t2:t3 0x4a8:rs:t2:t3 0x56c:rt:t3:t4 0x578:rd:t4:t6 0x578:rt:t3:t4 0x57c:rs:t4:t6 0x584:rt:t5:t8 0x588:rt:t7:t0 0x58c:rs:t5:t8 0x58c:rt:t6:t9 0x590:rt:t6:t9 0x594:rs:t7:t0 0x594:rt:t7:t0 0x59c:rs:t7:t0 0x5a0:rt:t1:t4 0x5a4:rt:t9:t2 0x5b0:rt:t9:t2 0x5b8:rt:t1:t4 0x5cc:rt:t2:t5 0x5d0:rs:t2:t5 0x5d0:rt:t2:t5 0x5d8:rs:t2:t5 0x5d8:rt:t3:t6 0x5dc:rs:t3:t6 0x604:rt:t4:t7 0x608:rs:t4:t7 0x608:rt:t4:t7 0x610:rs:t4:t7 0x610:rt:t5:t8 0x614:rs:t5:t8 0x694:rt:t6:t9 0x698:rs:t6:t9 0x698:rt:t6:t9 0x6a0:rs:t6:t9 0x6b0:rt:t7:t1 0x6b8:rs:t7:t1 0x6b8:rt:t8:t2 0x6c0:rt:t8:t2 0x6c8:rt:t9:t4 0x6d0:rs:t9:t4 0x6d0:rt:t0:t5 0x6d4:rt:t0:t5 */
/* Address: 0x801541D8 (absorbed head) / 0x801541E0 (prologue) */
/* Size: 1776 bytes (444 insns) incl. the absorbed 2-word head */
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
extern void fade_up_main(void);
extern void fade_down_main(void);
extern void fade_in_step(void);
extern void fade_out_step(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32  get_g_zyotai_low(s32);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);
extern void bunkasai_girl1_clubs0_2_srf_bank_set(void);

extern s16 now_serifu;
extern u8  screen_w[];
extern s16 girl1_syoushin;
extern u8  now_year;
extern u8  now_girl;
extern u8  now_sub_sub_step;

void bunkasai_main1d(void) {
    s32 idx;
    s32 v0;
    s32 h;
    u8 *p;

    p = screen_w;
    idx = now_sub_sub_step;
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
            bustup_cd_read(23873);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            bustup_ptr_init();
            now_girl = 1;
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
        cd_read_start2(20, 0x801E8000, 19075);
        sub_sub_step_up();
        break;
    case 7:
        fade_up_main();
        break;
    case 8:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(1);
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
            case 4:
                if (now_year == 95) {
                    now_serifu = 39;
                } else {
                    now_serifu = 40;
                }
                break;
            }
            bunkasai_girl1_clubs0_2_srf_bank_set();
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        if (get_g_zyotai_low(1) == 4) {
            goto_sub_sub_step(20);
        } else if ((*(s32 *) &screen_w[512] << 29) < 0) {
            now_serifu = 11;
            sub_sub_step_up();
        } else {
            now_serifu = 6;
            goto_sub_sub_step(13);
        }
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        v0 = get_g_zyotai_low(1);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 15;
            break;
        case 2:
            now_serifu = 16;
            break;
        case 3:
            now_serifu = 17;
            break;
        }
        sub_sub_step_up();
        break;
    case 13:
        serifu_run();
        break;
    case 14:
        v0 = get_g_zyotai_low(1);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 18;
            break;
        case 2:
            now_serifu = 19;
            break;
        case 3:
            now_serifu = 20;
            break;
        }
        h = now_serifu;
        if (now_year == 96) {
            now_serifu = h + 3;
        }
        sub_sub_step_up();
        break;
    case 15:
        serifu_run();
        break;
    case 16:
        if ((girl1_syoushin & 3) == 0) {
            v0 = get_g_zyotai_low(1);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 36;
                break;
            case 2:
                now_serifu = 37;
                break;
            case 3:
                now_serifu = 38;
                break;
            }
        } else {
            v0 = get_g_zyotai_low(1);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 24;
                break;
            case 2:
                now_serifu = 25;
                break;
            case 3:
                now_serifu = 26;
                break;
            }
        }
        sub_sub_step_up();
        break;
    case 17:
        serifu_run();
        break;
    case 18:
        if ((girl1_syoushin & 3) == 0) {
            v0 = get_g_zyotai_low(1);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 13;
                break;
            case 2:
                now_serifu = 12;
                break;
            case 3:
                now_serifu = 12;
                break;
            }
        } else {
            v0 = get_g_zyotai_low(1);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 27;
                break;
            case 2:
                now_serifu = 28;
                break;
            case 3:
                now_serifu = 29;
                break;
            }
            if ((*(s32 *) (p + 512) << 29) >= 0) {
                p[512] |= 4;
                if (now_year == 95) {
                    now_serifu = now_serifu + 3;
                } else {
                    now_serifu = now_serifu + 6;
                }
            }
        }
        sub_sub_step_up();
        break;
    case 19:
        if ((girl1_syoushin & 3) == 0) {
            fade_down_main();
        } else {
            serifu_run();
        }
        break;
    case 20:
        if ((girl1_syoushin & 3) == 0 && (u32) get_g_zyotai_low(1) < 4U) {
            serifu_run();
        } else {
            fade_down_main();
        }
        break;
    case 21:
        fade_out_step();
        break;
    case 22:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 23:
        serifu_run();
        break;
    case 24:
        if (get_g_zyotai_low(1) != 4) {
            if (now_year == 95) {
                screen_w[513] |= 0x10;
            } else {
                screen_w[513] |= 0x20;
            }
        }
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
