/* @CompileFlags: -Wc,-pic1 */
/* @SymbolStart: 0x8013F1A0 */
/* @SymbolSize: 492 */
/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=all IDO52_COPYPROP_VAR=-20 */
/* @PostProc: patch_stack_complete --old 48 --new 56 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x1C */
/* @PostProc: swap_instructions --offset1 0xD4 --offset2 0xD8 */
/* @PostProc: swap_instructions --offset1 0xD8 --offset2 0xDC */
/* @PostProc: swap_instructions --offset1 0xE8 --offset2 0xEC */
/* @PostProc: swap_instructions --offset1 0x160 --offset2 0x164 */
/* @PostProc: patch_insn_reg_field 0x10:rt:v0:v1 0x18:rs:v0:v1 0x18:rt:v0:v1 0x24:rs:t6:v1 0x24:rt:v0:t6 0x2c:rt:a1:a2 0x30:rs:a1:a2 0x30:rt:a1:a2 0x38:rs:a1:a2 0x94:rt:a1:a2 0x98:rs:a1:a2 0x98:rt:a1:a2 0x9c:rs:t3:v1 0x9c:rt:v0:t3 0xa0:rt:t6:t5 0xa4:rs:a1:a2 0xa8:rt:t4:v1 0xac:rs:t4:v1 0xac:rt:t4:v1 0xb4:rs:t4:v1 0xb4:rt:t5:t4 0xbc:rt:t5:t4 0xc0:rs:t6:t5 0xc0:rt:t6:t5 0xc8:rd:t7:t6 0xc8:rs:t6:t5 0xc8:rt:a1:a2 0xcc:rt:t7:t6 0xd4:rt:t8:a1 0xe4:rt:t8:a1 0xe8:rt:v1:a1 0xec:rt:a0:t7 0xf0:rs:v1:a1 0xf0:rt:v1:a1 0xf4:rs:a0:t7 0xf4:rt:t9:t8 0xf8:rs:a0:t7 0xf8:rt:t0:t9 0xfc:rs:t0:t9 0xfc:rt:t0:t9 0x100:rs:t9:t8 0x100:rt:t9:t8 0x104:rs:v1:a1 0x104:rt:t2:t1 0x108:rs:t2:t1 0x108:rt:t3:t2 0x10c:rd:t1:t0 0x10c:rs:t9:t8 0x10c:rt:t0:t9 0x110:rs:t1:t0 0x110:rt:t3:t2 0x114:rd:t5:t4 0x114:rt:v1:a1 0x118:rs:v1:a1 0x118:rt:t7:t6 0x11c:rs:t7:t6 0x11c:rt:t8:t6 0x120:rd:t5:t4 0x120:rs:t5:t4 0x120:rt:v1:a1 0x124:rd:t5:t4 0x124:rt:t5:t4 0x128:rd:t9:t7 0x128:rt:t8:t6 0x12c:rd:t9:t7 0x12c:rs:t9:t7 0x12c:rt:t8:t6 0x130:rt:a1:a2 0x138:rs:a1:a2 0x138:rt:a1:a2 0x140:rd:t4:t3 0x144:rd:t6:t5 0x144:rs:t4:t3 0x144:rt:t5:t4 0x148:rd:t0:t8 0x148:rs:t6:t5 0x148:rt:t9:t7 0x14c:rt:t0:t8 0x154:rs:a1:a2 0x158:rt:t4:t0 0x15c:rt:t2:v1 0x160:rs:t2:v1 0x160:rt:t2:v1 0x168:rt:t1:t9 0x16c:rs:v0:v1 0x16c:rt:t2:v0 0x174:rs:t1:t9 0x174:rt:t1:t9 0x180:rt:t1:t9 0x190:rt:v0:v1 0x194:rs:v0:v1 0x194:rt:v0:v1 0x19c:rs:a1:a2 0x1a0:rt:v1:v0 0x1a4:rs:v1:v0 0x1a4:rt:v1:v0 0x1a8:rt:t3:t1 0x1ac:rs:v0:v1 0x1ac:rt:v1:v0 0x1b4:rs:t3:t1 0x1b4:rt:t3:t1 0x1bc:rt:t3:t1 0x1cc:rt:v1:v0 0x1d8:rs:t4:t0 0x1d8:rt:t4:t0 0x1e0:rs:t4:t0 0x1e0:rt:t5:t2 0x1e8:rt:t5:t2 */
/* Function: date_machiawase_basyo_set */
/* Overlay: DATE */
/* Address: 0x8013F1A0 (cross-prologue head) / 0x8013F1B0 (body) */
/* Size: 492 bytes (123 instructions) */
#include "types.h"

extern s32 date_main0_serifu_run();
extern s32 get_g_zyotai_s();
extern s32 sub_sub_step_up();

extern u8 machiawase_mode;
extern volatile u8 machiawase_basyo;
extern u8 machiawase_basyo2;
extern u8 machiawase_basyo3;
extern s32 serifu_no;
extern u8 now_sub_sub_step;
extern s16 date_now_serifu_machi;
extern u8 date_machi_done;
extern s16 date_now_serifu0;
extern u8 date_machi_mode;
extern u8 now_girl;
extern u8 rec_idx;
extern u8 now_basyo;
extern volatile s16 now_serifu;

void date_machiawase_basyo_set(void)
{

    s32 saved;
    s32 cur;
    u32 band;
    s32 mode;
    s32 t;

    if (machiawase_mode == machiawase_basyo && serifu_no == 0) {
        saved = now_sub_sub_step;
        date_main0_serifu_run();
        cur = now_sub_sub_step;
        if (saved != cur) {
            date_machi_done = 1;
            date_now_serifu_machi = (s16)(date_now_serifu_machi + 1);
            now_sub_sub_step = (u8)(cur + 12);
        }
        return;
    }

    if (machiawase_mode != machiawase_basyo && serifu_no == 0) {
        now_sub_sub_step = (u8)(now_sub_sub_step + 30);
        return;
    }

    date_now_serifu0 = (s16)(date_now_serifu0 + serifu_no);
    date_machi_mode = 2;
    band = (u32)get_g_zyotai_s(now_girl) & 0x7FU;
    mode = date_machi_mode;
    date_now_serifu_machi = (s16)(((band >= 2) + (band >= 3)) * ((mode == 0) + 1)
                       + mode * 6 - ((mode ^ 2) == 0) * 3);

    if (serifu_no == 1) {
        if (machiawase_mode == machiawase_basyo2) {
            now_basyo = machiawase_basyo;
        } else {
            now_basyo = machiawase_basyo2;
        }
        now_serifu = 0;
    }
    t = machiawase_mode;
    if (serifu_no == 2) {
        if (t == machiawase_basyo3) {

            now_basyo = machiawase_basyo;
            now_serifu = 0;
        } else {
            now_basyo = machiawase_basyo3;
            now_serifu = 0;
        }
    }

    rec_idx = (u8)(rec_idx - 1);
    sub_sub_step_up();
}
