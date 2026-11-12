/* @SymbolStart: 0x80151064 */
/* @SymbolSize: 376 */
/* @RodataBase: 0x8015F1BC */
/* Address: 0x8015106C */
/* Size: 368 bytes (92 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern void goto_step(s32 a0);
extern s32 get_CD_State2(void);
extern void cd_read_start2(s32 a0, s32 a1, s32 a2);
extern void game_reset();
extern void class_clear_all(s32 a0);
extern s32 k_disp_start(s32 a0);
extern s32 k_reset();
extern void set_dec_bri(s32 a0);
extern s32 basyo_init(s32 a0);
extern void set_flag_b3_high_bit(s32 a0);
extern void fade_out_step(void);
extern void date_name_set(s32 a0);
extern void serifu_run(void);
extern s32 strcpy(s32 a0, s32 a1);
extern void bunkasai_tt_intro_bank_set(void);

extern s16 now_serifu;
extern s16 now_serifu_in;
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern u8 now_girl;
extern u8 pad_type;
extern s32 bunkasai_serifu_ptr[];
extern s32 g_name;
extern s32 bunkasai_maina4_str0;
extern s32 bunkasai_maina4_str1;

extern u8 now_sub_sub_step;
void bunkasai_maina4(void) {
    s32 t6 = now_sub_sub_step;

    s32 result;

    switch (t6) {
    case 0:
        cd_read_start2(1, 0x801E8000, 19327);
        sub_sub_step_up();
        break;

    case 1:
        result = get_CD_State2();
        if (result & 1) {
            k_reset(0);
            basyo_init((s32)&bunkasai_maina4_str0);
            k_disp_start(1);
            bunkasai_tt_intro_bank_set();
            now_girl = 8;
            date_name_set(8);
            strcpy((s32)&g_name, (s32)&bunkasai_maina4_str1);
            now_serifu = 0;
            now_serifu_in = 0;
            ss_serifu = bunkasai_serifu_ptr[0];
            ss_serifu_kind = bunkasai_serifu_ptr[1];
            sss_sound = bunkasai_serifu_ptr[2];
            sub_sub_step_up();
        }
        break;

    case 2:
        serifu_run();
        break;

    case 3:
        fade_out_step();
        break;

    case 4:
        if (pad_type == 1) {
            class_clear_all(0);
            set_flag_b3_high_bit(0);
            set_dec_bri(0);
            goto_step(208);
        }
        break;
    default:
        game_reset();
        break;
    }
}
