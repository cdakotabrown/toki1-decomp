/* @RodataBase: 0x8014D478 */
/* tt_shot_heading_make - TT overlay, Address: 0x80133E30, Size: 384 bytes (96 insns) */
#include "types.h"
#include "tokimeki/tt_box.h"

extern u8 *tt_shot_buf;
extern TT_BOX tt_hit_box_tbl[];
extern s32 get_tt_shot_free(u8 *a0, u8 *a1);
extern s32 rcos(s32 a0);
extern s32 rsin(s32 a0);
extern void tt_shot_sprite_set(s32 a0, s32 a1, s32 a2);
void tt_shot_heading_make(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
  u8 *base;
  s32 v0;
  s32 s0;
  s32 t9;
  base = tt_shot_buf;
  v0 = get_tt_shot_free(base, base + 13312);
  if (v0 == 0)
  {
    return;
  }
  s0 = v0;
  *((s32 *) (s0 + 32)) = ((s32) ((s16) arg0)) << 16;
  *((s32 *) (s0 + 36)) = ((s32) ((s16) arg1)) << 16;
  *((s32 *) (s0 + 40)) = rcos(arg3) * arg2;
  *((s32 *) (s0 + 44)) = rsin(arg3) * arg2;
  switch ((u16) arg4)
  {
    case 160:
      tt_shot_sprite_set(s0, 160, 18);
      t9 = 64;
      goto tail;

    case 161:
      tt_shot_sprite_set(s0, 161, 19);
      t9 = 64;
      goto tail;

    case 168:
      tt_shot_sprite_set(s0, 168, 21);
      t9 = 64;
      goto tail;

    case 169:
      tt_shot_sprite_set(s0, 169, 22);
      t9 = 64;
      goto tail;

    case 172:
      *((s16 *) (s0 + 96)) = (s16) (arg3 + 2048);
      *((s16 *) (s0 + 88)) = 0;
      tt_shot_sprite_set(s0, 172, 20);
      *((s32 *) (s0 + 76)) = (s32) (&(*(s32 *)&tt_hit_box_tbl[2]));
      t9 = 64;
      break;

    default:
      t9 = 64;
      break;

  }

  tail:
  *((s16 *) (s0 + 4)) = (s16) t9;

  *((s16 *) (s0 + 10)) = 0;
  *((s16 *) (s0 + 8)) = 0;
  *((s16 *) (s0 + 6)) = 0;
  *((u8 *) (s0 + 15)) = 0;
  *((u8 *) (s0 + 14)) = 0;
  *((u8 *) (s0 + 13)) = 0;
}
