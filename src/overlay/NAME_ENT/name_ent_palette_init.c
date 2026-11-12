/* @CompileFlags: -Wo,-loopunroll,0 */
/* @SymbolSize: 112 */
/* name_ent_palette_init - NAME_ENT overlay, Address: 0x8014781C, Size: 112 bytes (28 insns) */
#include "types.h"

extern u32 screen_w[];
extern u32 girl1_flag[];
extern u32 cursor_work_bak[];
extern void palette_upload_enqueue(u32 *p, s32 a1, s32 a2, s32 a3, s32 a4);
void name_ent_palette_init(void)
{
  u32 *p;
  u32 *end;
  u32 fill;
  fill = 0x7FFF7FFF;
 do { p = screen_w; end = girl1_flag; do { p += 4; p[0x58C] = fill; p[0x58D] = fill; p[0x58E] = fill; p[0x58B] = fill; } while (p != end); } while (0);
  cursor_work_bak[0] = 0x7FFF0000;
  fill = fill;
  palette_upload_enqueue(cursor_work_bak, 14, 1, 2, 0);
}
