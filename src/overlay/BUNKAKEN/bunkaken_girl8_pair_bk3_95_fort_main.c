/* @SymbolStart: 0x8013F9F0 */
/* @RodataBase: 0x801543BC */
/* @PostProc: patch_insn_reg_field 0x17c:rt:t3:t2 0x180:rt:t4:t3 0x184:rs:t4:t3 0x184:rt:t4:t3 0x18c:rt:t3:t2 0x194:rs:t4:t3 0x194:rt:t5:t4 0x198:rt:t5:t4 0x1e8:rt:t6:t5 0x1ec:rs:t6:t5 0x200:rt:t0:t9 0x214:rt:t8:t7 0x220:rt:t9:t8 0x22c:rt:t7:t6 0x238:rt:t7:t6 0x244:rt:t8:t7 0x24c:rt:t9:t8 0x250:rt:t1:t0 0x254:rs:t1:t0 0x254:rt:t1:t0 0x258:rs:t0:t9 0x258:rt:t0:t9 0x260:rd:t2:t1 0x260:rs:t0:t9 0x260:rt:t1:t0 0x268:rt:t2:t1 0x2a0:rt:t4:t3 0x2ac:rt:t5:t4 0x2b8:rt:t3:t2 0x2c4:rt:t3:t2 0x2d0:rt:t4:t3 0x2d8:rt:t5:t4 */
/* @SymbolSize: 856 */
/* Address: 0x8013F9F8 */
/* Size: 856 bytes (214 insns) */
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
extern s32 fade_up_main(void);
extern s32 fade_down_main(void);
extern s32 fade_in(void);
extern s32 fade_out(void);
extern s32 fade_out_step(void);
extern s32 serifu_run(void);
extern s32 get_g_zyotai_low(s32);
extern s32 bunkaken_girl8_pair_bank_init(void);
extern s32 bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);

extern s32 bunka_aisyou;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;

extern s16 now_serifu;
#define now_sub_step (*(u8 *)0x800EF869)
extern u8 now_sub_sub_step;
void bunkaken_girl8_pair_bk3_95_fort_main(void) {
    s32 sel = now_sub_sub_step;
    s32 v0;

    switch ((u32)sel) {
    case 0:
        value_cache_find_or_load(16645, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out();
        break;
    case 2:
        v0 = get_CD_State2();
        if ((v0 & 1) == 0) {
            break;
        }
        bunkaken_basyo_init3();
        v0 = dec_bg_init(1);
        indexed_byte_lsb_set(1, v0);
        cd_read_start2(16, 0x801E8000, 18855);
        sub_sub_step_up();
        break;
    case 3:
        fade_in();
        break;
    case 4:
        v0 = get_CD_State2();
        if ((v0 & 1) == 0) {
            break;
        }
        v0 = get_g_zyotai_low(8);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 45;
            break;
        case 2:
            now_serifu = 46;
            break;
        case 3:
            now_serifu = 47;
            break;
        }
        bunkaken_girl8_pair_bank_init();
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        fade_down_main();
        break;
    case 7:
        device_cmd_send(0x202);
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_sub_step(11);
        break;
    case 8:
        cd_read_start2(16, 0x801E8000, 18855);
        sub_sub_step_up();
        break;
    case 9:
        fade_up_main();
        break;
    case 10:
        v0 = get_CD_State2();
        if ((v0 & 1) == 0) {
            break;
        }
        v0 = get_g_zyotai_low(8);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 48;
            break;
        case 2:
            now_serifu = 51;
            break;
        case 3:
            now_serifu = 54;
            break;
        }
        now_serifu = now_serifu + bunka_aisyou;
        bunkaken_girl8_pair_bank_init();
        break;
    case 11:
        serifu_run();
        break;
    case 12:
        v0 = get_g_zyotai_low(8);
        switch (v0) {
        case 0:
        case 1:
            now_serifu = 57;
            break;
        case 2:
            now_serifu = 58;
            break;
        case 3:
            now_serifu = 59;
            break;
        }
        sub_sub_step_up();
        break;
    case 13:
        serifu_run();
        break;
    case 14:
        fade_down_main();
        break;
    case 15:
        fade_out_step();
        break;
    case 16:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 17:
        serifu_run();
        break;
    case 18:
        bunkaken_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
