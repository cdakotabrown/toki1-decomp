/* @CompileEnv: IDO52_COPYPROP_SITE=lod:4:deny,lod:5:deny IDO52_COPYPROP_VAR=-28 IDO52_AS1_NO_PASS1_PROP=36,37 IDO52_FORCE_WEB_COLOR=13:s2,40:s1 IDO52_AS1_TIE_REJECT=101 */
/* Address: 0x80147180, Size: 260 bytes (65 insns) */
#include "types.h"

extern u8 taco_obj[];
extern u8 taco_obj_1_attribute[];
extern u8 taco_coord[];
extern u8 taco_coord_1_flg[];

extern void GsLinkObject4(s32 *node, s32 *rec, s32 idx);
extern void GsMapModelingData(s32 *base);

void taco_obj_link2(s32 *arg0)
{
    s32 *base;
    u8 *node;
    u8 *callNode;
    u8 *rec;
    u8 *recEnd;
    u8 *rec2;
    s32 *otarg;
    s32 fill;
    s32 i;
    s32 count;

    base = arg0 + 1;
    GsMapModelingData(base);

    otarg = base + 2;
    node = taco_obj; rec = taco_coord; recEnd = taco_coord + 5120;
    fill = (s32)0x80000000;
    do {
        GsLinkObject4(otarg, (s32 *)node, 0);
        *(s32 *)(node + 4) = (s32)rec;
        rec += 80;
        node += 16;
        *(s32 *)(node - 16) = fill;
    } while ((u32)rec < (u32)recEnd);

    GsMapModelingData(base);

    otarg = base;
    count = otarg[1];
    otarg += 2;
    if (count != 0) {
        i = 0;
        node = taco_obj; callNode = taco_obj_1_attribute; rec2 = taco_coord_1_flg;
        do {
            GsLinkObject4(otarg, (s32 *)callNode, i);
            i += 1;
            *(s32 *)(node + 20) = (s32)rec2;
            rec2 += 80;
            node += 16;
            callNode += 16;
            *(s32 *)node = fill;
        } while ((u32)i < (u32)count);
    }
}
