/* @SymbolStart: 0x80136738 */
/* @CompileFlags: -Wo,-loopunroll,0 */
/* @SymbolSize: 384 */
/* name_ent_hizuke_cell_set - NAME_ENT overlay, Address: 0x80136740, Size: 376 bytes (94 insns) */
#include "types.h"
#include "tokimeki/class_w.h"

extern void name_ent_key_set(s32 voice);
extern s16 cw97_frame;
extern u8 cw97_mode;
extern s16 cw98_frame;
extern u8 class_w[];
extern u8 cw2_active[];
extern u8 cw4_active[];
extern u8 cw6_active[];
extern u8 name_ent_cell_no;
void name_ent_hizuke_cell_set(void)
{
  u8 *p;
  if (name_ent_cell_no == 0)
  {
    name_ent_key_set(97);
    name_ent_key_set(98);
    ((CLASS_W *)class_w)[99].frame = 13;
    ((CLASS_W *)class_w)[99].anim_flag = 64;
    ((CLASS_W *)class_w)[100].frame = 13;
    ((CLASS_W *)class_w)[100].anim_flag = 64;
    ((CLASS_W *)class_w)[101].frame = 13;
    ((CLASS_W *)class_w)[101].anim_flag = 64;
    ((CLASS_W *)class_w)[102].frame = 13;
    ((CLASS_W *)class_w)[102].anim_flag = 64;
  }
  else
  {
 do { if (name_ent_cell_no == 1) { name_ent_key_set(99); name_ent_key_set(100); p = class_w; do { p += 0x44; *((s16 *) (p + 0x1998)) = 13; *((s8 *) (p + 0x1982)) = 64; } while (p < cw2_active); p = cw4_active; do { p += 0x44; *((s16 *) (p + 0x1998)) = 13; *((s8 *) (p + 0x1982)) = 64; } while (p != cw6_active); } else if (name_ent_cell_no == 2) { cw97_frame = 13; cw97_mode = 64; cw98_frame = 13; ((CLASS_W *)class_w)[98].anim_flag = 64; ((CLASS_W *)class_w)[99].frame = 13; ((CLASS_W *)class_w)[99].anim_flag = 64; ((CLASS_W *)class_w)[100].frame = 13; ((CLASS_W *)class_w)[100].anim_flag = 64; name_ent_key_set(101); name_ent_key_set(102); } } while (0);
  }
}
