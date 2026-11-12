/* @SymbolStart: 0x8013C634 */
/* Address: 0x8013C63C */
/* Size: 1072 bytes (268 insns) */
/* @SymbolSize: 1080 */
/* @RodataBase: 0x80153F54 */
/* @PostProc: patch_insn_reg_field 0xec:rt:t2:t3 0xf0:rd:t0:t1 0xf4:rs:t0:t1 0xfc:rt:t1:t2 0x108:rt:t1:t2 0x10c:rt:t2:t3 0x140:rt:t3:t4 0x148:rt:t4:t5 0x14c:rs:t4:t5 0x14c:rt:t4:t5 0x150:rt:t3:t4 0x158:rt:t4:t5 0x160:rt:t5:t6 0x164:rt:t5:t6 0x1a4:rt:t6:t7 0x1a8:rs:t6:t7 0x1c0:rt:t7:t8 0x1c4:rt:t1:t4 0x1c8:rd:t8:t0 0x1c8:rt:t7:t8 0x1cc:rs:t8:t0 0x1e4:rt:t9:t2 0x1ec:rs:t9:t2 0x1ec:rt:t0:t3 0x1f0:rt:t0:t3 0x1f4:rt:t1:t4 0x21c:rt:t2:t5 0x228:rt:t2:t5 0x24c:rt:t3:t6 0x250:rs:t3:t6 0x28c:rt:t4:t7 0x290:rs:t4:t7 0x2a8:rt:t5:t8 0x2b4:rt:t5:t8 0x2d0:rt:t6:t9 0x2d4:rs:t6:t9 0x2d4:rt:t6:t9 0x2dc:rs:t6:t9 0x314:rd:t7:t0 0x31c:rt:t7:t0 0x320:rt:t7:t0 0x328:rs:t7:t0 0x330:rt:t8:t1 0x33c:rt:t8:t1 0x340:rt:t9:t2 0x34c:rt:t9:t2 0x350:rt:t0:t3 0x35c:rt:t0:t3 0x360:rt:t1:t4 0x368:rt:t1:t4 0x36c:rt:t2:t5 0x370:rs:t2:t5 0x370:rt:t2:t5 0x378:rs:t2:t5 0x37c:rt:t3:t6 0x384:rt:t3:t6 0x3b8:rt:t4:t7 0x3bc:rs:t4:t7 0x3bc:rt:t4:t7 0x3c4:rs:t4:t7 */
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
extern void bustup_cd_read(s32);
extern void bustup_ptr_init(void);
extern void fade_down_main(void);
extern void fade_in_step(void);
extern void fade_in(void);
extern void fade_out_step(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32 get_g_zyotai_low(s32);
extern void bunkaken_girl1_cl_bank_init(void);
extern void bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);
extern void bunkaken_basyo_init(void);

extern s16 now_serifu;
extern s32 girl1_flag;
extern u8  now_sub_step;
extern u8  now_girl;
extern s32 bunka_girl2;
extern s32 bunka_sd_kind;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern u8 now_sub_sub_step;
extern u8 screen_w[];

void bunkaken_main2e(void) {
    s32 v0;
    u8 *p;

    switch (now_sub_sub_step) {
    case 0:
        value_cache_find_or_load(18044, 1);
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
            cd_read_start2(7, 0x801E8000, 19126);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            v0 = girl1_flag;
            if ((s32)(v0 << 29) >= 0) goto L4_set26;
            now_serifu = 25;
            goto L4_after;
L4_set26:
            now_serifu = 26;
L4_after:
            bunkaken_girl1_cl_bank_init();
        }
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        fade_out_step();
        break;
    case 7:
        bunka_sd_kind = 3;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_step(128);
        break;
    case 8:
        cd_read_start2(7, 0x801E8000, 19126);
        sub_sub_step_up();
        break;
    case 9:
        if (get_CD_State2() & 1) {
            bunkaken_girl1_cl_bank_init();
            p = (u8 *)((u32)screen_w | 0);
            if ((s32)(*(s32 *)(p + 512) << 29) >= 0) goto L9_set;
            goto_sub_sub_step(11);
            goto L9_call;
L9_set:
            p[512] |= 4;
            now_serifu = 4;
L9_call:
            value_cache_find_or_load(16628, 1);
        }
        break;
    case 10:
        serifu_run();
        break;
    case 11:
        now_serifu = 6;
        sub_sub_step_up();
        break;
    case 12:
        serifu_run();
        break;
    case 13:
        if (get_CD_State2() & 1) {
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            bustup_cd_read(23873);
            sub_sub_step_up();
        }
        break;
    case 14:
        if (get_CD_State2() & 1) {
            bunkaken_basyo_init();
            bustup_ptr_init();
            now_girl = 1;
            zyotai_sprite_set_chk();
        }
        break;
    case 15:
        v0 = get_g_zyotai_low(1);
        if (v0 == 4) goto L15_then;
        if (bunka_girl2 != 1) goto L15_else;
L15_then:
        fade_in_step();
        break;
L15_else:
        fade_in();
        break;
    case 16:
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
        case 4:
            now_serifu = 30;
            break;
        }
        if (bunka_girl2 == 1) {
            now_serifu = 30;
        }
        sub_sub_step_up();
        break;
    case 17:
        serifu_run();
        break;
    case 18:
        v0 = get_g_zyotai_low(1);
        if (v0 == 4) goto L18_then;
        if (bunka_girl2 != 1) goto L18_else;
L18_then:
        sub_sub_step_up();
        break;
L18_else:
        fade_down_main();
        break;
    case 19:
        fade_out_step();
        break;
    case 20:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 21:
        serifu_run();
        break;
    case 22:
        bunkaken_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
