/* @SymbolStart: 0x8013E2C0 */
/* @RodataBase: 0x801541B0 */
/* @SymbolSize: 696 */
/* @PostProc: patch_insn_reg_field 0x154:rt:t1:at 0x15c:rs:t1:at 0x160:rt:t3:at 0x168:rs:t3:at 0x168:rt:t2:t1 0x16c:rt:t5:at 0x170:rs:t5:at 0x170:rt:t4:t2 0x138:rt:t2:t1 0x144:rt:t4:t2 */
/* @PostProc: patch_insn_reg_field 0x1f8:rt:t6:at 0x200:rs:t6:at */
/* @PostProc: patch_insn_reg_field 0x24c:rt:t7:t3 0x250:rt:t8:at 0x254:rs:t8:at 0x254:rt:t7:t3 */
/* Address: 0x8013E2C8 */
/* Size: 688 bytes (172 insns) */
#include "types.h"

extern s32 sub_sub_step_up();
extern s32 goto_sub_sub_step();
extern s32 get_CD_State2();
extern s32 cd_read_start2();
extern s32 game_reset();
extern s32 indexed_byte_lsb_set();
extern s32 value_cache_find_or_load();
extern s32 dec_bg_init();
extern s32 bustup_ptr_init();
extern s32 fade_up_main();
extern s32 fade_down_main();
extern s32 fade_in();
extern s32 fade_out_step();
extern s32 fade_out();
extern s32 zyotai_sprite_set_chk();
extern s32 date_name_set();
extern s32 serifu_run();
extern s32 get_g_zyotai_low();
extern s32 memcpy();
extern void bunkaken_girl10_pair_bank_init();
extern s32 bunkaken_sub_reset();
extern void bunkaken_basyo_init3();
extern u8 now_sub_sub_step;

void bunkaken_girl10_pair_bk4_96_exh_main(void) {
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
            cd_read_start2(45, 0x80197000, 27743);
            sub_sub_step_up();
            break;
        }
        case 3:
            fade_in();
            break;
        case 4: {
            s32 v0 = get_CD_State2();
            if (!(v0 & 1)) break;
            cd_read_start2(13, 0x801E8000, 19314);
            sub_sub_step_up();
            break;
        }
        case 5: {
            s32 v0 = get_CD_State2();
            if (!(v0 & 1)) break;
            v0 = get_g_zyotai_low(10);
            switch (v0) {
                case 0:
                case 1:
                    *(s16*)0x800D0648 = 53;
                    break;
                case 2:
                    *(s16*)0x800D0648 = 55;
                    break;
                case 3:
                    *(s16*)0x800D0648 = 57;
                    break;
            }
            bunkaken_girl10_pair_bank_init();
            break;
        }
        case 6:
            serifu_run();
            break;
        case 7:
            fade_down_main();
            break;
        case 8:
            serifu_run();
            break;
        case 9: {
            s32 v0 = get_g_zyotai_low(10);
            if (v0 == 3) {
                goto_sub_sub_step(14);
            } else {
                sub_sub_step_up();
            }
            break;
        }
        case 10:
            fade_out_step();
            break;
        case 11:
            *(s16*)0x800D0648 = 0;
            sub_sub_step_up();
            break;
        case 12:
            serifu_run();
            break;
        case 13:
            bunkaken_sub_reset();
            break;
        case 14:
            memcpy(0x80180000, 0x80197000, 0x00016800);
            bustup_ptr_init();
            *(u8*)0x800EF6BF = 13;
            date_name_set(13);
            zyotai_sprite_set_chk();
            break;
        case 15:
            fade_up_main();
            break;
        case 16:
            serifu_run();
            break;
        case 17:
            fade_down_main();
            break;
        case 18:
            goto_sub_sub_step(10);
            break;
        default:
            game_reset();
            break;
    }
}
