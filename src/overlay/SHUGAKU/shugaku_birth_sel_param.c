/* @SymbolStart: 0x801330E8 */
/* @SymbolSize: 440 */
/* @CompileEnv: IDO52_AS1_NO_LA_PULL=0, */
/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=0, */
/* @CompileEnv: (Hook 27) */
/* @CompileEnv: IDO52_AS1_XBB_VOLATILE_OK=0,      (Hook 27) */
/* @CompileEnv: IDO52_AS1_NO_LA_PULL=0,           (Hook 21) */
/* The old `@PostProc: swap_instructions --offset1 0x38 --offset2 0x40` is */
/* shugaku_birth_sel_param - SHUGAKU overlay, Address: 0x801330F0, Size: 432 bytes */
#include "types.h"

extern void bustup_face_change(s32 a0);
extern void param_clamp(void);
extern void sub_sub_step_up(void);

extern s32 serifu_no;
extern volatile u8 now_girl;
extern u8 screen_w[];

void shugaku_birth_sel_param(void)
{
    s32 v0;
    u8 *q;
    u8 c;

    c = (u8)(serifu_no + 1);
    bustup_face_change((s32)c);
    v0 = serifu_no;
    if (v0 != 0) goto lbl_one;

    q = screen_w + (s32)now_girl * 56;
    *(s16 *)(q + 446) = (s16)(*(s16 *)(q + 446) + 3);
    q = screen_w + (s32)now_girl * 56;
    *(s16 *)(q + 450) = (s16)(*(s16 *)(q + 450) + 2);
    q = screen_w + (s32)now_girl * 56;
    *(s16 *)(q + 454) = (s16)(*(s16 *)(q + 454) - 15);
    goto lbl_tail;

lbl_one:
    if (v0 != 1) goto lbl_other;
    q = screen_w + (s32)now_girl * 56;
    *(s16 *)(q + 446) = (s16)(*(s16 *)(q + 446) + 2);
    q = screen_w + (s32)now_girl * 56;
    *(s16 *)(q + 450) = (s16)(*(s16 *)(q + 450) + 1);
    q = screen_w + (s32)now_girl * 56;
    *(s16 *)(q + 454) = (s16)(*(s16 *)(q + 454) - 10);
    goto lbl_tail;

lbl_other:
    q = screen_w + (s32)now_girl * 56;
    *(s16 *)(q + 450) = (s16)(*(s16 *)(q + 450) + 1);
    q = screen_w + (s32)now_girl * 56;
    *(s16 *)(q + 454) = (s16)(*(s16 *)(q + 454) + 10);

lbl_tail:
    param_clamp();
    sub_sub_step_up();
}
