/* @SymbolStart: 0x8013D048 */
/* @RodataBase: 0x80154038 */
/* @SymbolSize: 428 */
/* @PostProc: swap_registers a0:t6 */
/* @PostProc: scheduler_normalizer */
/* @PostProc: swap_instructions --offset1 0x104 --offset2 0x110 */
/* @PostProc: patch_insn_reg_field 0x104:rt:t1:t0 0x110:rt:t3:t1 0x120:rt:t0:at 0x128:rs:t0:at 0x12c:rt:t2:at 0x134:rs:t2:at 0x134:rt:t1:t0 0x138:rt:t4:at 0x13c:rs:t4:at 0x13c:rt:t3:t1 0x170:rt:t5:at 0x178:rs:t5:at 0x44:rt:t6:a0 0x90:rt:t6:a0 0x94:rt:t6:a0 0xa8:rt:t6:a0 0xec:rt:t6:a0 */
/* Address: 0x8013D050 */
/* Size: 420 bytes (105 insns), BYTE-PERFECT */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 get_CD_State2();
extern s32 cd_read_start2();
extern s32 game_reset();
extern s32 indexed_byte_lsb_set();
extern s32 value_cache_find_or_load();
extern s32 dec_bg_init();
extern s32 fade_in();
extern s32 fade_out();
extern s32 serifu_run();
extern s32 get_g_zyotai_low();
extern void bunkaken_girl10_pair_bank_init();
extern s32 bunkaken_sub_reset();
extern void bunkaken_basyo_init3();
extern u8 now_sub_sub_step;

#define now_serifu (*(volatile s16 *)0x800D0648)

void bunkaken_girl10_pair_bk0_96_exh_main(void) {
    s32 t6 = now_sub_sub_step;
    s32 result;

    switch (t6) {
    case 0:
        value_cache_find_or_load(16618, 1);
        sub_sub_step_up();
        return;
    case 1:
        fade_out();
        return;
    case 2:
        result = get_CD_State2();
        if ((result & 1) == 0) {
            return;
        }
        bunkaken_basyo_init3();
        result = dec_bg_init(1);
        result = indexed_byte_lsb_set(1, result);
        cd_read_start2(13, 0x801E8000, 19314);
        sub_sub_step_up();
        return;
    case 3:
        fade_in();
        return;
    case 4:
        result = get_CD_State2();
        if ((result & 1) == 0) {
            return;
        }
        switch (get_g_zyotai_low(10)) {
        case 0:
        case 1:
            now_serifu = 2;
            break;
        case 2:
            now_serifu = 3;
            break;
        case 3:
            now_serifu = 4;
            break;
        }
        bunkaken_girl10_pair_bank_init();
        return;
    case 5:
        serifu_run();
        return;
    case 6:
        fade_out();
        return;
    case 7:
        now_serifu = 0;
        sub_sub_step_up();
        return;
    case 8:
        serifu_run();
        return;
    case 9:
        bunkaken_sub_reset();
        return;
    default:
        game_reset();
        return;
    }
}
