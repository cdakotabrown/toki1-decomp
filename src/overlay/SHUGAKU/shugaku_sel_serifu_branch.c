/* @SymbolStart: 0x801392B0 */
/* @PostProc: patch_insn_reg_field 0x30:rt:v0:t8 0x34:rt:v0:t8 0x40:rt:v0:t8 0x5c:rt:t7:t9 0x60:rs:t7:t9 0x60:rt:t7:t9 0x68:rs:t7:t9 0x74:rt:t1:t3 0x78:rd:t8:t0 0x7c:rt:t8:t0 0x80:rt:t9:t1 0x88:rs:t9:t1 0x88:rt:t0:t2 0x8c:rt:t0:t2 0x90:rs:t1:t3 0x90:rt:t1:t3 0x98:rs:t1:t3 0x9c:rd:t2:t4 0xa0:rt:t2:t4 0xa4:rt:t3:t5 0xac:rs:t3:t5 0xac:rt:t4:t6 0xb4:rt:t4:t6 */
/* @SymbolSize: 192 */
/* shugaku_sel_serifu_branch - SHUGAKU overlay, Address: 0x801392B8, Size: 184 bytes */
#include "types.h"

extern void param_clamp(void);
extern void sub_sub_step_up(void);

extern s32 serifu_no;
extern u8  shugaku_place;
extern s16 shugaku_place_serifu_no;
extern s16 now_serifu_shugaku;
extern volatile u8 now_girl;
extern u8  screen_w[];

void shugaku_sel_serifu_branch(void)
{
    s32 v0;
    u8 *q;

    if (serifu_no != 0) goto lbl_801392FC;
    v0 = (s32)shugaku_place + 26;
    shugaku_place_serifu_no = (s16)v0;
    now_serifu_shugaku = (s16)v0;
    sub_sub_step_up();
    sub_sub_step_up();
    sub_sub_step_up();
    goto lbl_80139358;

lbl_801392FC:
    q = screen_w + (s32)now_girl * 56;
    *(s16 *)(q + 446) = (s16)(*(s16 *)(q + 446) - 1);
    q = screen_w + (s32)now_girl * 56;
    *(s16 *)(q + 450) = (s16)(*(s16 *)(q + 450) - 2);
    param_clamp();

lbl_80139358:
    sub_sub_step_up();
}
