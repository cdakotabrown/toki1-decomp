/* @SymbolStart: 0x8013E69C */
/* @SymbolSize: 96 */
/* Address: 0x8013E69C (cross-prologue head) / 0x8013E6A4 (body) */
/* Size: 96 bytes (24 instructions) */
#include "types.h"

extern s32 serifu_no;
extern u8 now_girl;

extern void sub_sub_step_up(void);
extern void bustup_face_step0(void);
extern void bustup_face_step1(void);

void date_basyo18_face_step(void) {
    if (serifu_no == 0) {
        if (now_girl == 7) {
            bustup_face_step1();
            return;
        }
        bustup_face_step0();
        return;
    }
    sub_sub_step_up();
}
