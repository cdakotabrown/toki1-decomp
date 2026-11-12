/* @RodataBase: 0x80153d3c */
/* @SymbolSize: 624 */
/* Address: 0x8013AF00 */
/* Size: 624 bytes (156 insns) */
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
extern void fade_out_step(void);
extern void menu2_main(void);
extern void zyotai_sprite_set_chk(void);
extern void date_name_set(s32);
extern void serifu_run(void);
extern void bustup_load_step(s32);
extern void bunkaken_common_bank_init(void);
extern void bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);

extern s16 now_serifu;
extern u8  now_girl;
extern s32 serifu_no;

extern u8 now_sub_sub_step;

void bunkaken_main0f(void)
{
    s32 t6;
    s32 v0;

    t6 = now_sub_sub_step;

    switch ((u32)t6) {
    case 0:
        value_cache_find_or_load(16664, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out_step();
        break;
    case 2:
        v0 = get_CD_State2();
        if ((v0 & 1) == 0) {
            break;
        }
        bunkaken_basyo_init3();
        v0 = dec_bg_init(1);
        indexed_byte_lsb_set(1, v0);
        cd_read_start2(5, 0x801E8000, 18874);
        sub_sub_step_up();
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        v0 = get_CD_State2();
        if ((v0 & 1) == 0) {
            break;
        }
        bunkaken_common_bank_init();
        date_name_set(13);
        now_serifu = 71;
        break;
    case 5:
        bustup_load_step(27743);
        break;
    case 6:
        bustup_ptr_init();
        now_girl = 13;
        zyotai_sprite_set_chk();
        break;
    case 7:
        serifu_run();
        break;
    case 8:
        fade_up_main();
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        menu2_main();
        break;
    case 11:
        if (serifu_no != 0) {
            now_serifu = 75;
            goto_sub_sub_step(17);
        } else {
            sub_sub_step_up();
        }
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
        goto_sub_sub_step(20);
        break;
    case 17:
        serifu_run();
        break;
    case 18:
        fade_down_main();
        break;
    case 19:
        fade_out_step();
        break;
    case 20:
        now_serifu = 82;
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
