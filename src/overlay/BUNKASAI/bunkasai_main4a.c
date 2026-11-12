/* @SymbolStart: 0x8013EBD8 */
/* @RodataBase: 0x8015D094 */
/* Address: 0x8013EBE0 */
/* Size: 980 bytes (245 insns) */
/* @SymbolSize: 988 */
/* @PostProc: patch_insn_reg_field 0x324:rd:t9:t0 */
/* @PostProc: patch_insn_reg_field 0x320:rt:t0:t1 0x328:rs:t9:t0 0x330:rs:t0:t1 0x330:rt:t0:t1 0x334:rt:t2:t3 0x338:rs:t2:t3 0x338:rt:t2:t4 0x33c:rs:t0:t1 0x33c:rt:t1:t2 0x340:rt:t1:t2 0x348:rs:t2:t4 0x348:rt:t3:t5 0x34c:rt:t3:t5 */
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
extern u16 bukatu_pt4;
extern u8  now_girl;

extern u8 now_sub_sub_step;
void bunkasai_main4a(void) {
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
            now_serifu = 43;
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
                now_serifu = 1;
                break;
            case 2:
                now_serifu = 4;
                break;
            case 3:
                now_serifu = 7;
                break;
            case 4:
                now_serifu = 10;
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
            goto_sub_sub_step(14);
        } else {
            if ((u32)bukatu_pt4 < 100) {
                now_serifu = now_serifu + 1;
            }
            sub_sub_step_up();
        }
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        if ((girl3_syoushin & 3) == 0) {
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
        } else {
            v0 = get_g_zyotai_low(3);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 12;
                break;
            case 2:
                now_serifu = 14;
                break;
            case 3:
                now_serifu = 16;
                break;
            }
            if ((s32)(*(s32 *)&girl3_flag << 29) >= 0) {
                now_serifu = now_serifu + 6;
                girl3_flag |= 4;
            }
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
        fade_out_step();
        break;
    case 17:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 18:
        serifu_run();
        break;
    case 19:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
