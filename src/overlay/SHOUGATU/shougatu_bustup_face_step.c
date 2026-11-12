/* @SymbolStart: 0x80132E0C */
/* @SymbolSize: 128 */
/* shougatu_bustup_face_step - SHOUGATU overlay, Address: 0x80132E14, Size: 120 bytes (30 insns) */
#include "types.h"

extern void sub_sub_step_up();
extern void bustup_face_step3();
extern void bustup_face_step4();
extern u8 now_girl;
extern s32 serifu_no;

void shougatu_bustup_face_step(void)
{
    s32 v0;

    if (serifu_no != 0) {
        v0 = now_girl;
        if (v0 == 2 || v0 == 7 || v0 == 8 || v0 == 9 || v0 == 10) {
            bustup_face_step4();
        } else {
            bustup_face_step3();
        }
        return;
    }
    sub_sub_step_up();
}
