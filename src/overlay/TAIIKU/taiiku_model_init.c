/* @CompileEnv: IDO52_COPYPROP_SITE=lod:2:deny IDO52_AS1_NO_PASS1_PROP=13 */
/* Address: 0x80140880, Size: 144 bytes (36 insns) */
#include "types.h"

typedef struct {
    s32 flag;
    u8 *rec;
    s32 pad[2];
} Slot;

extern Slot object[];
extern u8 coord_buf[];

extern void GsLinkObject4(s32 *a0, Slot *a1, s32 a2);
extern void GsMapModelingData(s32 *a0);

void taiiku_model_init(s32 *arg0)
{
    s32 *base;
    s32 *outer;
    s32 i;

    base = arg0 + 1;
    GsMapModelingData(base);
    outer = base;
    outer += 2;
    for (i = 0; i < 16; i++) {
        GsLinkObject4(outer, &object[i], 0);
        object[i].rec = coord_buf + i * 80;
        object[i].flag = 0x80000000;
    }
}
