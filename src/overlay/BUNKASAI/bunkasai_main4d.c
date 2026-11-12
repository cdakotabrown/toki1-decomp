/* @SymbolStart: 0x8013F5D0 */
/* Address: 0x8013F5D8 */
/* Size: 1092 bytes (273 insns) */
/* @SymbolSize: 1100 */
/* @RodataBase: 0x8015D1AC */
/* @PostProc: patch_insn_reg_field 0xd0:rt:t0:v0 0xd4:rt:t9:v0 0xe0:rt:t9:v0 0xe8:rt:t0:v0 0x11c:rt:t1:t9 0x124:rt:t2:t0 0x128:rs:t2:t0 0x128:rt:t2:t0 0x12c:rt:t1:t9 0x134:rt:t2:t0 0x13c:rt:t3:t1 0x140:rt:t3:t1 0x16c:rt:t4:t2 0x178:rt:t4:t2 0x17c:rt:t5:t3 0x188:rt:t5:t3 0x1c0:rt:t6:t4 0x1c4:rs:t6:t4 0x1dc:rd:t7:t5 0x1e4:rt:t7:t5 0x1e8:rt:t7:t5 0x1f0:rs:t7:t5 0x1f8:rt:t8:v0 0x1fc:rs:t8:v0 0x1fc:rt:t8:v0 0x204:rd:t9:t6 0x204:rt:t8:v0 0x208:rd:t9:t6 0x208:rs:t9:t6 0x208:rt:t8:v0 0x20c:rs:t9:t6 0x20c:rt:t0:t8 0x214:rt:t0:t8 0x218:rt:t1:v0 0x21c:rs:t1:v0 0x21c:rt:t1:v0 0x224:rd:t2:t9 0x224:rt:t1:v0 0x228:rd:t2:t9 0x228:rs:t2:t9 0x228:rt:t1:v0 0x22c:rs:t2:t9 0x22c:rt:t3:t1 0x234:rt:t3:t1 0x238:rt:t4:v0 0x23c:rs:t4:v0 0x23c:rt:t4:v0 0x244:rd:t5:t2 0x244:rt:t4:v0 0x248:rd:t5:t2 0x248:rs:t5:t2 0x248:rt:t4:v0 0x24c:rs:t5:t2 0x24c:rt:t6:t4 0x254:rt:t6:t4 0x258:rt:t7:v0 0x260:rt:t7:v0 0x294:rt:t8:t5 0x2a8:rs:t8:t5 0x2a8:rt:t8:t5 0x2b0:rs:t8:t5 0x2b0:rt:t9:t6 0x2b4:rs:t9:t6 0x334:rt:t0:t7 0x374:rs:t0:t7 0x374:rt:t0:t7 0x378:rt:t4:t2 0x37c:rd:t1:t9 0x37c:rt:t0:t7 0x380:rs:t1:t9 0x384:rt:t6:t5 0x388:rs:t4:t2 0x388:rt:t4:t3 0x390:rd:t2:t0 0x398:rs:t4:t3 0x398:rt:t5:t4 0x39c:rt:t2:t0 0x3a0:rt:t5:t4 0x3a4:rs:t6:t5 0x3a4:rt:t6:t5 0x3ac:rd:t7:t6 0x3ac:rt:t6:t5 0x3b0:rd:t7:t6 0x3b0:rs:t7:t6 0x3b0:rt:t6:t5 0x3b4:rd:t7:t6 0x3b4:rt:t7:t6 0x3b8:rs:t7:v0 0x3b8:rt:v0:t6 */
#include "types.h"

extern u8 now_sub_sub_step;
extern void sub_sub_step_up(void);
extern void goto_step(s32);
extern void goto_sub_sub_step(s32);
extern s32 get_CD_State2(void);
extern void cd_read_start2(s32, s32, s32);
extern void game_reset(void);
extern void indexed_byte_lsb_set(s32, s32);
extern void value_cache_find_or_load(s32, s32);
extern s32 dec_bg_init(s32);
extern void bustup_ptr_init(void);
extern void fade_down_main(void);
extern void fade_in_step(void);
extern void fade_in(void);
extern void fade_out_step(void);
extern void zyotai_sprite_set_chk(void);
extern void serifu_run(void);
extern s32 get_g_zyotai_low(s32);
extern void bustup_load_step(s32);
extern void bunkasai_girl3_clubs4_5_srf_bank_set(void);
extern void bunkasai_sub_reset(void);
extern void bunkasai_basyo_init2(void);

