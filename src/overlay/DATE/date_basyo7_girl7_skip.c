/* @SymbolStart: 0x8013DD50 */
/* @SymbolSize: 140 */
/* Function: date_basyo7_girl7_skip */
/* Overlay: DATE */
/* Address: 0x8013DD50 (cross-prologue head) / 0x8013DD58 (body) */
/* Size: 140 bytes (35 instructions) */
#include "types.h"

extern void bustup_face_step4(void);
extern void sub_sub_step_up(void);

extern u8 now_girl;
extern s32 serifu_no;
extern u8 date_pattern;

s32 date_basyo7_girl7_skip(void)
{
    s32 v;

    if (now_girl == 7 && serifu_no == 1) {
        v = date_pattern;
        if (v == 2 || v == 0) {
            bustup_face_step4();
            return 0;
        }
    }
    sub_sub_step_up();
    sub_sub_step_up();
    sub_sub_step_up();
    sub_sub_step_up();
    sub_sub_step_up();
}
