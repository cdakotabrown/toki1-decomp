/* @SymbolStart: 0x8013FA24 */
/* @RodataBase: 0x8015D214 */
/* Address: 0x8013FA2C */
/* Size: 552 bytes (138 insns) */
/* @SymbolSize: 560 */
/* @CompileEnv: IDO52_COPYPROP_VAR=-16 IDO52_AS1_TIE_TAKE=84 IDO52_AS1_TIE_REJECT=80 IDO52_UGEN_TEMP_REG=4:t0,5:t1,6:t3,7:t7,8:t8,9:t4,11:t9,12:t0,13:t2,14:t3,15:t4,16:t5,17:t7,18:t8,19:t9,20:t1,21:t2 IDO52_AS1_XBB_VOLATILE_OK=all IDO52_COPYPROP_SITE=lod:5:deny,lod:6:deny,lod:7:deny,lod:8:deny */
/* @PostProc: patch_insn_reg_field 0xdc:rt:t7:t6 0xec:rs:t7:t6 */
#include "types.h"

extern s32 get_g_zyotai_low(s32 a0);
extern void goto_sub_sub_step(s32 a0);
extern void sub_sub_step_up(void);
extern void serifu_run(void);
extern void fade_down_main(void);
extern void fade_out_step(void);
extern void bunkasai_sub_reset(void);
extern void game_reset(void);

extern s16 now_serifu;
extern volatile u8 now_year;
extern u16 basyo_pack;
extern s16 girl3_syoushin;
extern u8  girl3_flag;
extern u8  girl3_basyo;

extern u8 now_sub_sub_step;
extern u8 screen_w[];
void bunkasai_main4f(void) {
    s32 sel = now_sub_sub_step;

    s32 v0;
    s32 msg;
    u8 *p;

    switch (sel) {
    case 0:
        v0 = get_g_zyotai_low(3);
        if (v0 == 4) {
            goto_sub_sub_step(2);
            break;
        }
        v0 = get_g_zyotai_low(3);
        switch (v0) {
        case 0:
        case 1:
            msg = 80;
            break;
        case 2:
            msg = 82;
            break;
        case 3:
            msg = 84;
            break;
        default:
            msg = now_serifu;
            break;
        }
        p = screen_w;
        if ((girl3_syoushin & 3) == 0) {
            now_serifu = msg + 24;
        } else {
            if ((s32)(*(s32 *)(p + 624) << 29) >= 0) {
                girl3_flag |= 4;
                msg = (s16)(msg + 6);
            }
            now_serifu = msg;
            if (((u32)girl3_basyo >> 4) == 5) {
                now_serifu = msg + 12;
            }
        }
        sub_sub_step_up();
        break;
    case 1:
        serifu_run();
        break;
    case 2:
        fade_down_main();
        break;
    case 3:
        serifu_run();
        break;
    case 4:
        fade_out_step();
        break;
    case 5:
        now_serifu = 0;
        sub_sub_step_up();
        break;
    case 6:
        serifu_run();
        break;
    case 7:
        if (((u32)basyo_pack >> 12) != 0) goto L7_call;
        if (get_g_zyotai_low(3) == 4) goto L7_call;
        if (now_year == 95) {
            p = screen_w;
            p[625] |= 0x10;
        }
        p = screen_w;
        if (now_year == 96) {
            p[625] |= 0x20;
        }
L7_call:
        bunkasai_sub_reset();
        break;
    default:
        game_reset();
        break;
    }
}
