/* @SymbolStart: 0x8014382C */
/* @SymbolSize: 296 */
/* Address: 0x8014385C */
/* Size: 248 bytes (62 insns) from the prologue; 296 with the head. */
#include "types.h"
#include "tokimeki/libgs_types.h"

extern s32 pslt;

extern void GsSetFlatLight(s32 idx, void *p);
extern void GsSetAmbient(s32 a, s32 b, s32 c);
extern void GsSetLightMode(s32 a);

void taco_light_init(void)
{
    pslt = 0;
    ((LIBGS_F_LIGHT *)&pslt)[0].vy = 0;
    ((LIBGS_F_LIGHT *)&pslt)[0].vz = -100;
    ((LIBGS_F_LIGHT *)&pslt)[0].r = 255;
    ((LIBGS_F_LIGHT *)&pslt)[0].g = 255;
    ((LIBGS_F_LIGHT *)&pslt)[0].b = 255;
    GsSetFlatLight(0, &pslt);
    ((LIBGS_F_LIGHT *)&pslt)[1].vx = 0;
    ((LIBGS_F_LIGHT *)&pslt)[1].vy = 100;
    ((LIBGS_F_LIGHT *)&pslt)[1].vz = -100;
    ((LIBGS_F_LIGHT *)&pslt)[1].r = 255;
    ((LIBGS_F_LIGHT *)&pslt)[1].g = 255;
    ((LIBGS_F_LIGHT *)&pslt)[1].b = 255;
    GsSetFlatLight(1, &((LIBGS_F_LIGHT *)&pslt)[1].vx);
    ((LIBGS_F_LIGHT *)&pslt)[2].vx = -100;
    ((LIBGS_F_LIGHT *)&pslt)[2].vy = 0;
    ((LIBGS_F_LIGHT *)&pslt)[2].vz = -100;
    ((LIBGS_F_LIGHT *)&pslt)[2].r = 255;
    ((LIBGS_F_LIGHT *)&pslt)[2].g = 255;
    ((LIBGS_F_LIGHT *)&pslt)[2].b = 255;
    GsSetFlatLight(2, &((LIBGS_F_LIGHT *)&pslt)[2].vx);
    GsSetAmbient(2048, 2048, 2048);
    GsSetLightMode(0);
}
