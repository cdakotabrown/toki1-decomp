/* Address: 0x80146E24 (true start 0x80146DEC, cross-prologue hoist) */
/* @SymbolStart: 0x80146DEC */
/* @SymbolSize: 312 */
#include "types.h"

typedef struct {
    s32 a;
    s32 b;
    s32 c;
    u8 d;
    u8 e;
    u8 f;
    u8 g;
} Cfg80146;

extern Cfg80146 pslt[];

extern void GsSetFlatLight();
extern void GsSetAmbient();
extern void GsSetLightMode();

void taco_light_set2(void)
{
    pslt[0].a = 20;
    pslt[0].b = 100;
    pslt[0].c = -100;
    pslt[0].d = 176;
    pslt[0].e = 176;
    pslt[0].f = 176;
    GsSetFlatLight(0, &pslt[0]);

    pslt[1].a = 20;
    pslt[1].b = -100;
    pslt[1].c = 100;
    pslt[1].d = 128;
    pslt[1].e = 128;
    pslt[1].f = 128;
    GsSetFlatLight(1, &pslt[1]);

    pslt[2].a = -20;
    pslt[2].b = 20;
    pslt[2].c = -100;
    pslt[2].d = 96;
    pslt[2].e = 96;
    pslt[2].f = 96;
    GsSetFlatLight(2, &pslt[2]);

    GsSetAmbient(0, 0, 0);
    GsSetLightMode(0);
}
