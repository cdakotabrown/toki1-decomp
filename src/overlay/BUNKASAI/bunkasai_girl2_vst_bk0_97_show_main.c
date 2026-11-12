/* @SymbolStart: 0x801328D4 */
/* @RodataBase: 0x8015B97C */
/* Address: 0x801328DC */
/* Size: 1240 bytes (310 insns) */
/* @SymbolSize: 1248 */
/* @PostProc: patch_insn_reg_field 0x348:rt:t9:t0 0x34c:rd:t7:t8 0x350:rs:t7:t8 0x354:rt:t3:t4 0x358:rt:t8:t9 0x360:rt:t8:t9 0x374:rs:t9:t0 0x374:rt:t9:t0 0x37c:rd:t0:t1 0x37c:rt:t9:t0 0x380:rs:t0:t1 0x384:rt:t2:t3 0x388:rt:t1:t2 0x394:rt:t1:t2 0x39c:rt:t2:t3 0x3a0:rs:t3:t4 0x3a0:rt:t3:t5 0x3a8:rs:t3:t5 0x3a8:rt:t4:t6 0x3b0:rt:t4:t6 0x3cc:rt:t5:t7 0x3d8:rt:t5:t7 0x3f4:rt:t6:t8 0x400:rt:t6:t8 */
#include "types.h"

extern void sub_sub_step_up(void);
extern void goto_step(s32);
extern void goto_sub_sub_step(s32);
extern s32 get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void game_reset(void);
extern void indexed_byte_lsb_set(s32, s32);
extern void value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);
extern void bustup_ptr_init(void);
extern void fade_up_main(void);
extern void fade_down_main(void);
extern void fade_in_step(void);
extern void fade_in(void);
extern void fade_out_step(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32 get_g_zyotai_low(s32);
extern void bustup_load_step(s32);
extern void bunkasai_girl2_vst_bank_init(void);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init2(void);

extern s16 now_serifu;
extern s16 girl2_syoushin;
extern u8  girl2_flag;
extern u8  girl2_basyo;
extern u8  now_sub_step;
extern u8  now_girl;
extern s32 bunka_sd_kind;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

extern u8 now_sub_sub_step;
void bunkasai_girl2_vst_bk0_97_show_main(void) {
    s32 v0;

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
            bunkasai_basyo_init2();
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        now_serifu = 38;
        sub_sub_step_up();
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        fade_out_step();
        break;
    case 7:
        bunka_sd_kind = 0;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_step(128);
        break;
    case 8:
        bustup_load_step(21038);
        break;
    case 9:
        bunkasai_basyo_init2();
        mes_base_y = 192;
        bustup_ptr_init();
        now_girl = 2;
        zyotai_sprite_set_chk();
        cd_read_start2(26, 0x801E8000, 18962);
        break;
    case 10:
        v0 = get_g_zyotai_low(2);
        if (v0 == 4) {
            fade_in_step();
        } else {
            fade_in();
        }
        break;
    case 11:
        if (get_CD_State2() & 1) {
            bunkasai_girl2_vst_bank_init();
            v0 = get_g_zyotai_low(2);
            switch (v0) {
            case 0:
            case 1:
            case 2:
                break;
            case 3:
                goto_sub_sub_step(17);
                break;
            case 4:
                now_serifu = 30;
                goto_sub_sub_step(30);
                break;
            }
        }
        break;
    case 12:
        v0 = get_g_zyotai_low(2);
        if ((u32)v0 < 2) {
            now_serifu = 17;
        } else {
            now_serifu = 19;
        }
        sub_sub_step_up();
        break;
    case 13:
        serifu_run();
        break;
    case 14:
        fade_down_main();
        break;
    case 15:
        serifu_run();
        break;
    case 16:
        goto_sub_sub_step(35);
        break;
    case 17:
        now_serifu = 21;
        sub_sub_step_up();

    case 18:
        serifu_run();
        break;
    case 19:
        if ((girl2_syoushin & 3) == 0) {
            now_serifu = 33;
            goto_sub_sub_step(26);
            break;
        }
        if (((u32)girl2_basyo >> 4) == 2) {
            now_serifu = 22;
        } else {
            now_serifu = 23;
        }
        sub_sub_step_up();
        break;
    case 20:
        serifu_run();
        break;
    case 21:
        if ((s32)(*(s32 *)&girl2_flag << 29) >= 0) goto L21_clear;
        now_serifu = 26;
        goto_sub_sub_step(24);
        break;
L21_clear:
        if (((u32)girl2_basyo >> 4) == 2) {
            now_serifu = 24;
        } else {
            now_serifu = 25;
        }
        girl2_flag |= 4;
        sub_sub_step_up();
        break;
    case 22:
        serifu_run();
        break;
    case 23:
        now_serifu = 27;
        sub_sub_step_up();
        break;
    case 24:
        serifu_run();
        break;
    case 25:
        now_serifu = 28;
        sub_sub_step_up();

    case 26:
        serifu_run();
        break;
    case 27:
        fade_down_main();
        break;
    case 28:
        serifu_run();
        break;
    case 29:
        goto_sub_sub_step(35);
        break;
    case 30:
        serifu_run();
        break;
    case 31:
        fade_up_main();
        break;
    case 32:
        serifu_run();
        break;
    case 33:
        fade_down_main();
        break;
    case 34:
        serifu_run();
        break;
    case 35:
        fade_out_step();
        break;
    case 36:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 37:
        serifu_run();
        break;
    case 38:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
