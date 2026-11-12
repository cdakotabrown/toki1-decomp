/* @SymbolStart: 0x8013E214 */
/* @SymbolSize: 132 */
/* Function: date_basyo17_girl5_serifu1b */
/* Overlay: DATE */
/* Address: 0x8013E214 (cross-prologue head) / 0x8013E21C (body) */
/* Size: 132 bytes (33 instructions) */
#include "types.h"

extern void bustup_face_change(s32 a);
extern void serifu_run(void);
extern void sub_sub_step_up(void);

extern u8 now_girl;
extern u8 date_pattern;
extern s32 serifu_no;
extern s32 step_counter;

void date_basyo17_girl5_serifu1b(void)
{
    s32 old;
    s32 isFirst;

    if (now_girl == 5 && date_pattern == 0 && serifu_no == 2) {
        old = step_counter;
        isFirst = (u32)old < 1U;
        step_counter = old + 1;
        if (isFirst != 0) {
            bustup_face_change(3);
        }
        serifu_run();
    } else {
        sub_sub_step_up();
    }
}
