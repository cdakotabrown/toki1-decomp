/* @SymbolStart: 0x8013A224 */
/* @SymbolSize: 100 */
/* Address: 0x8013A224 (head) / 0x8013A22C (prologue) */
/* Size: 100 bytes (25 instructions) */
#include "types.h"

extern void bustup_face_change(s32 a0);
extern void sub_sub_step_up(void);

extern volatile u8 now_girl;
extern u8 menu_x;

void shugaku_bustup_face_set(void)
{
    if (now_girl == 2) {
        bustup_face_change(3);
    }
    if (now_girl == 7 && menu_x == 0) {
        bustup_face_change(4);
    }
    sub_sub_step_up();
}
