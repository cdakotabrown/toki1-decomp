/* Function: initLight */
/* Address: 0x80059120 (prologue at 0x80059150) */
/* Size: 272 bytes (68 instructions) */
/* Region: Game (0x80041000-0x80085FFF) */
/* @SymbolStart: 0x80059120 */
/* @SymbolSize: 272 */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

typedef struct {
    s32 field_00;
    s32 field_04;
    s32 field_08;
    u8  r;
    u8  g;
    u8  b;
    u8  pad;
} SndEntry;

extern SndEntry pslt;
extern SndEntry pslt_1_vx;
extern SndEntry pslt_2_vx;

extern void GsSetFlatLight(s32, SndEntry *);
extern void GsSetAmbient(s32, s32, s32);
extern void GsSetLightMode(s32);

void initLight(void) {
    pslt.field_00 = 0;
    pslt.field_04 = 0;
    pslt.field_08 = 100;
    pslt.r = 255;
    pslt.g = 255;
    pslt.b = 255;
    GsSetFlatLight(0, &pslt);

    pslt_1_vx.field_00 = 0;
    pslt_1_vx.field_04 = 100;
    pslt_1_vx.field_08 = 0;
    pslt_1_vx.r = 0;
    pslt_1_vx.g = 255;
    pslt_1_vx.b = 0;
    GsSetFlatLight(1, &pslt_1_vx);

    pslt_2_vx.field_00 = 100;
    pslt_2_vx.field_04 = 0;
    pslt_2_vx.field_08 = 0;
    pslt_2_vx.r = 255;
    pslt_2_vx.g = 0;
    pslt_2_vx.b = 0;
    GsSetFlatLight(2, &pslt_2_vx);

    GsSetAmbient(0, 0, 0);
    GsSetLightMode(0);
}
