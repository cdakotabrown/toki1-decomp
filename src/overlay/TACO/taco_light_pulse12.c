/* @SymbolSize: 628 */
/* Address: 0x80149CEC */
/* Size: 628 bytes (157 insns) */
#include "types.h"

extern s32 pslt;
extern s32 pslt_0_vy;
extern s32 pslt_0_vz;
extern u8 pslt_0_r;
extern u8 pslt_0_g;
extern u8 pslt_0_b;
extern s32 pslt_1_vx;
extern s32 pslt_1_vy;
extern s32 pslt_1_vz;
extern u8 pslt_1_r;
extern u8 pslt_1_g;
extern u8 pslt_1_b;
extern s32 pslt_2_vx;
extern s32 pslt_2_vy;
extern s32 pslt_2_vz;
extern u8 pslt_2_r;
extern u8 pslt_2_g;
extern u8 pslt_2_b;
extern u8 taco_mainc_hp_tbl;
extern u8 *taco_actor_rec;
extern s32 get_taco_percent(s32 a, void *p);
extern void GsSetFlatLight(s32 idx, void *p);
void taco_light_pulse12(void)
{
  unsigned int ret;
  u8 *ptr;
  s32 v;
  ret = get_taco_percent(12, &taco_mainc_hp_tbl);
  ptr = taco_actor_rec;
  if (ptr[2994] < ret)
  {
    pslt = 0;
    pslt_0_vy = 0;
    pslt_0_vz = -100;
    pslt_0_r = 255;
    pslt_0_g = 128;
    pslt_0_b = 128;
    GsSetFlatLight(0, &pslt);
    pslt_1_vx = 0;
    pslt_1_vy = 0;
    pslt_1_vz = -100;
    pslt_1_r = 255;
    pslt_1_g = 128;
 if (1) { }
    pslt_1_b = 128;
    GsSetFlatLight(1, &pslt_1_vx);
    pslt_2_vx = 0;
    pslt_2_vy = 0;
    pslt_2_vz = -100;
    pslt_2_r = 255;
    pslt_2_b = (pslt_2_g = 128);
    GsSetFlatLight(2, &pslt_2_vx);
    ptr = taco_actor_rec;
    v = ptr[2995];
    if (((u32) v) >= 3)
    {
      taco_actor_rec[2994] = ret;
      pslt = 20;
      pslt_0_vy = 100 & 0xFFFFFFFFFFFFFFFF;
      pslt_0_vz = -100;
      pslt_0_r = 176;
      pslt_0_g = 176;
      pslt_0_b = 176;
      GsSetFlatLight(0, &pslt);
      pslt_1_vx = 20;
      pslt_1_vy = -100;
      pslt_1_vz = 100;
      pslt_1_r = 128;
      pslt_1_g = 128;
      pslt_1_b = 128;
      GsSetFlatLight(1, &pslt_1_vx);
      pslt_2_vx = -20;
      pslt_2_vy = 20;
      pslt_2_vz = -100;
      pslt_2_r = 96;
      pslt_2_g = 96;
      pslt_2_b = (short) 96;
      GsSetFlatLight(2, &pslt_2_vx);
      ptr = taco_actor_rec;
      ptr[2995] = 0;
    }
    else
    {
      ptr = taco_actor_rec;
      ptr[2995] = v + 1;
    }
  }
}
