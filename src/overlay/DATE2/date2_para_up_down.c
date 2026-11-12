/* @SymbolStart: 0x80137B94 */
/* @SymbolSize: 440   <-- the .sym entry is an OVER-COUNTED BLOB, see below */
/* @CompileEnv: IDO52_AS1_NO_LA_PULL=0 IDO52_AS1_XBB_VOLATILE_OK=0 */
#include "types.h"

extern void bustup_face_change(s32 arg0);
extern void param_clamp(void);
extern void sub_sub_step_up(void);

extern s32 serifu_no;
extern volatile u8 now_girl;
extern u8 screen_w[];

void date2_para_up_down(void)
{
    s32 arg0 = serifu_no;
    s16 *p;
    s32 v;

    bustup_face_change((s32)(u8)(arg0 + 1));

    v = serifu_no;
    if (v != 0) {
        goto L1;
    }
    p = (s16 *)(screen_w + now_girl * 56);
    p[223] = (s16)(p[223] + 3);
    p = (s16 *)(screen_w + now_girl * 56);
    p[225] = (s16)(p[225] + 2);
    p = (s16 *)(screen_w + now_girl * 56);
    p[227] = (s16)(p[227] - 15);
    goto END;

L1:
    if (v != 1) {
        goto L2;
    }
    p = (s16 *)(screen_w + now_girl * 56);
    p[223] = (s16)(p[223] + 2);
    p = (s16 *)(screen_w + now_girl * 56);
    p[225] = (s16)(p[225] + 1);
    p = (s16 *)(screen_w + now_girl * 56);
    p[227] = (s16)(p[227] - 10);
    goto END;

L2:
    p = (s16 *)(screen_w + now_girl * 56);
    p[225] = (s16)(p[225] + 1);
    p = (s16 *)(screen_w + now_girl * 56);
    p[227] = (s16)(p[227] + 10);

END:
    param_clamp();
    sub_sub_step_up();
}
