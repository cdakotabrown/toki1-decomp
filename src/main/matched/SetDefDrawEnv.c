/* Address: 0x800A345C */
/* Size: 160 bytes (40 instructions) */
/* Region: Library (0x80086000+) */
/* @CompileFlags: -Wc,-pic0 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=2:v1,7:t0,23:t1,28:t2 IDO52_FORCE_WEB_COLOR_NOSPLIT=7,23,28 IDO52_AS1_TIE_TAKE=5,61,63 IDO52_AS1_TIE_REJECT=53,56 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
#include "types.h"

typedef struct {
    s16 field_00;
    s16 field_02;
    s16 field_04;
    s16 field_06;
    s16 field_08;
    s16 field_0A;
    s16 field_0C;
    s16 field_0E;
    s16 field_10;
    s16 field_12;
    s16 field_14;
    u8  field_16;
    u8  field_17;
    u8  field_18;
    u8  field_19;
    u8  field_1A;
    u8  field_1B;
} Struct_345C;

extern s16 GetTPage(s32, s32, s32, s32);

Struct_345C *SetDefDrawEnv(Struct_345C *arg0, s32 arg1, s32 arg2, s32 w, s32 arg4) {
    volatile Struct_345C *ptr;
    s32 x;
    s32 y;
    s32 h;

    ptr = (volatile Struct_345C *)arg0;
    h = arg4;
    ptr->field_04 = w;
    ptr->field_0C = 0;
    ptr->field_0E = 0;
    ptr->field_10 = 0;
    ptr->field_12 = 0;
    ptr->field_19 = 0;
    ptr->field_1A = 0;
    ptr->field_1B = 0;
    ptr->field_06 = h;
    x = arg1;
    y = arg2;
    ptr->field_02 = y;
    ptr->field_00 = x;
    ptr->field_16 = 1;
    if (h == 480) {
        ptr->field_17 = 0;
    } else {
        ptr->field_17 = 1;
    }
    ptr->field_08 = x;
    ptr->field_0A = y;
    ptr->field_14 = GetTPage(0, 0, 640, 0);
    ptr->field_18 = 0;
    return (Struct_345C *)ptr;
}
