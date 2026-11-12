/* @SymbolStart: 0x801548C8 */
/* Address: 0x801548D0 */
/* Size: 1172 bytes (293 insns) */
/* @SymbolSize: 1180 */
/* @RodataBase: 0x8015F7D0 */
/* @PostProc: patch_insn_reg_field 0x32c:rt:t3:t4 0x330:rd:t2:t3 0x334:rs:t2:t3 0x33c:rs:t3:t4 0x33c:rt:t3:t4 0x344:rs:t3:t4 0x344:rt:t4:t5 0x348:rt:t4:t5 0x36c:rt:t5:t6 0x370:rs:t5:t6 0x370:rt:t5:t6 0x378:rs:t5:t6 0x378:rt:t6:t7 0x37c:rs:t6:t7 0x394:rd:t7:t8 0x39c:rt:t7:t8 0x3a0:rt:t7:t8 0x3a8:rs:t7:t8 0x3b0:rt:t8:t9 0x3bc:rt:t8:t9 0x3c0:rt:t9:t0 0x3cc:rt:t9:t0 0x3d0:rt:t0:t1 0x3dc:rt:t0:t1 0x3e8:rt:t1:t3 0x3f0:rs:t1:t3 0x3f0:rt:t2:t4 0x3f4:rt:t2:t4 0x418:rt:t3:t5 0x41c:rs:t3:t5 0x41c:rt:t3:t5 0x424:rs:t3:t5 0x424:rt:t4:t6 0x428:rs:t4:t6 */
#include "types.h"

extern u8 screen_w[];

extern void sub_sub_step_up(void);
extern void goto_step(s32);
extern s32 get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void game_reset(void);
extern void indexed_byte_lsb_set(s32, s32);
extern void value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);
extern void bustup_ptr_init(void);
extern void fade_down_main(void);
extern void fade_in_step(void);
extern void fade_in(void);
extern void fade_out_step(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32 get_g_zyotai_low(s32);
extern void bustup_load_step(s32);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init2(void);
extern void bunkasai_girl1_clubs0_2_srf_bank_set(void);

extern s16 now_serifu;
extern s16 girl1_syoushin;
extern s32 girl1_flag;
extern u8  now_sub_step;
extern u8  now_girl;
extern s32 bunka_sd_kind;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

extern u8 now_sub_sub_step;
void bunkasai_main1e(void) {
    s32 v0;
    u8 *p;

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
        bustup_load_step(23873);
        break;
    case 9:
        bunkasai_basyo_init2();
        mes_base_y = 192;
        bustup_ptr_init();
        now_girl = 1;
        zyotai_sprite_set_chk();
        cd_read_start2(20, 0x801E8000, 19075);
        break;
    case 10:
        fade_in();
        break;
    case 11:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(1);
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
                now_serifu = 4;
                break;
            }
            bunkasai_girl1_clubs0_2_srf_bank_set();
        }
        break;
    case 12:
        serifu_run();
        break;
    case 13:
        v0 = get_g_zyotai_low(1);
        switch (v0) {

            break;
        case 0: case 1: case 2: case 3:
            now_serifu = 7;
            break;
        case 4:
            now_serifu = 8;
            break;

            break;

            break;
        }
        sub_sub_step_up();
        break;
    case 14:
        serifu_run();
        break;
    case 15:
        v0 = get_g_zyotai_low(1);
        switch (v0) {

            break;
        case 0: case 1:
            now_serifu = 41;
            break;
        case 2:
            now_serifu = 42;
            break;
        case 3:
            now_serifu = 43;
            break;
        case 4:
            now_serifu = 44;
            break;
        }
        if ((girl1_syoushin & 3) == 0) {
            now_serifu = now_serifu + 8;
        } else if ((s32)(girl1_flag << 29) >= 0) {
            now_serifu = now_serifu + 4;
        }
        sub_sub_step_up();
        break;
    case 16:
        serifu_run();
        break;
    case 17:
        if ((girl1_syoushin & 3) != 0) goto L17_set;
        v0 = get_g_zyotai_low(1);
        switch (v0) {

            break;
        case 0: case 1:
            now_serifu = 13;
            break;
        case 2: case 3:
            now_serifu = 12;
            break;
        case 4:
            now_serifu = 14;
            break;

            break;
        }
        goto L17_call;
L17_set:
        p = (u8 *)((u32)screen_w | 0);
        p[512] |= 4;
L17_call:
        sub_sub_step_up();
        break;
    case 18:
        fade_down_main();
        break;
    case 19:
        if ((girl1_syoushin & 3) == 0) {
            serifu_run();
        } else {
            sub_sub_step_up();
        }
        break;
    case 20:
        fade_out_step();
        break;
    case 21:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 22:
        serifu_run();
        break;
    case 23:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
