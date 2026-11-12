/* @SymbolStart: 0x80139444 */
/* @SymbolSize: 112 */
#include "types.h"

extern s32 serifu_no;

extern void bustup_face_step4(void);
extern void bustup_face_step3(void);
extern void sub_sub_step_up(void);

extern u8 now_girl;

void shugaku_sel_bustup_face(void)
{
    s32 param_t6 = serifu_no;

    s32 v0;

    if (param_t6 == 0) goto lbl_8013949C;
    v0 = (s32)now_girl;
    if (v0 == 2) goto lbl_8013947C;
    if (v0 == 7) goto lbl_8013947C;
    if (v0 != 9) goto lbl_8013948C;
lbl_8013947C:
    bustup_face_step4();
    goto lbl_801394A8;
lbl_8013948C:
    bustup_face_step3();
    goto lbl_801394A8;
lbl_8013949C:
    sub_sub_step_up();
lbl_801394A8:
    return;
}
