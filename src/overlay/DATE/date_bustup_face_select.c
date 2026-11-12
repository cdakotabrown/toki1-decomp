/* @SymbolStart: 0x80152DF8 */
/* @SymbolSize: 60 */
/* Function: date_bustup_face_select */
/* Overlay: DATE */
/* Address: 0x80152DF8 (cross-prologue head) / 0x80152E00 (body) */
/* Size: 60 bytes (15 instructions) */
#include "types.h"

extern s32 serifu_no;

extern s32 bustup_face_step0();
extern s32 bustup_face_step1();

void date_bustup_face_select(void)
{
    if (serifu_no == 0) {
        bustup_face_step0();
    } else {
        bustup_face_step1();
    }
}
