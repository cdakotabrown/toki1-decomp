/* @SymbolStart: 0x80139FB8 */
/* @RodataBase: 0x80153B58 */
/* @SymbolSize: 352 */
/* @PostProc: patch_insn_reg_field 0xe8:rt:t0:t9 0xf0:rs:t0:t9 0xf4:rt:t3:t1 0xf8:rt:t1:t0 0xfc:rt:t2:at 0x104:rs:t2:at 0x104:rt:t1:t0 0x108:rt:t4:at 0x10c:rs:t4:at 0x10c:rt:t3:t1 0x130:rt:t5:t2 0x134:rt:t6:at 0x13c:rs:t6:at 0x13c:rt:t5:t2 */
/* bunkaken_solo_bk0_95_96_exh_main - BUNKAKEN overlay, Address: 0x80139FC0, Size: 344 bytes (86 insns) */
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
#define now_year (*(u8 *)0x800EE79E)

void bunkaken_solo_bk0_95_96_exh_main(void) {
    s32 t6 = now_sub_sub_step;
    s32 result;

    switch (t6) {
    case 0:
        value_cache_find_or_load(16618, 1);
        sub_sub_step_up();
        return;
    case 1:
    case 6:
    case 10:
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
    case 8:
        fade_in_step();
        return;
    case 4:
        result = get_CD_State2();
        if ((result & 1) == 0) {
            return;
        }
        if (now_year == 95) {
            now_serifu = 48;
        } else {
            now_serifu = 51;
        }
        bunkaken_common_bank_init();
        return;
    case 5:
    case 7:
    case 9:
    case 12:
        serifu_run();
        return;
    case 11:
        now_serifu = 82;
        sub_sub_step_up();
        return;
    case 13:
        bunkaken_sub_reset();
        return;
    default:
        game_reset();
        return;
    }
}
