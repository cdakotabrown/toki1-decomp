/* @SymbolStart: 0x8013D8E0 */
/* @SymbolSize: 1372 */
/* @RodataBase: 0x8015CE80 */
/* @PostProc: swap_instructions --offset1 0x338 --offset2 0x33c */
/* @PostProc: patch_insn_reg_field 0xd8:rt:t9:v0 0xe4:rt:t9:v0 0xe8:rt:t0:t9 0xf4:rt:t0:t9 0x14c:rt:t1:t0 0x150:rs:t1:t0 0x168:rd:t2:t1 0x170:rt:t2:t1 0x174:rt:t2:t1 0x17c:rs:t2:t1 0x184:rt:t3:v0 0x190:rt:t3:v0 0x194:rt:t4:v0 0x1a0:rt:t4:v0 0x1a4:rt:t5:v0 0x1b0:rt:t5:v0 0x1b4:rt:t6:v0 0x1bc:rt:t6:v0 0x22c:rt:t7:t2 0x230:rs:t7:t2 0x230:rt:t7:t2 0x234:rt:t8:t3 0x238:rs:t8:t3 0x238:rt:t8:t3 0x240:rt:t7:t2 0x248:rs:t8:t3 0x248:rt:t9:t4 0x250:rt:t9:t4 0x2bc:rt:t0:t5 0x2c0:rs:t0:t5 0x2d8:rd:t1:t6 0x2e0:rt:t1:t6 0x2e4:rt:t1:t6 0x2ec:rs:t1:t6 0x2f4:rt:t2:v0 0x300:rt:t2:v0 0x304:rt:t3:v0 0x310:rt:t3:v0 0x314:rt:t4:v0 0x320:rt:t4:v0 0x324:rt:t5:v0 0x32c:rt:t5:v0 0x330:rt:t6:v0 0x338:rs:t6:v0 0x338:rt:t6:v0 0x344:rd:t8:t9 0x344:rs:t6:v0 0x34c:rt:t8:t9 0x378:rt:t9:t0 0x38c:rs:t9:t0 0x38c:rt:t9:t0 0x394:rs:t9:t0 0x394:rt:t0:t1 0x398:rs:t0:t1 0x40c:rt:t1:t2 0x44c:rs:t1:t2 0x44c:rt:t1:t2 0x450:rt:t3:t5 0x454:rd:t2:t4 0x454:rt:t1:t2 0x458:rs:t2:t4 0x45c:rt:t7:t0 0x460:rs:t3:t5 0x460:rt:t3:t6 0x46c:rd:t5:t8 0x470:rs:t3:t6 0x470:rt:t4:t7 0x474:rt:t4:t7 0x478:rt:t5:t8 0x47c:rs:t7:t0 0x47c:rt:t7:t0 0x48c:rd:t8:t1 0x48c:rt:t7:t0 0x490:rs:t8:t1 0x494:rt:t9:t3 0x49c:rt:t9:t3 0x4c8:rt:t0:t4 0x4cc:rs:t0:t4 0x4cc:rt:t0:t4 0x4d4:rd:t1:t5 0x4d4:rt:t0:t4 0x4d8:rs:t1:t5 0x4f0:rt:t2:v0 0x4fc:rt:t2:v0 */
/* Address: 0x8013D8E0 (absorbed head) / 0x8013D8E8 (prologue) */
/* Size: 1372 bytes (343 insns) incl. the absorbed 2-word head */
#include "types.h"

extern void sub_step_up(void);
extern void sub_sub_step_up(void);
extern void goto_sub_sub_step(s32);
extern void device_cmd_send(s32);
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
extern void bunkasai_girl3_clubs2_3_srf_bank_set(void);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);

extern s16 now_serifu;
extern s16 girl3_syoushin;
extern u8  girl3_flag;
extern u8  girl3_basyo;
extern u8  now_girl;
extern u8  now_sub_step;
extern u8  now_sub_sub_step;
extern s32 bunka_aisyou;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;

void bunkasai_girl3_club3_95_scene(void) {
    s32 idx;
    s32 v0;
    s16 val;
    s32 sel;

    idx = now_sub_sub_step;
    switch (idx) {
    case 0:
        value_cache_find_or_load(16645, 1);
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
            now_serifu = 42;
            bustup_ptr_init();
            now_girl = 3;
            zyotai_sprite_set_chk();
        }
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        cd_read_start2(23, 0x801E8000, 19158);
        sub_sub_step_up();
        break;
    case 7:
        fade_up_main();
        break;
    case 8:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(3);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 101;
                break;
            case 2:
                now_serifu = 102;
                break;
            case 3:
                now_serifu = 103;
                break;
            case 4:
                now_serifu = 104;
                break;
            }
            bunkasai_girl3_clubs2_3_srf_bank_set();
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        fade_down_main();
        break;
    case 11:
        if (get_g_zyotai_low(3) == 4) {
            serifu_run();
        } else {
            sub_sub_step_up();
        }
        break;
    case 12:
        device_cmd_send(514);
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        sub_step_up();
        break;
    case 13:
        cd_read_start2(23, 0x801E8000, 19158);
        sub_sub_step_up();
        break;
    case 14:
        if (get_g_zyotai_low(3) == 4) {
            sub_sub_step_up();
        } else {
            fade_up_main();
        }
        break;
    case 15:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(3);
            switch (v0) {
            case 0:
            case 1:
                now_serifu = 106;
                break;
            case 2:
                now_serifu = 109;
                break;
            case 3:
                now_serifu = 112;
                break;
            case 4:
                now_serifu = 115;
                break;
            }
            now_serifu = now_serifu + bunka_aisyou;
            bunkasai_girl3_clubs2_3_srf_bank_set();
        }
        break;
    case 16:
        serifu_run();
        break;
    case 17:
        if (get_g_zyotai_low(3) == 4) {
            goto_sub_sub_step(21);
        } else {
            if ((girl3_syoushin & 3) == 0) {
                sel = get_g_zyotai_low(3);
                switch (sel) {
                case 0:
                case 1:
                    sel = 23;
                    goto stA;
                case 2:
                    sel = 25;
                    goto stB;
                case 3:
                    sel = 27;
                    goto stC;
                default:
                    goto stD;
                stA:
                    now_serifu = sel;
                    goto stD;
                stB:
                    now_serifu = sel;
                    goto stD;
                stC:
                    now_serifu = sel;
                    goto stD;
                stD:
                    ;
                }
            } else {
                v0 = get_g_zyotai_low(3);
                switch (v0) {
                case 0:
                case 1:
                    val = 118;
                    break;
                case 2:
                    val = 120;
                    break;
                case 3:
                    val = 122;
                    break;
                default:
                    val = now_serifu;
                    break;
                }
                if ((s32) (*(s32 *) &girl3_flag << 29) >= 0) {
                    girl3_flag |= 4;
                    val = val + 6;
                }
                now_serifu = val;
                if (((u32) girl3_basyo >> 4) == 5) {
                    now_serifu = val + 12;
                }
            }
            sub_sub_step_up();
        }
        break;
    case 18:
        serifu_run();
        break;
    case 19:
        fade_down_main();
        if (((u32) girl3_basyo >> 4) == 5) {
            if (get_g_zyotai_low(3) == 3) {
                now_serifu = 135;
            }
        }
        break;
    case 20:
        serifu_run();
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
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
