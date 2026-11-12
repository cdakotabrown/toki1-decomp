/* @SymbolStart: 0x8014AFF0 */
/* @SymbolSize: 276 */
/* Function: date_light_init -- DATE overlay */
/* Address: 0x8014B020 (true entry 0x8014AFF0; callers jal 0x8014AFF0) */
/* Size: 276 bytes (69 insns) = 48-byte "fragment" head + 228-byte .sym body. */
#include "types.h"
#include "tokimeki/libgs_types.h"

extern void GsSetFlatLight(s32 idx, void *data);
extern void GsSetAmbient(s32 a0, s32 a1, s32 a2);
extern void GsSetLightMode(s32 a0);

extern s32 pslt;



void date_light_init(void)
{
    pslt = 100;
    ((LIBGS_F_LIGHT *)&pslt)[0].vy = 30;
    ((LIBGS_F_LIGHT *)&pslt)[0].vz = -50;
    ((LIBGS_F_LIGHT *)&pslt)[0].r = 0;
    ((LIBGS_F_LIGHT *)&pslt)[0].g = 0;
    ((LIBGS_F_LIGHT *)&pslt)[0].b = 0;
    GsSetFlatLight(0, &pslt);

    ((LIBGS_F_LIGHT *)&pslt)[1].vx = 20;
    ((LIBGS_F_LIGHT *)&pslt)[1].vy = 10;
    ((LIBGS_F_LIGHT *)&pslt)[1].vz = -100;
    ((LIBGS_F_LIGHT *)&pslt)[1].r = 0;
    ((LIBGS_F_LIGHT *)&pslt)[1].g = 0;
    ((LIBGS_F_LIGHT *)&pslt)[1].b = 0;
    GsSetFlatLight(1, &((LIBGS_F_LIGHT *)&pslt)[1].vx);

    ((LIBGS_F_LIGHT *)&pslt)[2].vx = -20;
    ((LIBGS_F_LIGHT *)&pslt)[2].vy = -20;
    ((LIBGS_F_LIGHT *)&pslt)[2].vz = -100;
    ((LIBGS_F_LIGHT *)&pslt)[2].r = 0;
    ((LIBGS_F_LIGHT *)&pslt)[2].g = 0;
    ((LIBGS_F_LIGHT *)&pslt)[2].b = 0;
    GsSetFlatLight(2, &((LIBGS_F_LIGHT *)&pslt)[2].vx);

    GsSetAmbient(0, 0, 0);
    GsSetLightMode(0);
}
