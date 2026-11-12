/* @CompileFlags: -cckr */
/* @SymbolSize: 264 */
#include "types.h"

typedef struct {
    s32 f0;
    u8 *f4;
    s32 f8;
    s32 fc;
} EntryA;

extern u8 etc_model_init2_nobj_fmt[];
extern u8 coord_buf[16][80];
extern EntryA object[16];
extern s16 etc_roll_tx;
extern s16 etc_roll_ty;
extern s16 etc_roll_tz;
s16 etc_roll_rot[2] = { 0 };
extern s16 etc_roll_rot_2;

void etc_model_init2()
{
    s32 *p;
    s32 i;

    p = (s32 *)0x801F9004;
    GsMapModelingData(p);
    p++;
    printf(etc_model_init2_nobj_fmt, *p);
    p++;
    for (i = 0; i < 16; i++) {
        GsLinkObject4(p, &object[i], 0);
        object[i].f4 = coord_buf[i];
        object[i].f0 = 0x80000000;
    }
    GsLinkObject4(p, object, 0);
    object[0].f4 = coord_buf[0];
    object[0].f0 = 0;
    etc_roll_tx = -96;
    etc_roll_ty = 32;
    etc_roll_tz = -768;
    etc_roll_rot_2 = 0;
    etc_roll_rot[0] = 0;
    etc_roll_rot[1] = 0;
}
