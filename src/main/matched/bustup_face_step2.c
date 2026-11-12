/* Likely Symbol: bustup_face_step0 (from o.bin, offset +112 bytes from 0x80083560) */
/* Address: 0x800835D0 - 0x800835F8 (40 bytes) */
/* Region: IDO (>= 0x80081000) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void bustup_face_change(s32 arg0);
extern void sub_sub_step_up(void);

void bustup_face_step2(void) {
    bustup_face_change(2);
    sub_sub_step_up();
}
