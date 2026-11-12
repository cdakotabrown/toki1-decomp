/* @SymbolStart: 0x8013ADC4 */
/* @RodataBase: 0x80153D14 */
/* @SymbolSize: 316 */
/* @PostProc: patch_insn_reg_field 0xe8:rt:t0:at */
/* @PostProc: patch_insn_reg_field 0xf0:rs:t0:at */
/* @PostProc: patch_insn_reg_field 0x10c:rt:t1:t0 */
/* @PostProc: patch_insn_reg_field 0x110:rt:t2:at */
/* @PostProc: patch_insn_reg_field 0x118:rt:t1:t0 0x118:rs:t2:at */
/* bunkaken_main0e -- BUNKAKEN overlay, Address: 0x8013ADCC, Size: 308 bytes (77 insns) */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 get_CD_State2();
extern s32 cd_read_start2();
extern s32 game_reset();
extern s32 indexed_byte_lsb_set();
extern s32 value_cache_find_or_load();
extern s32 dec_bg_init();
extern s32 fade_in_step();
extern s32 fade_out_step();
extern s32 serifu_run();
extern void bunkaken_common_bank_init();
extern s32 bunkaken_sub_reset();
extern void bunkaken_basyo_init3();
extern u8 now_sub_sub_step;

#define now_serifu (*(s16 *)0x800D0648)

void bunkaken_main0e(void) {
    s32 t6 = now_sub_sub_step;
    s32 result;

    switch (t6) {
    case 0:
        value_cache_find_or_load(16655, 1);
        sub_sub_step_up();
        return;
    case 1:
    case 6:
        fade_out_step();
        return;
    case 2:
        result = get_CD_State2();
        if ((result & 1) == 0) {
            return;
        }
        bunkaken_basyo_init3();
        result = dec_bg_init(1);
        result = indexed_byte_lsb_set(1, result);
        cd_read_start2(5, 0x801E8000, 18874);
        sub_sub_step_up();
        return;
    case 3:
        fade_in_step();
        return;
    case 4:
        result = get_CD_State2();
        if ((result & 1) == 0) {
            return;
        }
        now_serifu = 70;
        bunkaken_common_bank_init();
        return;
    case 5:
    case 8:
        serifu_run();
        return;
    case 7:
        now_serifu = 82;
        sub_sub_step_up();
        return;
    case 9:
        bunkaken_sub_reset();
        return;
    default:
        game_reset();
        return;
    }
}
