/* @SymbolStart: 0x8013DDDC */
/* @SymbolSize: 88 */
/* Function: date_basyo7_face_step */
/* Overlay: DATE */
/* Address: 0x8013DDDC (cross-prologue head) / 0x8013DDE4 (body) */
/* Size: 88 bytes (22 instructions) */
#include "types.h"

extern u8 date_pattern;

extern s32 bustup_face_step4(void);
extern void date_basyo7_kao_face_step(void);

void date_basyo7_face_step(void) {
    if (date_pattern == 0) {
        bustup_face_step4();
    } else if (date_pattern == 2) {
        bustup_face_step4();
    } else {
        date_basyo7_kao_face_step();
    }
}
