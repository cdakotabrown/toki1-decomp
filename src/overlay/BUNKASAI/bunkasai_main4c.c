/* @SymbolStart: 0x8013F230 */
/* @RodataBase: 0x8015D150 */
/* Address: 0x8013F238 */
/* Size: 920 bytes (230 insns) */
/* @SymbolSize: 928 */
/* @PostProc: patch_insn_reg_field 0x2e8:rd:t6:t7 */
/* @PostProc: patch_insn_reg_field 0x2e4:rt:t7:t8 0x2ec:rs:t6:t7 0x2f4:rs:t7:t8 0x2f4:rt:t7:t8 0x2f8:rt:t9:t0 0x2fc:rs:t9:t0 0x2fc:rt:t9:t1 0x300:rs:t7:t8 0x300:rt:t8:t9 0x304:rt:t8:t9 0x30c:rs:t9:t1 0x30c:rt:t0:t2 0x310:rt:t0:t2 */
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
extern void bunkasai_girl3_clubs4_5_srf_bank_set(void);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);

extern s16 now_serifu;
extern s16 girl3_syoushin;
extern u8  girl3_flag;
extern u8  now_girl;

extern u8 now_sub_sub_step;
void bunkasai_main4c(void) {
    s32 idx = now_sub_sub_step;

    s32 v0;

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
            bunkasai_basyo_init();
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            bustup_cd_read(22703);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            bustup_ptr_init();
            now_girl = 3;
            zyotai_sprite_set_chk();
            now_serifu = 45;
        }
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        cd_read_start2(15, 0x801E8000, 19181);
        sub_sub_step_up();
        break;
    case 7:
        fade_up_main();
        break;
    case 8:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(3);
            switch (v0) {

                break;
            case 0: case 1:
                now_serifu = 36;
                break;
            case 2:
                now_serifu = 37;
                break;
            case 3:
                now_serifu = 38;
                break;
            case 4:
                now_serifu = 39;
                break;
            }
            bunkasai_girl3_clubs4_5_srf_bank_set();
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        v0 = get_g_zyotai_low(3);
        if (v0 == 4) {
            goto_sub_sub_step(12);
        } else if ((girl3_syoushin & 3) == 0) {
            v0 = get_g_zyotai_low(3);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 24;
                break;
            case 2:
                now_serifu = 26;
                break;
            case 3:
                now_serifu = 28;
                break;
            }
            sub_sub_step_up();
        } else {
            v0 = get_g_zyotai_low(3);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 41;
                break;
            case 2:
                now_serifu = 43;
                break;
            case 3:
                now_serifu = 45;
                break;
            }
            if ((s32)(*(s32 *)&girl3_flag << 29) >= 0) {
                now_serifu = now_serifu + 6;
                girl3_flag |= 4;
            }
            sub_sub_step_up();
        }
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        fade_down_main();
        break;
    case 13:
        serifu_run();
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
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
