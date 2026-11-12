/* @SymbolStart: 0x80142FE4 */
/* @SymbolSize: 1216 */
/* @RodataBase: 0x80154840 */
/* @PostProc: swap_instructions --offset1 0x3a8 --offset2 0x3ac */
/* @PostProc: swap_instructions --offset1 0x3b0 --offset2 0x3b4 */
/* @PostProc: patch_insn_reg_field 0x248:rt:t0:v0 0x25c:rs:t0:v0 0x25c:rt:t0:v0 0x260:rs:t0:v0 0x260:rt:t1:t0 0x268:rt:t1:t0 0x284:rt:t4:t5 0x288:rt:t6:t7 0x28c:rs:t4:t5 0x28c:rt:t5:t6 0x290:rt:t5:t6 0x298:rt:t6:t7 0x2cc:rt:t7:t8 0x2d0:rs:t7:t8 0x2d0:rt:t7:t8 0x2d8:rs:t7:t8 0x31c:rt:t9:t0 0x328:rt:t0:t1 0x334:rt:t8:t9 0x340:rt:t8:t9 0x34c:rt:t9:t0 0x354:rt:t0:t1 0x3b8:rt:t1:t3 0x3c0:rs:t1:t3 0x3c0:rt:t2:t4 0x3c8:rt:t2:t4 0x404:rt:t3:t5 0x408:rs:t3:t5 0x408:rt:t3:t5 0x40c:rt:t5:t7 0x410:rs:t3:t5 0x418:rt:t4:t6 0x424:rt:t4:t6 0x428:rt:t5:t7 0x45c:rt:t6:t8 0x460:rs:t6:t8 0x460:rt:t6:t8 0x468:rs:t6:t8 */
/* Address: 0x80142FEC (window starts at the absorbed cross-prologue head */
/* Size: 1216 bytes (304 insns incl. head) */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern s32 goto_sub_sub_step(s32);
extern s32 get_CD_State2(void);
extern s32 cd_read_start2(s32, s32, s32);
extern s32 game_reset(void);
extern s32 indexed_byte_lsb_set(s32, s32);
extern s32 value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);
extern s32 bustup_cd_read(s32);
extern s32 bustup_ptr_init(void);
extern s32 fade_up_main(void);
extern s32 fade_down_main(void);
extern s32 fade_in_step(void);
extern s32 fade_out_step(void);
extern s32 menu2_main(void);
extern s32 zyotai_sprite_set_chk(void);
extern s32 date_name_set(s32);
extern s32 serifu_run(void);
extern s32 get_g_zyotai_low(s32);
extern s32 bunkaken_girl3_cl_bank_init(void);
extern s32 bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);

extern u16 now_serifu;
extern u8  screen_w[];
extern u8  girl3_flag;
extern u8  now_girl;
extern u8  now_sub_sub_step;
extern s32 serifu_no;
extern s32 bunka_girl2;

void bunkaken_girl3_cl_bk4_96_exh_main(void) {
    s32 v0;
    u8 *base;

    switch ((s32)now_sub_sub_step) {
    case 0:
        value_cache_find_or_load(16664, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out_step();
        break;
    case 2:
        if (get_CD_State2() & 1) {
            bunkaken_basyo_init3();
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            cd_read_start2(9, 0x801E8000, 19210);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(3);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 32;
                break;
            case 2:
                now_serifu = 33;
                break;
            case 3:
                now_serifu = 34;
                break;
            }
            v0 = get_g_zyotai_low(3);
            if (v0 == 4 || bunka_girl2 == 3) {
                now_serifu = 35;
                now_girl = 13;
            } else {
                now_girl = 3;
            }
            bunkaken_girl3_cl_bank_init();
            v0 = get_g_zyotai_low(3);
            if (v0 == 4 || bunka_girl2 == 3) {
                date_name_set(13);
                bustup_cd_read(27743);
            } else {
                bustup_cd_read(22748);
            }
        }
        break;
    case 5:
        if (get_CD_State2() & 1) {
            bustup_ptr_init();
            zyotai_sprite_set_chk();
        }
        break;
    case 6:
        serifu_run();
        break;
    case 7:
        fade_up_main();
        break;
    case 8:
        v0 = get_g_zyotai_low(3);
        if (v0 == 4 || bunka_girl2 == 3) {
            now_serifu = 44;
        } else {
            base = (u8 *)((u32)screen_w | 0);
            if ((*(s32 *)(base + 624) << 29) < 0) {
                now_serifu = 36;
            } else {
                girl3_flag |= 4;
                now_serifu = 37;
            }
        }
        sub_sub_step_up();
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        v0 = get_g_zyotai_low(3);
        if (v0 == 4 || bunka_girl2 == 3) {
            goto_sub_sub_step(19);
        } else {
            sub_sub_step_up();
        }
        break;
    case 11:
        v0 = get_g_zyotai_low(3);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 38;
            break;
        case 2:
            now_serifu = 40;
            break;
        case 3:
            now_serifu = 42;
            break;
        }
        sub_sub_step_up();
        break;
    case 12:
        serifu_run();
        break;
    case 13:
        fade_down_main();
        break;
    case 14:
        serifu_run();
        break;
    case 15:
        fade_out_step();
        break;
    case 16:
        base = (u8 *)((u32)screen_w | 0);
        now_serifu = 0;
        base[624] |= 4;
        sub_sub_step_up();
        break;
    case 17:
        serifu_run();
        break;
    case 18:
        bunkaken_sub_reset();
        break;
    case 19:
        menu2_main();
        break;
    case 20:
        if (serifu_no != 0) {
            now_serifu = 47;
        } else {
            now_serifu = 45;
        }
        sub_sub_step_up();
        break;
    case 21:
        serifu_run();
        break;
    case 22:
        fade_down_main();
        break;
    case 23:
        if (serifu_no != 0) {
            goto_sub_sub_step(15);
        } else {
            date_name_set(3);
            sub_sub_step_up();
        }
        break;
    case 24:
        serifu_run();
        break;
    case 25:
        goto_sub_sub_step(15);
        break;
    default:
        game_reset();
        break;
    }
}
