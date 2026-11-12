/* @SymbolSize: 252 */
/* @PostProc: swap_instructions --offset1 0x60 --offset2 0x68 */
/* etc_model_init - ETC overlay, Address: 0x8013F900, Size: 252 bytes (63 insns) */
#include "types.h"

extern void GsLinkObject4();
extern void GsMapModelingData();
extern void printf();

extern u8 etc_model_init_nobj_fmt[];
extern u8 coord_buf[];
extern u8 daen_obj[];
extern s32 etc_model_pos[];
extern s32 etc_model_roty;

typedef struct {
    s32 f0;
    s32 f4;
    s32 f8;
    s32 fc;
} EntryA;

extern EntryA object[];

void etc_model_init(void)
{
    EntryA *p;
    u8 *q;
    s32 *r;

    r = (s32 *)0x801FA804;
    GsMapModelingData(r);
    r = r + 1;
    printf(etc_model_init_nobj_fmt, *r);
    r = r + 1;

    p = object;
    q = coord_buf;
    do {
        GsLinkObject4(r, p, 0);
        p->f4 = (s32)q;
        q += 80;
        p += 1;
        p[-1].f0 = (s32)0x80000000;
    } while (q != daen_obj);
    GsLinkObject4(r, object, 0);
    object[0].f4 = (s32)coord_buf;
    object[0].f0 = 0;
    etc_model_pos[0] = 159;
    etc_model_pos[1] = -120;
    etc_model_pos[2] = 0;
    etc_model_roty = 3072;
}
