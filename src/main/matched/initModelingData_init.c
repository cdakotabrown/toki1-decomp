/* Address: 0x8005928C */
/* Size: 144 bytes (36 instructions) */
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

void initModelingData_init(buf)
    s32 *buf;
{
    s32 *p;
    s32 i;

    p = buf + 1;
    GsMapModelingData(p);
    p++;
    p++;
    for (i = 0; i < 16; i++) {
        GsLinkObject4(p, &object[i], 0);
        object[i].b = &coord_buf[i];
        object[i].a = 0x80000000;
    }
}
