/* @SymbolStart: 0x8013A6F4 */
/* @RodataBase: 0x80153C54 */
/* @SymbolSize: 472 */
/* Address: 0x8013A6F4 (prologue 0x8013A6FC), Size: 472 bytes (118 insns) */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern s32 goto_sub_step(s32);
extern s32 device_cmd_send(s32);
extern s32 get_CD_State2(void);
extern s32 cd_read_start2(s32, s32, s32);
extern s32 game_reset(void);
extern s32 indexed_byte_lsb_set(s32, s32);
extern s32 value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);
extern s32 fade_in_step(void);
extern s32 fade_out_step(void);
extern s32 serifu_run(void);
extern void bunkaken_common_bank_init(void);
extern s32 bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);

extern s32 bunka_aisyou;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s32 bunka_girl;

extern s16 now_serifu;
extern u8 now_sub_step;
extern u8 now_sub_sub_step;

void bunkaken_main0a(void) {
    s32 v0;

    switch (now_sub_sub_step) {
    case 0:
        value_cache_find_or_load(16645, 1);
        sub_sub_step_up();
        break;
    case 1:
    case 10:
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
        cd_read_start2(5, 0x801E8000, 0x49BA);
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
        now_serifu = 0x3E;
        bunkaken_common_bank_init();
        break;
    case 5:
    case 9:
    case 12:
        serifu_run();
        break;
    case 6:
        device_cmd_send(0x202);
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_sub_step(11);
        break;
    case 7:
        bunka_girl = 0xFF;
        cd_read_start2(5, 0x801E8000, 0x49BA);
        sub_sub_step_up();
        break;
    case 8:
        v0 = get_CD_State2();
        if ((v0 & 1) == 0) {
            break;
        }
        now_serifu = bunka_aisyou + 0x3F;
        bunkaken_common_bank_init();
        break;
    case 11:
        now_serifu = 0x52;
        sub_sub_step_up();
        break;
    case 13:
        bunkaken_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
