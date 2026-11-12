/* Address: 0x80061B3C */
/* Size: 296 bytes (74 instructions) */
/* Region: Game (IDO 5.2) */
/* @CompileFlags: -Wc,-pic1 -cckr */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    s32 a;
    void *b;
    s32 c;
    s32 d;
} Rec;

typedef struct {
    u8 x[80];
} Buf;

extern Rec object[];
extern Buf coord_buf[];

void title_init_ModelingData()
{
    s32 *p;
    s32 i;
    s32 r;

    p = (s32 *)0x801F0004;
    GsMapModelingData(p);
    p++;
    if (*p == 13U) {
        p++;
        for (i = 0; i < 16; i++) {
            GsLinkObject4(p, &object[i], 0);
            object[i].b = &coord_buf[i];
            object[i].a = 0;
        }
        r = rand();
        for (i = 0; i < 13; i++) {
            GsLinkObject4(p, &object[i], (r + i) % 13);
            object[i].b = &coord_buf[i];
            object[i].a = 0;
        }
    }
}
