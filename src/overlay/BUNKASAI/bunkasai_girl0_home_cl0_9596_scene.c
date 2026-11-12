/* @SymbolStart: 0x80151364 */
/* Address: 0x80151364 (prologue 0x8015136C) */
/* Size: 844 bytes (211 insns) */
/* @SymbolSize: 844 */
/* @RodataBase: 0x8015F208 */
/* @PostProc: swap_instructions --offset1 0x190 --offset2 0x194 */
/* @PostProc: swap_instructions --offset1 0x278 --offset2 0x27c */
/* @PostProc: swap_instructions --offset1 0x290 --offset2 0x28c */
/* @PostProc: swap_instructions --offset1 0x28c --offset2 0x288 */
/* @PostProc: swap_instructions --offset1 0x288 --offset2 0x284 */
/* @PostProc: swap_instructions --offset1 0x284 --offset2 0x280 */
/* @PostProc: patch_insn_reg_field 0x14c:rt:t1:v1 0x158:rt:t1:v1 0x15c:rt:t2:v1 0x168:rt:t2:v1 0x16c:rt:t3:v1 0x178:rt:t3:v1 0x17c:rt:t4:v1 0x184:rt:t4:v1 0x188:rt:a0:v0 0x18c:rt:v0:v1 0x190:rs:v0:v1 0x190:rt:v0:v1 0x194:rs:a0:v0 0x194:rt:a0:v0 0x19c:rs:a0:v0 0x1a0:rs:v0:v1 0x1a0:rt:t5:t2 0x1a8:rt:t5:t2 0x230:rt:a0:t6 0x234:rt:a0:v0 0x238:rs:a0:v0 0x238:rt:v1:v0 0x23c:rt:t6:t3 0x240:rd:t7:t4 0x240:rt:v1:v0 0x244:rd:t7:t4 0x244:rs:t7:t4 0x244:rt:v1:v0 0x248:rd:t7:t4 0x248:rt:t7:t4 0x24c:rd:t7:t4 0x24c:rs:t7:t4 0x24c:rt:v1:v0 0x250:rs:t6:t3 0x250:rt:t6:t3 0x254:rd:t7:t4 0x254:rt:t7:t4 0x258:rs:t7:t4 0x258:rt:t8:t5 0x25c:rs:t6:t3 0x25c:rt:t8:t5 0x26c:rt:v0:v1 0x274:rt:v0:v1 0x278:rt:t9:v0 0x27c:rs:a0:v0 0x27c:rt:v1:v0 0x280:rs:t9:t6 0x280:rt:t9:t6 0x284:rd:t0:t7 0x284:rt:v1:v0 0x288:rd:t0:t7 0x288:rs:t0:t7 0x288:rt:v1:v0 0x28c:rd:t0:t7 0x28c:rt:t0:t7 0x290:rd:t0:t7 0x290:rs:t0:t7 0x290:rt:v1:v0 0x294:rd:t0:t7 0x294:rt:t0:t7 0x298:rs:t0:t7 0x298:rt:t1:t8 0x29c:rs:t9:t6 0x29c:rt:t1:t8 0x2a4:rt:v0:v1 0x2ac:rt:v0:v1 0x2b4:rt:v0:v1 0x2bc:rs:v1:v0 0x2c0:rs:v0:v1 0x2c0:rt:t2:t0 0x2c8:rt:t2:t0 0x30c:rt:t3:v1 0x318:rt:t3:v1 */
#include "types.h"

extern void sub_sub_step_up(void);
extern s32 get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void game_reset(void);
extern void indexed_byte_lsb_set(s32, s32);
extern void value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);
extern void bustup_cd_read(s32);
extern void bustup_ptr_init(void);
extern void fade_up_main(void);
extern void fade_down_main(void);
extern void fade_in_step(void);
extern void fade_in(void);
extern void fade_out_step(void);
extern void fade_out(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32 get_g_zyotai_low(s32);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init(void);
extern void bunkasai_girl0_home_srf_bank_set(void);

extern s16 now_serifu;
extern u8  now_year;
extern u16 bukatu_rec;
extern u8  now_girl;
extern u8  now_sub_sub_step;

void bunkasai_girl0_home_cl0_9596_scene(void) {
    s32 sel;
    s32 msg;
    u8 loc;

    switch (now_sub_sub_step) {
    case 0:
        value_cache_find_or_load(16618, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out_step();
        break;
    case 2:
        if (get_CD_State2() & 1) {
            sel = dec_bg_init(1);
            indexed_byte_lsb_set(1, sel);
            bustup_cd_read(21578);
            sub_sub_step_up();
        }
        break;
    case 3:
        if (get_CD_State2() & 1) {
            bunkasai_basyo_init();
            bustup_ptr_init();
            now_girl = 0;
            zyotai_sprite_set_chk();
            cd_read_start2(24, 0x801E8000, 18896);
        }
        break;
    case 4:
        fade_in_step();
        break;
    case 5:
        if (get_CD_State2() & 1) {
            sel = get_g_zyotai_low(0);
            switch (sel) {

                break;
            case 0: case 1:
                msg = 2;
                now_serifu = msg;
                break;
            case 2:
                msg = 4;
                now_serifu = msg;
                break;
            case 3:
                msg = 6;
                now_serifu = msg;
                break;
            case 4:
                msg = 8;
                now_serifu = msg;
                break;
            }
            msg = now_serifu;
            loc = now_year;
            if (loc == 96) {
                now_serifu = msg + 12;
            }
            bunkasai_girl0_home_srf_bank_set();
        }
        break;
    case 6:
        fade_up_main();
        break;
    case 7:
        serifu_run();
        break;
    case 8:
        serifu_run();
        break;
    case 9:
        fade_out();
        now_serifu = 0;
        break;
    case 10:
        serifu_run();
        break;
    case 11:
        fade_in();
        break;
    case 12:
        sel = get_g_zyotai_low(0);
        if (sel == 4) {
            loc = now_year;
            if (!((u32)bukatu_rec < (u32)(loc * 100 - 9400))) {
                msg = 12;
            } else {
                msg = 13;
            }
        } else {
            loc = now_year;
            if (!((u32)bukatu_rec < (u32)(loc * 100 - 9400))) {
                msg = 10;
            } else {
                msg = 11;
            }
        }
        now_serifu = msg;
        if (loc == 96) {
            now_serifu = msg + 12;
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
        msg = 1;
        now_serifu = msg;
        sub_sub_step_up();
        break;
    case 17:
        serifu_run();
        break;
    case 18:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
