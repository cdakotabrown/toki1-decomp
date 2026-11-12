/* @SymbolStart: 0x80135554 */
/* @RodataBase: 0x8015BED0 */
/* Address: 0x8013555C */
/* Size: 952 bytes (238 insns) */
/* @SymbolSize: 960 */
#include "types.h"

extern void sub_sub_step_up(void);
extern void goto_sub_sub_step(s32);
extern s32  get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void game_reset(void);
extern void indexed_byte_lsb_set(s32, s32);
extern void value_cache_find_or_load(s32, s32);
extern s32  dec_bg_init(s32);
extern void bustup_ptr_init(void);
extern void fade_up_main(void);
extern void fade_down_main(void);
extern void fade_in_step(void);
extern void fade_in(void);
extern void fade_out_step(void);
extern void fade_out(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32  get_g_zyotai_low(s32);
extern void bustup_load_step(s32);
extern void bunkasai_girl1_cl_bank_init(void);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);

extern s16 now_serifu;
extern u8  now_year;
extern u16 bukatu_rec;
extern u8  now_girl;

extern u8 now_sub_sub_step;
void bunkasai_main2b(void) {
    s32 sel = now_sub_sub_step;

    s32 v0;
    s32 loc;

    switch (sel) {
    case 0:
        value_cache_find_or_load(16618, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out_step();
        break;
    case 2:
        if (get_CD_State2() & 1) {
            bustup_load_step(23873);
        }
        break;
    case 3:
        v0 = dec_bg_init(1);
        indexed_byte_lsb_set(1, v0);
        bunkasai_basyo_init();
        bustup_ptr_init();
        now_girl = 1;
        zyotai_sprite_set_chk();
        cd_read_start2(11, 0x801E8000, 19113);
        break;
    case 4:
        fade_in_step();
        break;
    case 5:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(1);
            switch (v0) {
            case 0: case 1:
                now_serifu = 2;
                break;
            case 2:
                now_serifu = 4;
                break;
            case 3:
                now_serifu = 6;
                break;
            case 4:
                now_serifu = 8;
                break;

                break;
            }
            if (now_year == 96) {
                now_serifu = now_serifu + 8;
            }
            bunkasai_girl1_cl_bank_init();
        }
        break;
    case 6:
        fade_up_main();
        break;
    case 7:
        serifu_run();
        break;
    case 8:
        serifu_run();
        break;
    case 9:
        fade_out();
        now_serifu = 0;
        break;
    case 10:
        serifu_run();
        break;
    case 11:
        fade_in();
        break;
    case 12:
        v0 = get_g_zyotai_low(1);
        switch (v0) {
        case 0: case 1:
            now_serifu = 18;
            break;
        case 2:
            now_serifu = 20;
            break;
        case 3:
            now_serifu = 22;
            break;
        case 4:
            now_serifu = 24;
            break;

            break;
        }
        loc = now_year;
        if (bukatu_rec < (u32)(loc * 100 - 9400)) {
            now_serifu = now_serifu + 1;
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
        if (get_g_zyotai_low(1) == 4) {
            loc = now_year;
            if (bukatu_rec < (u32)(loc * 100 - 9400)) {
                now_serifu = 61;
            } else {
                now_serifu = 62;
            }
            sub_sub_step_up();
        } else {
            goto_sub_sub_step(17);
        }
        break;
    case 16:
        serifu_run();
        break;
    case 17:
        fade_out_step();
        break;
    case 18:
        now_serifu = 1;
        sub_sub_step_up();
        break;
    case 19:
        serifu_run();
        break;
    case 20:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
