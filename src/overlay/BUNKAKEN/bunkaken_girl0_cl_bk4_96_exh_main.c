/* @SymbolStart: 0x8014D69C */
/* @SymbolSize: 1240 */
/* @RodataBase: 0x80155634 */
/* @PostProc: swap_instructions --offset1 0x380 --offset2 0x384 */
/* @PostProc: swap_instructions --offset1 0x388 --offset2 0x38c */
/* @PostProc: patch_insn_reg_field 0x218:rt:t6:v1 0x22c:rs:t6:v1 0x22c:rt:t6:v1 0x230:rs:t6:v1 0x230:rt:t7:t6 0x238:rt:t7:t6 0x254:rt:t0:t1 0x258:rt:t2:t3 0x25c:rs:t0:t1 0x25c:rt:t1:t2 0x260:rt:t1:t2 0x268:rt:t2:t3 0x2dc:rt:t4:t5 0x2e8:rt:t5:t6 0x2f4:rt:t3:t4 0x300:rt:t3:t4 0x30c:rt:t4:t5 0x314:rt:t5:t6 0x348:rt:t6:t7 0x354:rt:t6:t7 0x3dc:rt:t9:t1 0x3e0:rs:t9:t1 0x3e0:rt:t9:t1 0x3e4:rt:t1:t3 0x3e8:rs:t9:t1 0x3f0:rt:t0:t2 0x3fc:rt:t0:t2 0x400:rt:t1:t3 0x434:rt:t2:t4 0x438:rs:t2:t4 0x438:rt:t2:t4 0x440:rs:t2:t4 0x460:rt:t3:v1 0x464:rs:t3:v1 0x464:rt:t3:v1 0x468:rs:t3:v1 0x468:rt:t4:t5 0x46c:rt:t7:t9 0x470:rd:t5:t7 0x470:rt:t4:t5 0x474:rs:t5:t7 0x47c:rt:t6:t8 0x488:rt:t6:t8 0x48c:rt:t7:t9 0x380:rt:v0:v1 0x388:rs:v0:v1 0x388:rt:v0:v1 0x390:rs:v0:v1 0x390:rt:t7:t9 0x398:rs:t7:t9 0x398:rt:t8:t0 0x3a0:rs:v0:v1 0x3a0:rt:t8:t0 */
/* Address: 0x8014D6A4 */
/* Size: 1232 bytes (308 insns) */
#include "types.h"

extern u8 screen_w[];

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
extern void menu2_main(void);
extern void zyotai_sprite_set_chk(void);
extern void date_name_set(s32);
extern void serifu_run(void);
extern s32  get_g_zyotai_low(s32);
extern void bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);
extern void bunkaken_param_set2(void);

extern s16 now_serifu;
extern u8  girl0_flag;
extern u8  now_girl;
extern u8  now_sub_sub_step;
extern s32 serifu_no;

void bunkaken_girl0_cl_bk4_96_exh_main(void)
{
    s32 idx = now_sub_sub_step;
    s32 v0;
    s32 ret;
    u8 *p;

    switch (idx) {
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
            ret = dec_bg_init(1);
            indexed_byte_lsb_set(1, ret);
            cd_read_start2(23, 0x801E8000, 18923);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            bunkaken_param_set2();
            v0 = get_g_zyotai_low(0);
            if (v0 == 4) {
                bustup_cd_read(27743);
            } else {
                bustup_cd_read(21668);
            }
        }
        break;
    case 5:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(0);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 134;
                break;
            case 2:
                now_serifu = 135;
                break;
            case 3:
                now_serifu = 136;
                break;
            }
            bustup_ptr_init();
            v0 = get_g_zyotai_low(0);
            if (v0 == 4) {
                now_serifu = 137;
                now_girl = 13;
                date_name_set(13);
            } else {
                now_girl = 0;
            }
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
        v0 = get_g_zyotai_low(0);
        if (v0 == 4) {
            now_serifu = 140;
        } else {
            p = (u8 *)((u32)screen_w | 0);
            if ((s32)(*(s32 *)(p + 456) << 29) >= 0) goto L8_set;
            now_serifu = 138;
            goto L8_call;
L8_set:
            girl0_flag |= 4;
            now_serifu = 139;
        }
L8_call:
        sub_sub_step_up();
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        v0 = get_g_zyotai_low(0);
        if (v0 == 4) {
            goto_sub_sub_step(20);
        } else {
            sub_sub_step_up();
        }
        break;
    case 11:
        v0 = get_g_zyotai_low(0);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 141;
            break;
        case 2:
            now_serifu = 142;
            break;
        case 3:
            now_serifu = 143;
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
        now_serifu = 144;
        sub_sub_step_up();
        break;
    case 15:
        serifu_run();
        break;
    case 16:
        fade_out_step();
        break;
    case 17:
        p = (u8 *)((u32)screen_w | 0);
        now_serifu = 0;
        p[456] |= 4;
        sub_sub_step_up();
        break;
    case 18:
        serifu_run();
        break;
    case 19:
        bunkaken_sub_reset();
        break;
    case 20:
        menu2_main();
        break;
    case 21:
        if (serifu_no != 0) {
            now_serifu = 148;
        } else {
            now_serifu = 145;
        }
        sub_sub_step_up();
        break;
    case 22:
        serifu_run();
        break;
    case 23:
        fade_down_main();
        break;
    case 24:
        if (serifu_no != 0) {
            goto_sub_sub_step(16);
        } else {
            date_name_set(0);
            p = (u8 *)((u32)screen_w | 0);
            if ((s32)(*(s32 *)(p + 456) << 29) >= 0) goto L24_set;
            now_serifu = 146;
            goto L24_call;
L24_set:
            now_serifu = 147;
L24_call:
            sub_sub_step_up();
        }
        break;
    case 25:
        serifu_run();
        break;
    case 26:
        fade_out_step();
        break;
    case 27:
        goto_sub_sub_step(17);
        break;
    default:
        game_reset();
        break;
    }
}
