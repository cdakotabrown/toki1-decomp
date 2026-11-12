/* Likely Symbol: bustup_face_step0 (from o.bin, offset +72 bytes from 0x80083560) */
/* Address: 0x800835A8 - 0x800835D0 (40 bytes) */
/* Region: IDO (>= 0x80081000) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void bustup_face_change(s32 arg0);
extern void sub_sub_step_up(void);

void bustup_face_step1(void) {
    bustup_face_change(1);
    sub_sub_step_up();
}
