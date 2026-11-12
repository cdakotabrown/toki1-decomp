/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:53 */
/* @SymbolStart: 0x80138E4C */
/* @SymbolSize: 364 */
/* @PostProc: swap_instructions --offset1 0x134 --offset2 0x138 */
/* @PostProc: patch_insn_reg_field 0x38:rt:a0:v1 0x50:rs:a0:v1 0x50:rt:a0:v1 0x58:rs:a0:v1 0x60:rt:t9:v0 0x64:rs:t9:v0 0x64:rt:t9:v0 0x68:rt:t0:t9 0x6c:rs:t9:v0 0x74:rt:t0:t9 0x80:rs:a0:v1 0x104:rt:t2:t1 0x108:rs:t2:t1 0x108:rt:t2:t1 0x10c:rt:t1:t0 0x110:rt:t3:t2 0x114:rs:t1:t0 0x114:rt:t2:t1 0x118:rs:t3:t2 0x120:rt:t6:t5 0x12c:rs:t3:t2 0x134:rt:t5:t3 0x13c:rs:t5:t3 0x144:rt:t5:t3 0x14c:rs:t6:t5 0x14c:rt:t6:t5 0x158:rt:t7:t6 0x15c:rt:t6:t5 0x164:rt:t7:t6 0x168:rs:t7:t6 */
/* shugaku_girl4_serifu_fade - SHUGAKU overlay, Address: 0x80138E54, Size: 356 bytes */
#include "types.h"

extern void fade_out_step(void);
extern void func_80138664(void);
extern void fade_up_main(void);
extern void fade_down_main(void);

extern u8  now_girl;
extern u8  shugaku_pick_serifu_no;
extern s32 step_counter;
extern u8  now_sub_sub_step;
extern u8  step_mode;
extern s16 now_serifu_shugaku;
extern s16 shugaku_place_serifu_no;

s32 shugaku_girl4_serifu_fade(void)
{
    s32 v1;
    s32 saved;
    s32 mode;
    s32 ret;

    if (now_girl != 4) goto lbl_other;
    if (shugaku_pick_serifu_no == 54) goto lbl_main;
lbl_other:
    fade_out_step();
    goto lbl_end;

lbl_main:
    v1 = step_counter;
    saved = (s32)now_sub_sub_step;
    if (v1 == 0) {
        if (step_mode == 0) {
            now_serifu_shugaku = 16;
        }
    }
    v1 = v1 + 1;
    if (step_mode == 0) {
        step_counter = v1;
    }
    mode = (s32)step_mode;
    if (mode == 0) {
        func_80138664();
    } else if (mode == 1) {
        fade_up_main();
    } else if (mode == 2) {
        func_80138664();
    } else if (mode == 3) {
        fade_down_main();
    } else {
        fade_out_step();
    }

    if (saved == (s32)now_sub_sub_step) goto lbl_end;
    if (mode < 4) {
        now_sub_sub_step = (u8)saved;
        ret = (mode + 1) & 0xFF;
        step_mode = (u8)ret;
        return ret;
    }
    now_serifu_shugaku = shugaku_place_serifu_no;
    ret = (mode + 1) & 0xFF;
    step_mode = (u8)ret;
    return ret;

lbl_end:
    ;
}
