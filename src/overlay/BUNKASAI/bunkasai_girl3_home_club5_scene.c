/* @SymbolStart: 0x80146A78 */
/* @SymbolSize: 1216 */
/* bunkasai_girl3_home_club5_scene -- BUNKASAI overlay, Address: 0x80146A80, Size: 1208 bytes */
/* @RodataBase: 0x8015DE04 */
/* @PostProc: patch_insn_reg_field 0x18c:rt:t7:v0 0x190:rs:t7:v0 0x190:rt:t7:v0 0x19c:rd:t8:t7 0x19c:rt:t7:v0 0x1a0:rd:t9:t8 0x1a0:rt:t8:t7 0x1a4:rs:t9:t8 0x1a4:rt:t0:t9 0x1b0:rt:t0:t9 0x1ec:rt:t1:t0 0x1f4:rt:t2:t1 0x1f8:rs:t2:t1 0x1f8:rt:t2:t1 0x1fc:rt:t1:t0 0x204:rt:t2:t1 0x20c:rt:t3:t2 0x210:rt:t3:t2 0x23c:rt:t4:t3 0x248:rt:t4:t3 0x24c:rt:t5:t4 0x258:rt:t5:t4 0x290:rt:t7:t5 0x294:rs:t7:t5 0x2c8:rt:t8:t7 0x2d4:rt:t8:t7 0x2d8:rt:t9:t8 0x2e4:rt:t9:t8 0x2e8:rt:t0:t9 0x2f4:rt:t0:t9 0x2f8:rt:t1:t0 0x300:rt:t1:t0 0x30c:rt:t2:t1 0x310:rd:t3:t2 0x318:rs:t2:t1 0x318:rt:t2:t1 0x31c:rd:t3:t2 0x31c:rs:t3:t2 0x324:rd:t3:t2 0x324:rt:t3:t2 0x32c:rt:t7:t5 0x334:rd:t4:t3 0x334:rs:t2:t1 0x334:rt:t3:t2 0x338:rs:t7:t5 0x338:rt:t7:t5 0x33c:rs:t4:t3 0x33c:rt:t5:t4 0x348:rt:t5:t4 0x34c:rt:t8:t7 0x350:rs:t7:t5 0x350:rt:t8:t7 0x36c:rt:t1:t0 0x370:rt:t9:t8 0x374:rs:t9:t8 0x374:rt:t9:t8 0x384:rs:t9:t8 0x384:rt:t0:t9 0x38c:rt:t0:t9 0x390:rs:t1:t0 0x390:rt:t1:t0 0x39c:rs:t1:t0 0x39c:rt:t2:t1 0x3a4:rt:t2:t1 0x3c4:rt:t3:t2 0x3c8:rs:t3:t2 0x3c8:rt:t3:t2 0x3d0:rs:t3:t2 0x3d0:rt:t4:t3 0x3d4:rt:t4:t3 0x480:rt:t5:t4 0x48c:rt:t5:t4 */
#include "types.h"

extern u8 now_sub_sub_step;
extern void value_cache_find_or_load(s32, s32);
extern void sub_sub_step_up(void);
extern void fade_out_step(void);
extern s32  get_CD_State2(void);
extern void bustup_cd_read(s32);
extern s32  dec_bg_init(s32);
extern void indexed_byte_lsb_set(s32, s32);
extern void bunkasai_basyo_init2(void);
extern void bustup_ptr_init(void);
extern void zyotai_sprite_set_chk(void);
extern void cd_read_start2(s32, s32, s32);
extern void fade_in(void);
extern s32  get_g_zyotai_low(s32);
extern void bunkasai_girl3_home_srf_bank_set(void);
extern void serifu_run(void);
extern void fade_down_main(void);
extern void fade_out(void);
extern void goto_step(s32);
extern void bustup_load_step(s32);
extern void goto_sub_sub_step(s32);
extern void bunkasai_sub_reset(void);
extern void game_reset(void);

extern s16 now_serifu;
extern u8  now_girl;
extern u8  now_year;
extern u8  now_sub_step;
extern u16 bukatu_pt5;
extern s32 bunka_sd_kind;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

void bunkasai_girl3_home_club5_scene(void)
{
    s32 loc;
    s32 t;
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
            bustup_cd_read(22703);
            t = dec_bg_init(1);
            indexed_byte_lsb_set(1, t);
            sub_sub_step_up();
        }
        break;
    case 3:
        if (get_CD_State2() & 1) {
            bunkasai_basyo_init2();
            bustup_ptr_init();
            now_girl = 3;
            zyotai_sprite_set_chk();
            cd_read_start2(12, 0x801E8000, 19196);
        }
        break;
    case 4:
        fade_in();
        break;
    case 5:
        if (get_CD_State2() & 1) {
            switch (get_g_zyotai_low(3)) {
            case 0:
            case 1:
                now_serifu = 30;
                break;
            case 2:
                now_serifu = 31;
                break;
            case 3:
                now_serifu = 32;
                break;
            case 4:
                now_serifu = 33;
                break;
            }
            now_serifu = now_serifu + now_year * 4 - 380;
            bunkasai_girl3_home_srf_bank_set();
        }
        break;
    case 6:
        serifu_run();
        break;
    case 7:
        fade_down_main();
        break;
    case 8:
        fade_out();
        break;
    case 9:
        bunka_sd_kind = 6;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_step(128);
        break;
    case 10:
        bustup_load_step(22703);
        break;
    case 11:
        bunkasai_basyo_init2();
        mes_base_y = 192;
        bustup_ptr_init();
        now_girl = 3;
        zyotai_sprite_set_chk();
        cd_read_start2(12, 0x801E8000, 19196);
        break;
    case 12:
        fade_in();
        break;
    case 13:
        if (get_CD_State2() & 1) {
            switch (get_g_zyotai_low(3)) {
            case 0:
            case 1:
                now_serifu = 42;
                break;
            case 2:
                now_serifu = 44;
                break;
            case 3:
                now_serifu = 46;
                break;
            case 4:
                now_serifu = 48;
                break;
            }
            loc = now_year;
            now_serifu = now_serifu + loc * 10 - 950;
            if ((u32)bukatu_pt5 < (u32)(loc * 100 - 9400)) {
                if (get_g_zyotai_low(3) == 4) {
                    now_serifu = now_serifu + 2;
                } else {
                    now_serifu = now_serifu + 1;
                }
            }
            if (now_year == 97) {
                if ((u32)get_g_zyotai_low(3) >= 2) {
                    now_serifu = now_serifu - 2;
                }
            }
            bunkasai_girl3_home_srf_bank_set();
        }
        break;
    case 14:
        serifu_run();
        break;
    case 15:
        if (get_g_zyotai_low(3) == 4) {
            fade_down_main();
        } else {
            goto_sub_sub_step(17);
        }
        break;
    case 16:
        serifu_run();
        break;
    case 17:
        if (get_g_zyotai_low(3) == 4) {
            sub_sub_step_up();
        } else {
            fade_down_main();
        }
        break;
    case 18:
        fade_out_step();
        break;
    case 19:
        now_serifu = 1;
        sub_sub_step_up();
        break;
    case 20:
        serifu_run();
        break;
    case 21:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
