/* @SymbolStart: 0x80134CF8 */
/* @RodataBase: 0x8015BDF8 */
/* Address: 0x80134D00 */
/* Size: 936 bytes (234 insns) */
/* @SymbolSize: 944 */
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
extern void bunkasai_girl2_vst_bank_init(void);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);

typedef struct {
    u32 lo2     : 2;
    u32 visited : 1;
    u32 mid17   : 17;
    u32 nib     : 4;
    u32 hi8     : 8;
} GirlFlags;

extern s16 now_serifu;
extern s16 girl2_syoushin;
extern GirlFlags girl2_flag;
extern u8  now_girl;

extern u8 now_sub_sub_step;
void bunkasai_girl2_vst_bk4_97_exh_main(void) {
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
            bustup_cd_read(21038);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        if (get_CD_State2() & 1) {
            bustup_ptr_init();
            now_girl = 2;
            zyotai_sprite_set_chk();
            now_serifu = 45;
        }
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        cd_read_start2(26, 0x801E8000, 18962);
        sub_sub_step_up();
        break;
    case 7:
        fade_up_main();
        break;
    case 8:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(2);
            switch (v0) {

                break;
            case 0: case 1:
                now_serifu = 174;
                break;
            case 2:
                now_serifu = 176;
                break;
            case 3:
                now_serifu = 178;
                break;
            case 4:
                now_serifu = 187;
                break;
            }
            bunkasai_girl2_vst_bank_init();
        }
        break;
    case 9:
        serifu_run();
        break;
    case 10:
        v0 = get_g_zyotai_low(2);
        if (v0 == 3) {
            goto_sub_sub_step(17);
        } else {
            sub_sub_step_up();
        }
        break;
    case 11:
        fade_down_main();
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
    case 17:
        if ((girl2_syoushin & 3) == 0) {
            now_serifu = 185;
            goto_sub_sub_step(20);
        } else {
            if (!girl2_flag.visited) {
                girl2_flag.visited = 1;
                if (girl2_flag.nib == 2) {
                    now_serifu = 181;
                } else {
                    now_serifu = 182;
                }
            } else {
                if (girl2_flag.nib == 2) {
                    now_serifu = 179;
                } else {
                    now_serifu = 180;
                }
            }
            sub_sub_step_up();
        }
        break;
    case 18:
        serifu_run();
        break;
    case 19:
        now_serifu = 183;
        sub_sub_step_up();
        break;
    case 20:
        serifu_run();
        break;
    case 21:
        fade_down_main();
        break;
    case 22:
        serifu_run();
        break;
    case 23:
        goto_sub_sub_step(13);
        break;
    default:
        game_reset();
        break;
    }
}
