/* @SymbolStart: 0x80151F00 */
/* @RodataBase: 0x80155CB0 */
/* Address: 0x80151F08 (symbol starts 0x80151F00 -- includes the */
/* Size: 500 bytes (125 insns) including the absorbed head */
/* @SymbolSize: 500 */
#include "types.h"

extern void value_cache_find_or_load(s32, s32);
extern void sub_sub_step_up(void);
extern void fade_out(void);
extern s32  get_CD_State2(void);
extern void bunkaken_basyo_init3(void);
extern s32  dec_bg_init(s32);
extern void indexed_byte_lsb_set(s32, s32);
extern void cd_read_start2(s32, s32, s32);
extern void fade_in(void);
extern s32  get_g_zyotai_low(s32);
extern void bunkaken_girl9_pair_bank_init(void);
extern void bustup_face_change(s32);
extern void bustup_kuchi_ani(void);
extern void serifu_run(void);
extern void fade_down_main(void);
extern void fade_out_step(void);
extern void bunkaken_sub_reset(void);
extern void game_reset(void);

extern s16 now_serifu;
extern u8  now_girl;
extern u8 now_sub_sub_step;

void bunkaken_girl9_pair_bk4_96_exh_main(void) {
    s32 idx = now_sub_sub_step;
    s32 n;

    switch (idx) {
    case 0:
        value_cache_find_or_load(16664, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out();
        break;
    case 2:
        if (get_CD_State2() & 1) {
            bunkaken_basyo_init3();
            n = dec_bg_init(1);
            indexed_byte_lsb_set(1, n);
            cd_read_start2(9, 0x801E8000, 19262);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            switch (get_g_zyotai_low(9)) {
            case 0:
            case 1:
                now_serifu = 44;
                break;
            case 2:
                now_serifu = 46;
                break;
            case 3:
                now_serifu = 44;
                break;
            }
            bunkaken_girl9_pair_bank_init();
            bustup_face_change(0);
            bustup_kuchi_ani();
        }
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        fade_down_main();
        break;
    case 7:
        now_girl = 14;
        sub_sub_step_up();
        break;
    case 8:
        serifu_run();
        break;
    case 9:
        fade_out_step();
        break;
    case 10:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        bunkaken_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
