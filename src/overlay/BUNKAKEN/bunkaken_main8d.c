/* @SymbolStart: 0x80146424 */
/* @RodataBase: 0x80154C90 */
/* @SymbolSize: 380 */
/* bunkaken_main8d -- BUNKAKEN overlay, Address: 0x8014642C, Size: 368 bytes (92 insns) */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 goto_step();
extern s32 get_CD_State2();
extern s32 cd_read_start2();
extern s32 game_reset();
extern void class_clear_all();
extern s32 k_disp_start();
extern s32 k_reset();
extern s32 set_dec_bri();
extern s32 basyo_init();
extern void set_flag_b3_high_bit();
extern s32 fade_out_step();
extern s32 date_name_set();
extern s32 serifu_run();
extern s32 strcpy();
extern void bunkaken_tt_intro_bank_set();

extern s16 now_serifu;
extern s16 now_serifu_in;
extern s32 ss_serifu;
extern s32 ss_serifu_kind;
extern s32 sss_sound;
extern u8 now_girl;
extern u8 pad_type;

extern s32 bunkaken_main8d_str0;
extern s32 bunkaken_main8d_str1;
extern u8 g_name[];
extern s32 bunkaken_bank_ptr[];
extern u8 now_sub_sub_step;

void bunkaken_main8d(void) {
    s32 t6 = now_sub_sub_step;
    s32 result;

    switch (t6) {
    case 0:
        cd_read_start2(1, 0x801E8000, 19327);
        sub_sub_step_up();
        return;
    case 1:
        result = get_CD_State2();
        if ((result & 1) == 0) {
            return;
        }
        k_reset(0);
        basyo_init((s32)&bunkaken_main8d_str0);
        k_disp_start(1);
        bunkaken_tt_intro_bank_set();
        now_girl = 8;
        date_name_set(8);
        strcpy((s32)g_name, (s32)&bunkaken_main8d_str1);
        now_serifu = 0;
        now_serifu_in = 0;
        ss_serifu = bunkaken_bank_ptr[0];
        ss_serifu_kind = bunkaken_bank_ptr[1];
        sss_sound = bunkaken_bank_ptr[2];
        sub_sub_step_up();
        return;
    case 2:
        serifu_run();
        return;
    case 3:
        fade_out_step();
        return;
    case 4:
        if (pad_type != 1) {
            return;
        }
        class_clear_all(0);
        set_flag_b3_high_bit(0);
        set_dec_bri(0);
        goto_step(208);
        return;
    default:
        game_reset();
        return;
    }
}
