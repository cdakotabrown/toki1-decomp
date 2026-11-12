/* Address: 0x801406EC */
/* Size: 368 bytes (92 insns, window from 0x801406B4) */
/* @SymbolStart: 0x801406B4 */
/* @SymbolSize: 368 */
#include "types.h"

extern volatile s32 pslt;
extern volatile s32 pslt_0_vy;
extern volatile s32 pslt_0_vz;
extern volatile u8 pslt_0_r;
extern volatile u8 pslt_0_g;
extern volatile u8 pslt_0_b;
extern volatile s32 pslt_1_vx;
extern volatile s32 pslt_1_vy;
extern volatile s32 pslt_1_vz;
extern volatile u8 pslt_1_r;
extern volatile u8 pslt_1_g;
extern volatile u8 pslt_1_b;
extern volatile s32 pslt_2_vx;
extern volatile s32 pslt_2_vy;
extern volatile s32 pslt_2_vz;
extern volatile u8 pslt_2_r;
extern volatile u8 pslt_2_g;
extern volatile u8 pslt_2_b;
extern volatile s16 fogp;
extern volatile s32 fogp_dqb;
extern volatile u8 fogp_rfc;
extern volatile u8 fogp_gfc;
extern volatile u8 fogp_bfc;

extern void GsSetFlatLight(s32 a0, void *a1);
extern void GsSetAmbient(s32 a0, s32 a1, s32 a2);
extern void GsSetLightMode(s32 a0);
extern void taiiku_depth_set(void *a0);

void taiiku_light_init(void)
{
    pslt = 100;
    pslt_0_vy = 100;
    pslt_0_vz = 800;
    pslt_0_r = 208;
    pslt_0_g = 208;
    pslt_0_b = 208;
    GsSetFlatLight(0, &pslt);
    pslt_1_vx = 100;
    pslt_1_vy = 100;
    pslt_1_vz = 100;
    pslt_1_r = 240;
    pslt_1_g = 0;
    pslt_1_b = 0;
    GsSetFlatLight(1, &pslt_1_vx);
    pslt_2_vx = 100;
    pslt_2_vy = 100;
    pslt_2_vz = 100;
    pslt_2_r = 0;
    pslt_2_g = 0;
    pslt_2_b = 240;
    GsSetFlatLight(2, &pslt_2_vx);
    GsSetAmbient(2048, 2048, 2048);
    fogp = -3840;
    fogp_dqb = 0x01400000;
    fogp_rfc = 1;
    fogp_gfc = 1;
    fogp_bfc = 1;
    taiiku_depth_set(&fogp);
    GsSetLightMode(1);
}
