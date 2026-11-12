/* @SymbolStart: 0x80151648 */
/* @SymbolSize: 308 */
/* Address: 0x80151680 */
/* Size: 252 bytes (63 insns) from the prologue; 308 with the head. */
#include "types.h"
#include "tokimeki/libgs_types.h"

extern s32 pslt;

extern void GsSetFlatLight(s32 idx, void *p);
extern void GsSetAmbient(s32 a, s32 b, s32 c);
extern void GsSetLightMode(s32 a);

void taco_main16_light_set(void)
{
    pslt = 100;
    ((LIBGS_F_LIGHT *)&pslt)[0].vy = 50;
    ((LIBGS_F_LIGHT *)&pslt)[0].vz = -100;
    ((LIBGS_F_LIGHT *)&pslt)[0].r = 112;
    ((LIBGS_F_LIGHT *)&pslt)[0].g = 112;
    ((LIBGS_F_LIGHT *)&pslt)[0].b = 128;
    GsSetFlatLight(0, &pslt);
    ((LIBGS_F_LIGHT *)&pslt)[1].vx = 0;
    ((LIBGS_F_LIGHT *)&pslt)[1].vy = -100;
    ((LIBGS_F_LIGHT *)&pslt)[1].vz = -100;
    ((LIBGS_F_LIGHT *)&pslt)[1].r = 64;
    ((LIBGS_F_LIGHT *)&pslt)[1].g = 64;
    ((LIBGS_F_LIGHT *)&pslt)[1].b = 64;
    GsSetFlatLight(1, &((LIBGS_F_LIGHT *)&pslt)[1].vx);
    ((LIBGS_F_LIGHT *)&pslt)[2].vx = -100;
    ((LIBGS_F_LIGHT *)&pslt)[2].vy = 50;
    ((LIBGS_F_LIGHT *)&pslt)[2].vz = -100;
    ((LIBGS_F_LIGHT *)&pslt)[2].r = 128;
    ((LIBGS_F_LIGHT *)&pslt)[2].g = 128;
    ((LIBGS_F_LIGHT *)&pslt)[2].b = 144;
    GsSetFlatLight(2, &((LIBGS_F_LIGHT *)&pslt)[2].vx);
    GsSetAmbient(0, 0, 0);
    GsSetLightMode(0);
}
