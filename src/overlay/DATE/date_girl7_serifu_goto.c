/* @SymbolSize: 140 */
/* @PostProc: patch_insn_reg_field 0x38:rt:t7:t8 0x3c:rs:t7:t8 0x44:rt:t8:t9 0x48:rs:t8:t9 0x48:rt:t8:t9 0x50:rs:t8:t9 0x54:rt:t9:t0 0x5c:rt:t9:t0 0x68:rt:t0:t1 0x6c:rs:t0:t1 0x6c:rt:t0:t1 0x74:rs:t0:t1 0x74:rt:t1:t2 0x78:rt:t1:t2 */
/* Function: date_girl7_serifu_goto */
/* Overlay: DATE */
/* Address: 0x80140A08 */
/* Size: 140 bytes (35 instructions) */
#include "types.h"

extern s32 get_g_zyotai_s(s32 a);
extern void goto_sub_sub_step(s32 a);
extern void sub_sub_step_up(void);

extern u8 girl_no;
extern u8 date_machi_mode;
extern s16 date_now_serifu0;
extern u8 now_basyo;

s32 date_girl7_serifu_goto(void)
{
    s32 r;

    r = get_g_zyotai_s(girl_no);
    if (girl_no != 7) {
        goto tail;
    }
    if ((u32)(r & 0x7F) < 3U) {
        goto tail;
    }
    if (date_machi_mode != 2) {
        goto tail;
    }
    date_now_serifu0 = 8;
    goto_sub_sub_step(52);
    now_basyo = now_basyo + 50;
    return 0;

tail:
    sub_sub_step_up();

}
