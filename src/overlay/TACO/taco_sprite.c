/* @PostProc: patch_insn_reg_field 0x98:rt:v0:t9 0xb4:rs:v0:t9 */
/* Address: 0x80142280 */
/* Size: 248 bytes (62 insns) */
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
  s16 padE;
  s16 pad10;
  u8 timer;
  u8 flags;
} Sprite80142280;
extern void taco_item_bill_disp(s32, s32);
extern Sprite80142280 taco_throw_tbl[];
s32 taco_sprite(void)
{
  Sprite80142280 *e;
  s32 i;
  short t;
  e = taco_throw_tbl;
  i = 0;
  do
  {
    if (((u32) ((u8) e->timer)) >= 255U)
    {
      e->flags = 0;
      return 0;
    }
    if (((u32) ((u8) e->timer)) >= 128U)
    {
      e->dz = -1024;
    }
    if (e->z < (-32000))
    {
      e->flags = 0;
    }
    if ((e->flags & 0x80) != 0)
    {
      t = ((u8) e->timer) + 1;
      e->timer = (u8) t;
      e->x = e->x + e->dx;
      e->y = e->y + e->dy;
      e->z = e->z + e->dz;
      taco_item_bill_disp(i, t & 0xFF);
    }
    i++;
    e++;
  }
  while (16 != i);
  return 1;
}