extern s16 now_serifu;
extern u8  now_year;
extern s16 girl3_syoushin;
extern union { s32 w; u8 b; } girl3_flag;
extern u8  now_girl;
extern u8  now_sub_step;
extern s32 bunka_sd_kind;
extern s32 bunka_ret_sub_step;
extern s32 bunka_ret_sub_sub_step;
extern s16 mes_base_y;

void bunkasai_main4d(void) {
    s32 v0;
    s32 msg;
    s32 sel;
    switch (now_sub_sub_step) {
    case 0:
        value_cache_find_or_load(16628, 1);
        sub_sub_step_up();
        break;
    case 1:
        fade_out_step();
        break;
    case 2:
        if (get_CD_State2() & 1) {
            bunkasai_basyo_init2();
            v0 = dec_bg_init(1);
            indexed_byte_lsb_set(1, v0);
            sub_sub_step_up();
        }
        break;
    case 3:
        fade_in_step();
        break;
    case 4:
        if (now_year == 95) {
            sel = 46;
            now_serifu = sel;
        } else {
            sel = 47;
            now_serifu = sel;
        }
        sub_sub_step_up();
        break;
    case 5:
        serifu_run();
        break;
    case 6:
        fade_out_step();
        break;
    case 7:
        bunka_sd_kind = 2;
        bunka_ret_sub_step = now_sub_step;
        bunka_ret_sub_sub_step = now_sub_sub_step + 1;
        goto_step(128);
        break;
    case 8:
        bustup_load_step(22703);
        break;
    case 9:
        bunkasai_basyo_init2();
        mes_base_y = 192;
        bustup_ptr_init();
        now_girl = 3;
        zyotai_sprite_set_chk();
        cd_read_start2(15, 0x801E8000, 19181);
        break;
    case 10:
        fade_in();
        break;
    case 11:
        if (get_CD_State2() & 1) {
            v0 = get_g_zyotai_low(3);
            switch (v0) {

                break;
            case 0: case 1:
                now_serifu = now_year * 3 - 232;
                break;
            case 2:
                now_serifu = now_year * 3 - 231;
                break;
            case 3:
                now_serifu = now_year * 3 - 230;
                break;
            case 4:
                now_serifu = 62;
                break;
            }
            bunkasai_girl3_clubs4_5_srf_bank_set();
        }
        break;
    case 12:
        serifu_run();
        break;
    case 13:
        v0 = get_g_zyotai_low(3);
        if (v0 == 4) {
            goto_sub_sub_step(15);
            break;
        }

        if ((girl3_syoushin & 3) == 0) {
            sel = get_g_zyotai_low(3);
            switch (sel) {
            case 0:
            case 1:
                sel = 24;
                goto stA;
            case 2:
                sel = 26;
                goto stB;
            case 3:
                sel = 28;
                goto stC;
            default:
                goto stD;
            stA:
                now_serifu = sel;
                goto stD;
            stB:
                now_serifu = sel;
                goto stD;
            stC:
                now_serifu = sel;
                goto stD;
            stD:
                ;
            }
            sub_sub_step_up();
        } else {
            v0 = get_g_zyotai_low(3);
            switch (v0) {
            case 0:
            case 1:
                msg = 64;
                break;
            case 2:
                msg = 66;
                break;
            case 3:
                msg = 68;
                break;
            default:
                msg = now_serifu;
                break;
            }
            if ((girl3_flag.w << 29) >= 0) {
                msg = (s16)(msg + 6);
                girl3_flag.b |= 4;
            }
            msg = msg + now_year * 12;
            now_serifu = msg - 1140;
            sub_sub_step_up();
        }
        break;
    case 14:
        serifu_run();
        break;
    case 15:
        fade_down_main();
        break;
    case 16:
        serifu_run();
        break;
    case 17:
        fade_out_step();
        break;
    case 18:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 19:
        serifu_run();
        break;
    case 20:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
