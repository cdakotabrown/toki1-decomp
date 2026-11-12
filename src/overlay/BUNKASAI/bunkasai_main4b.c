/* @RodataBase: 0x8015D0F8 */
/* @SymbolStart: 0x8013EFB4 */
/* @SymbolSize: 636 */
/* Address: 0x8013EFB4 (absorbed head) / 0x8013EFBC (prologue) */
/* Size: 636 bytes (159 insns) incl. the absorbed 2-word head */
#include "types.h"

extern u8 now_sub_sub_step;
extern u8 now_girl;
extern s16 now_serifu;

extern void sub_sub_step_up(void);
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

void bunkasai_main4b(void) {
    switch (now_sub_sub_step) {
    case 0:
        value_cache_find_or_load(16664, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out_step();
        break;
    case 2:
        if (get_CD_State2() & 1) {
            bunkasai_basyo_init();
            indexed_byte_lsb_set(1, dec_bg_init(1));
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
            now_serifu = 44;
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
            switch ((u32)get_g_zyotai_low(3)) {
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
                now_serifu = 34;
                break;
            }
            bunkasai_girl3_clubs4_5_srf_bank_set();
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        fade_down_main();
        break;
    case 11:
        if (get_g_zyotai_low(3) != 4) {
            now_serifu = 33;
        }
        sub_sub_step_up();
        break;
    case 12:
        serifu_run();
        break;
    case 13:
        fade_out_step();
        break;
    case 14:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 15:
        serifu_run();
        break;
    case 16:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
