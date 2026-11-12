/* @PostProc: scheduler_normalizer */
/* @PostProc: patch_insn_reg_field 0x34:rt:v0:t6 0x44:rs:v0:t6 0x58:rs:v0:t6 */
/* Address: 0x801371C4 */
/* Size: 176 bytes (44 insns) */
#include "types.h"

extern void glyph_disp(s32, s32, s32, s32, s32, s32);
void taco_str_disp(u8 *str, s32 arg1, s32 arg2, s32 arg3, s32 col, s32 arg6)
{
  u8 *p;
  s32 ch;
  s32 m;
  s32 i;
  ch = str[0];
  if (ch == 0)
  {
    return;
  }
  p = str;
  i = col * 0;
  do
  {
    m = ch & 0xFF;
    glyph_disp(m, arg2, col, arg6, arg1, arg3);
    i++;
    p++;
    col++;
    if (i >= 21)
    {
      return;
    }
    ch = p[0];
  }
  while ((ch & 0xFF) != 0);
}
