/* @SymbolStart: 0x8013E6FC */
/* @SymbolSize: 64 */
/* Function: date_basyo2_face_step1d */
/* Overlay: DATE */
/* Address: 0x8013E6FC (cross-prologue head) / 0x8013E704 (body) */
/* Size: 64 bytes (16 instructions) */
#include "types.h"

extern u8 now_girl;

extern s32 bustup_face_step0(void);
extern s32 bustup_face_step1(void);

s32 date_basyo2_face_step1d(void)
{
    if (now_girl == 9) {
        return bustup_face_step0();
    }
    return bustup_face_step1();
}
