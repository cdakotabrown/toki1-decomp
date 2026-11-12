/* @SymbolStart: 0x80133E6C */
/* @SymbolSize: 60 */
/* shougatu_bustup_face_step2 - SHOUGATU overlay, Address: 0x80133E74, Size: 60 bytes (15 insns) */
#include "types.h"

extern void bustup_face_step3();
extern void bustup_face_step1();
extern s32 serifu_no;

void shougatu_bustup_face_step2(void)
{
    if (serifu_no != 0) {
        bustup_face_step3();
        return;
    }
    bustup_face_step1();
}
