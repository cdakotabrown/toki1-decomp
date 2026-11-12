/* @SymbolStart: 0x80134400 */
/* @RodataBase: 0x80153330 */
/* @SymbolSize: 460 */
/* @PostProc: patch_insn_reg_field 0x104:rt:t1:t0 0x110:rt:t3:t1 0x120:rt:t0:at 0x128:rs:t0:at 0x12c:rt:t2:at 0x134:rs:t2:at 0x134:rt:t1:t0 0x138:rt:t4:at 0x13c:rs:t4:at 0x13c:rt:t3:t1 */
/* @PostProc: patch_insn_reg_field 0x190:rt:t5:at 0x198:rs:t5:at */
/* Address: 0x80134408 */
/* Size: 452 bytes (113 insns) */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 get_CD_State2();
extern s32 cd_read_start2();
extern s32 game_reset();
extern s32 indexed_byte_lsb_set();
extern s32 value_cache_find_or_load();
extern s32 dec_bg_init();
extern s32 fade_down_main();
extern s32 fade_in();
extern s32 fade_out_step();
extern s32 fade_out();
extern s32 serifu_run();
extern s32 get_g_zyotai_low();
extern void bunkaken_girl0_pair_bank_init();
extern s32 bunkaken_sub_reset();
extern void bunkaken_basyo_init3();
extern u8 now_sub_sub_step;

void bunkaken_main1c(void) {
    s32 t6 = now_sub_sub_step;
    switch (t6) {
        case 0:
            value_cache_find_or_load(16664, 1);
            sub_sub_step_up();
            break;
        case 1:
            fade_out();
            break;
        case 2: {
            s32 v0 = get_CD_State2();
            if (!(v0 & 1)) break;
            bunkaken_basyo_init3();
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            cd_read_start2(15, 0x801E8000, 18947);
            sub_sub_step_up();
            break;
        }
        case 3:
            fade_in();
            break;
        case 4: {
            s32 v0 = get_CD_State2();
            if (!(v0 & 1)) break;
            v0 = get_g_zyotai_low(0);
            switch (v0) {
                case 0:
                case 1:
                    *(s16*)0x800D0648 = 99;
                    break;
                case 2:
                    *(s16*)0x800D0648 = 101;
                    break;
                case 3:
                    *(s16*)0x800D0648 = 103;
                    break;
            }
            bunkaken_girl0_pair_bank_init();
            break;
        }
        case 5:
            serifu_run();
            break;
        case 6:
            fade_down_main();
            break;
        case 7:
            serifu_run();
            break;
        case 8:
            fade_out_step();
            break;
        case 9:
            *(s16*)0x800D0648 = 0;
            sub_sub_step_up();
            break;
        case 10:
            serifu_run();
            break;
        case 11:
            bunkaken_sub_reset();
            break;
        default:
            game_reset();
            break;
    }
}
