/* @SymbolStart: 0x80133604 */
/* @SymbolSize: 72 */
/* ending_bustup_face_step - ENDING overlay, Address: 0x80133604, Size: 72 bytes (18 instructions) */
#include "types.h"

extern u8 ending_save_girl;
extern void bustup_face_change();
extern void sub_sub_step_up();

void ending_bustup_face_step(void)
{
    u8 step = ending_save_girl;

    if (step == 12) {
        bustup_face_change(4);
    } else {
        bustup_face_change(0);
    }
    sub_sub_step_up();
}
