/* @PostProc: patch_stack_complete --old 64 --new 72 */
/* @PostProc: remap_stack_slots --from 36 --to 48 --from 40 --to 52 --from 44 --to 56 --from 48 --to 60 */
/* Address: 0x80143610 */
/* Size: 228 bytes (57 insns) */
#include "types.h"

extern void load_image_rect(volatile unsigned short, s32, s32, s32, s32);
void taco_tile_load(s32 base, s32 cell)
{
  s32 x;
  s32 y;
  u16 x1;
  u16 y1;
  x = (cell & 0x0F) << 6;
  y = (cell & 0x10) << 4;
  load_image_rect((u16) x, (u16) y, 64, 128, base);
  x1 = (u16) (x + 64);
  load_image_rect(x1, (u16) y, 64, 128, base + 16384);
  y1 = (u16) (y + 128);
  load_image_rect((u16) x, y1, 64, 128, base + 32768);
  load_image_rect(x1, y1, 64, 128, base + 49152);
}
