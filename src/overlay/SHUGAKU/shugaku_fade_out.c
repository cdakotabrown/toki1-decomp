#include "types.h"

extern void fade_out(void);
extern u8 shugaku_place;
extern u8 fade_bri;
extern u8 cw97_bright;
extern u8 cw97_disp;
char shugaku_fade_out(void)
{
  s32 t6;
  s32 v0;
  s32 t7;
  s32 t8;
  fade_out();
  t6 = (s32) ((unsigned short) shugaku_place);
  if (t6 != 1)
  {
    return;
  }
  v0 = (s32) fade_bri;
  cw97_bright = (u8) v0;
  if (((u32) v0) < 8U)
  {
    t7 = (s32) cw97_disp;
    cw97_disp = (u8) (t8 = t7 & 0x7F);
  }
}
