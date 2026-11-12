/* @SymbolStart: 0x8013DFC4 */
/* @SymbolSize: 100 */
/* Address: 0x8013DFC4 (cross-prologue head) / 8013DFCC (body) */
/* Size: 100 bytes (25 instructions) */
#include "types.h"

extern u8 now_girl;
extern s32 serifu_no;
extern u8 date_pattern;

extern void sub_sub_step_up(void);
extern void bustup_face_step4(void);

void date_basyo9_girl7_bustup_step4(void) {
    if (now_girl == 7) {
        if (serifu_no == 0) {
            if (date_pattern == 1) {
                bustup_face_step4();
                return;
            }
        }
    }
    sub_sub_step_up();
}
