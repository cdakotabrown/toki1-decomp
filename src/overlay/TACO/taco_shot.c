/* @PostProc: scheduler_normalizer */
/* Address: 0x8013DD04 */
/* Size: 232 bytes (58 insns) */
#include "types.h"

typedef struct
{
  s16 x;
  s16 y;
  s16 z;
  s16 pad6;
  s16 dx;
  s16 dy;
  s16 dz;
  s16 pad14;
  s16 pad16;
  u8 life;
  u8 flags;
} Particle;
extern Particle taco_pickup_tbl[16];
extern void taco_shot_disp(s32 idx);
void taco_shot(void)
{
  Particle *e;
  unsigned int i;
  e = taco_pickup_tbl;
  do
  {
    e->life = e->life + 1;
    if (((u8) e->life) == 255)
    {
      e->flags = 0;
    }
    if (e->z < 10001)
    {
    }
    else
    {
      e->flags = 0;
    }
    if (e->flags & 0x80)
    {
      e->x = e->x + e->dx;
      if (1)
      {
        e->y = e->y + e->dy;
        e->z = e->z + e->dz;
      }
    }
    e++;
  }
  while (((u32) e) < ((u32) (&taco_pickup_tbl[16])));
  e = taco_pickup_tbl;
  for (i = 0; i != 16; i++)
  {
    if (e->flags & 0x80)
    {
      taco_shot_disp(i);
    }
    e++;
  }

  if (taco_pickup_tbl && taco_pickup_tbl)
  {
  }
}
