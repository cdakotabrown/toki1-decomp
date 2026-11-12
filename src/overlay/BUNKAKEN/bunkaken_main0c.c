/* @SymbolStart: 0x8013A8CC */
/* @SymbolSize: 804 */
/* @RodataBase: 0x80153C8C */
/* @PostProc: swap_instructions --offset1 0xf8 --offset2 0xfc */
/* @PostProc: swap_instructions --offset1 0x100 --offset2 0x104 */
/* @PostProc: patch_insn_reg_field 0xe8:rt:t9:t4 0xec:rs:t9:t4 0xec:rt:t9:t4 0xf4:rt:t0:t5 0xf8:rt:t9:t4 0xfc:rs:t0:t5 0xfc:rt:t0:t5 0x100:rt:t2:v1 0x108:rs:t2:v1 0x108:rt:t2:t9 0x10c:rt:t0:t5 0x114:rt:t1:t6 0x118:rt:t1:t6 0x11c:rs:t2:t9 0x11c:rt:t3:t1 0x120:rs:t3:t1 0x120:rt:t4:t3 0x128:rt:t4:t3 0x13c:rt:t5:t7 0x144:rt:t6:t8 0x148:rs:t6:t8 0x148:rt:t6:t8 0x14c:rt:t5:t7 0x154:rt:t6:t8 0x15c:rt:t7:t9 0x160:rt:t7:t9 0x17c:rt:t8:t0 0x180:rt:t9:t1 0x184:rt:t9:t1 0x188:rt:t8:t0 0x1b4:rt:t0:t2 0x1c0:rt:t0:t2 0x238:rt:t1:t3 0x23c:rs:t1:t3 0x294:rt:t2:t4 0x298:rs:t2:t4 0x29c:rt:t3:t5 0x2a8:rt:t3:t5 0x2d4:rt:t4:t6 0x2e0:rt:t4:t6 0x2fc:rt:t5:v1 0x300:rs:t5:v1 0x300:rt:t5:t7 0x308:rs:t5:t7 0x308:rt:t6:t9 0x310:rt:t6:t9 */
/* Address: 0x8013A8D4 */
/* Size: 796 bytes (199 insns) */
#include "types.h"

extern s32 sub_sub_step_up(void);
extern s32 goto_sub_step(s32);
extern s32 device_cmd_send(s32);
extern s32 device_load_start(s32, s32, s32, s32, s32, s32);
extern s32 device_load_poll(void);
extern s32 get_CD_State2(void);
extern s32 cd_read_start2(s32, s32, s32);
extern s32 game_reset(void);
extern s32 set_dec_bri(s32);
extern s32 dec_bg_reset(void);
extern s32 indexed_byte_lsb_set(s32, s32);
extern s32 value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);
extern s32 message_window_init(void);
extern s32 fade_in_step(void);
extern s32 fade_out_step(void);
extern s32 date_name_set(s32);
extern s32 yazirushi_off(void);
extern s32 serifu_run(void);
extern s32 date_x_taku_set(s32, s32);
extern void bunkaken_common_bank_init(void);
extern s32 bunkaken_sub_reset(void);
extern void bunkaken_basyo_init3(void);

extern s16 now_serifu;
extern u8 now_step;
extern u8 now_sub_step;
extern u8 ret_step;
extern u8 ret_sub_step;
extern u8 ret_sub_sub_step;
extern u8 bunkaken_flags;
extern s32 common_disp_off;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;
extern u8 now_sub_sub_step;

void bunkaken_main0c(void) {
    s32 t9;
    s32 t4;
    s32 t5;
    s32 r;

    switch (now_sub_sub_step) {
    case 0:
        value_cache_find_or_load(16645, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out_step();
        break;
    case 2:
        r = get_CD_State2();
        if ((r & 1) == 0) {
            break;
        }
        r = dec_bg_init(1);
        indexed_byte_lsb_set(1, r);
        bunkaken_basyo_init3();
        now_serifu = 66;
        sub_sub_step_up();
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        serifu_run();
        break;
    case 5:
        sub_sub_step_up();
        break;
    case 6:
        t4 = now_step;
        ret_step = t4;
        t5 = now_sub_step;
        t9 = bunkaken_flags;
        ret_sub_step = t5;
        ret_sub_sub_step = now_sub_sub_step + 1;
        bunkaken_flags = (t9 | 2) & 0xFFE3;
        goto_sub_step(0x8D);
        break;
    case 7:
        common_disp_off = 1;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_sub_step(0x8C);
        break;
    case 8:
        device_cmd_send(0xBF);
        device_load_start(1, 0xC486, 0xC459, 0xD266, 0xD216, 0xD20E);
        date_x_taku_set(1, 0);
        bunkaken_basyo_init3();
        mes_base_y = 0xC0;
        message_window_init();
        yazirushi_off();
        date_name_set(0);
        common_disp_off = 0;
        sub_sub_step_up();
        break;
    case 9:
        r = device_load_poll();
        if (r != 1) {
            break;
        }
        device_cmd_send(0x200);
        dec_bg_reset();
        set_dec_bri(0);
        value_cache_find_or_load(16645, 1);
        sub_sub_step_up();
        break;
    case 10:
        r = get_CD_State2();
        if ((r & 1) == 0) {
            break;
        }
        r = dec_bg_init(1);
        indexed_byte_lsb_set(1, r);
        cd_read_start2(5, 0x801E8000, 18874);
        sub_sub_step_up();
        break;
    case 11:
        fade_in_step();
        break;
    case 12:
        r = get_CD_State2();
        if ((r & 1) != 0) {
            now_serifu = 67;
            bunkaken_common_bank_init();
        }
        break;
    case 13:
        serifu_run();
        break;
    case 14:
        fade_out_step();
        break;
    case 15:
        now_serifu = 82;
        sub_sub_step_up();
        break;
    case 16:
        serifu_run();
        break;
    case 17:
        bunkaken_flags = bunkaken_flags | 1;
        bunkaken_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
