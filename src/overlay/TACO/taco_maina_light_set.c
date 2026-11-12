/* @SymbolStart: 0x8014863C */
/* @SymbolSize: 312 */
/* Address: 0x80148674 */
/* Size: 256 bytes (64 insns) */
#include "types.h"
#include "tokimeki/libgs_types.h"

extern s32 pslt;

extern void GsSetFlatLight(s32 idx, void *p);
extern void GsSetAmbient(s32 a, s32 b, s32 c);
extern void GsSetLightMode(s32 a);

void taco_maina_light_set(void)
{
    pslt = -100;
    ((LIBGS_F_LIGHT *)&pslt)[0].vy = 30;
    ((LIBGS_F_LIGHT *)&pslt)[0].vz = -30;
    ((LIBGS_F_LIGHT *)&pslt)[0].r = 224;
    ((LIBGS_F_LIGHT *)&pslt)[0].g = 224;
    ((LIBGS_F_LIGHT *)&pslt)[0].b = 224;
    GsSetFlatLight(0, &pslt);
    ((LIBGS_F_LIGHT *)&pslt)[1].vx = 40;
    ((LIBGS_F_LIGHT *)&pslt)[1].vy = 50;
    ((LIBGS_F_LIGHT *)&pslt)[1].vz = -100;
    ((LIBGS_F_LIGHT *)&pslt)[1].r = 64;
    ((LIBGS_F_LIGHT *)&pslt)[1].g = 64;
    ((LIBGS_F_LIGHT *)&pslt)[1].b = 64;
    GsSetFlatLight(1, &((LIBGS_F_LIGHT *)&pslt)[1].vx);
    ((LIBGS_F_LIGHT *)&pslt)[2].vx = 10;
    ((LIBGS_F_LIGHT *)&pslt)[2].vy = -20;
    ((LIBGS_F_LIGHT *)&pslt)[2].vz = -100;
    ((LIBGS_F_LIGHT *)&pslt)[2].r = 48;
    ((LIBGS_F_LIGHT *)&pslt)[2].g = 48;
    ((LIBGS_F_LIGHT *)&pslt)[2].b = 48;
    GsSetFlatLight(2, &((LIBGS_F_LIGHT *)&pslt)[2].vx);
    GsSetAmbient(0, 0, 0);
    GsSetLightMode(0);
}
