#include "types.h"

extern s32 set_kanji_string(s32 a0, s32 a1, s32 a2, s32 a3, s32 sp16);
extern s32 k_disp_start(s32 a0);
extern void sub_sub_step_up(void);

extern s32 shugaku_dantai_disp_str0;

void shugaku_dantai_disp(void)
{
    s32 v0;
    v0 = set_kanji_string(-128, 48, 2, (s32)&shugaku_dantai_disp_str0, 0);
    k_disp_start(v0);
    sub_sub_step_up();
}
