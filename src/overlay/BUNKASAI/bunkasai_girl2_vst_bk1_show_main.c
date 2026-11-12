/* @SymbolStart: 0x80132DB4 */
/* @RodataBase: 0x8015BA2C */
/* Address: 0x80132DBC */
/* Size: 1552 bytes (388 insns) */
/* @SymbolSize: 1560 */
/* @PostProc: patch_insn_reg_field 0x50c:rt:t7:t8 0x510:rd:t5:t6 0x514:rs:t5:t6 0x518:rt:t1:t2 0x51c:rt:t6:t7 0x524:rt:t6:t7 0x538:rs:t7:t8 0x538:rt:t7:t8 0x540:rd:t8:t9 0x540:rt:t7:t8 0x544:rs:t8:t9 0x548:rt:t0:t1 0x54c:rt:t9:t0 0x558:rt:t9:t0 0x560:rt:t0:t1 0x564:rs:t1:t2 0x564:rt:t1:t3 0x56c:rs:t1:t3 0x56c:rt:t2:t4 0x574:rt:t2:t4 0x590:rt:t3:t5 0x59c:rt:t3:t5 0x5b8:rt:t4:t6 0x5c4:rt:t4:t6 */
#include "types.h"

extern void sub_sub_step_up(void);
extern void goto_step(s32);
extern void goto_sub_sub_step(s32);
extern s32 get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void game_reset(void);
extern void dec_bg_reset(void);
extern void set_dec_bri(s32);
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
extern u8  now_year;
extern s16 girl2_syoushin;
extern u8  girl2_flag;
extern u8  girl2_basyo;
extern u8  now_sub_step;
extern u8  now_girl;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

extern u8 now_sub_sub_step;
void bunkasai_girl2_vst_bk1_show_main(void) {
    s32 v0;
    u8 loc;

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
        if (now_year == 95) {
            now_serifu = 39;
        } else {
            now_serifu = 40;
        }
        break;
    case 4:
        serifu_run();
        break;
    case 5:
        fade_out_step();
        break;
    case 6:
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_step(128);
        break;
    case 7:
        dec_bg_reset();
        set_dec_bri(0);
        sub_sub_step_up();
        break;
    case 8:
        bustup_load_step(21038);
        break;
    case 9:
        bustup_ptr_init();
        now_girl = 2;
        zyotai_sprite_set_chk();
        value_cache_find_or_load(16628, 1);
        break;
    case 10:
        if (get_CD_State2() & 1) {
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            bunkasai_basyo_init2();
            mes_base_y = 192;
            cd_read_start2(26, 0x801E8000, 18962);
            sub_sub_step_up();
        }
        break;
    case 11:
        if (get_CD_State2() & 1) {
            bunkasai_girl2_vst_bank_init();
            v0 = get_g_zyotai_low(2);
            if (v0 == 4) {
                now_serifu = 41;
                goto_sub_sub_step(21);
            }
        }
        break;
    case 12:
        fade_in();
        break;
    case 13:
        sub_sub_step_up();
        v0 = get_g_zyotai_low(2);
        switch (v0) {
        case 0:
        case 1:
            loc = now_year;
            switch (loc) {
            case 95:
                now_serifu = 35;
                break;
            case 96:
                now_serifu = 44;
                break;
            case 97:
                now_serifu = 50;
                break;
            }
            break;
        case 2:
            loc = now_year;
            switch (loc) {
            case 95:
                now_serifu = 37;
                break;
            case 96:
                now_serifu = 46;
                break;
            case 97:
                now_serifu = 52;
                break;
            }
            break;
        case 3:
            loc = now_year;
            switch (loc) {
            case 95:
                now_serifu = 39;
                break;
            case 96:
                now_serifu = 48;
                break;
            case 97:
                now_serifu = 54;
                goto_sub_sub_step(28);
                break;
            }
            break;
        }
        break;
    case 14:
        serifu_run();
        break;
    case 15:
        fade_down_main();
        break;
    case 16:
        serifu_run();
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
        bunkasai_sub_reset();
        break;
    case 21:
        fade_in_step();
        break;
    case 22:
        serifu_run();
        break;
    case 23:
        fade_up_main();
        break;
    case 24:
        serifu_run();
        break;
    case 25:
        fade_down_main();
        break;
    case 26:
        serifu_run();
        break;
    case 27:
        goto_sub_sub_step(17);
        break;
    case 28:
        serifu_run();
        break;
    case 29:
        if ((girl2_syoushin & 3) == 0) {
            now_serifu = 63;
            goto_sub_sub_step(36);
        } else {
            if (((u32)girl2_basyo >> 4) == 2) {
                now_serifu = 55;
            } else {
                now_serifu = 56;
            }
            sub_sub_step_up();
        }
        break;
    case 30:
        serifu_run();
        break;
    case 31:
        if ((s32)(*(s32 *)&girl2_flag << 29) < 0) {
            now_serifu = 59;
            goto_sub_sub_step(34);
        } else {
            if (((u32)girl2_basyo >> 4) == 2) {
                now_serifu = 57;
            } else {
                now_serifu = 58;
            }
            girl2_flag |= 4;
            sub_sub_step_up();
        }
        break;
    case 32:
        serifu_run();
        break;
    case 33:
        now_serifu = 60;
        sub_sub_step_up();
        break;
    case 34:
        serifu_run();
        break;
    case 35:
        now_serifu = 61;
        sub_sub_step_up();
        break;
    case 36:
        serifu_run();
        break;
    case 37:
        fade_down_main();
        break;
    case 38:
        serifu_run();
        break;
    case 39:
        goto_sub_sub_step(17);
        break;
    default:
        game_reset();
        break;
    }
}
