/* @SymbolStart: 0x80132C64 */
/* @SymbolSize: 176 */
/* shugaku_birth_sel_branch -- SHUGAKU overlay, Address: 0x80132C6C, Size: 168 bytes */
/* cfe, and `@CompileEnv: IDO52_CONST_INREG_VALUES=56` to keep 56 in a */
#include "types.h"

extern volatile u8 now_girl;
extern u8  screen_w[];
extern u8  now_sub_sub_step;
extern s16 shugaku_birth_now_serifu;

extern void param_clamp(void);
extern void sub_sub_step_up(void);

extern s32 serifu_no;

void shugaku_birth_sel_branch(void)
{
    u8 *q;

    if (serifu_no != 0) {
        q = screen_w + (s32)now_girl * 56;
        *(s16 *)(q + 450) = (s16)(*(s16 *)(q + 450) - 1);
        q = screen_w + (s32)now_girl * 56;
        *(s16 *)(q + 454) = (s16)(*(s16 *)(q + 454) + 5);
        param_clamp();
        now_sub_sub_step = (u8)(now_sub_sub_step + 27);
    } else {
        shugaku_birth_now_serifu = (s16)(shugaku_birth_now_serifu + 1);
        sub_sub_step_up();
    }
}
