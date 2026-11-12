/* Address: 0x800835F8 */
/* Size: 40 bytes (10 instructions) */
/* Region: IDO (>= 0x80081000) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern void bustup_face_change(s32 arg0);
extern void sub_sub_step_up(void);

void bustup_face_step3(void) {
    bustup_face_change(3);
    sub_sub_step_up();
}
