/* Address: 0x80062140 */
/* Size: 128 bytes (32 instructions) */
/* Compiler: IDO 5.2 */
/* Region: GCC (0x80040000-0x80080000) */
#include "types.h"

extern void GsSetProjection(s32);
extern void GsSetRefView2(void*);
extern void GsSetNearClip(s32);
extern void GsSetFarClip(s32);

typedef struct {
    s32 field_00;
    s32 field_04;
    s32 field_08;
    s32 field_0C;
    s32 field_10;
    s32 field_14;
    s32 field_18;
    s32 field_1C;
} InitStruct;

extern InitStruct ref_view;

void func_80062140(void) {
    GsSetProjection(1000);

    ref_view.field_00 = 0;
    ref_view.field_04 = 0;
    ref_view.field_08 = 1000;
    ref_view.field_0C = 0;
    ref_view.field_10 = 0;
    ref_view.field_14 = 0;
    ref_view.field_18 = 0;
    ref_view.field_1C = 0;

    GsSetRefView2(&ref_view);
    GsSetNearClip(100);
    GsSetFarClip(0x10000);
}
