/* @SymbolStart: 0x80138170 */
/* @SymbolSize: 52 */
/* date2_face2_step - DATE2 overlay, Address: 0x80138170, Size: 52 bytes (13 insns) */
#include "types.h"

extern s32 serifu_no;

void bustup_face_change(s32 arg0);
void sub_sub_step_up(void);

void date2_face2_step(void)
{
    if (serifu_no != 0) {
        bustup_face_change(2);
    }
    sub_sub_step_up();
}
