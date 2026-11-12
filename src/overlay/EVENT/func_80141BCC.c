/* @SymbolStart: 0x80141BC4 */
/* @SymbolSize: 200 */
/* @PostProc: patch_insn_reg_field 0x9c:rd:t3:t2 0xa0:rt:t3:t2 0xb4:rd:t4:t3 0xb8:rt:t4:t3 */
/* Address: 0x80141BCC */
/* Size: 200 bytes (50 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern s32 D_800B1AF0;

extern void func_8004C90C(void);
extern u8 D_800F19AB;
extern u8 D_800EC190[];
void func_80141BCC(void) {
    s32 arg0_v = D_800B1AF0;

  s16 i;
  u8 v;
  if (arg0_v == 0)
  {
    for (i = 4; i < 19; i++)
    {
      *((s32 *) (&D_800EC190[i * 36])) &= 0x7FFFFFFF;
    }

  }
  func_8004C90C();
  v = D_800F19AB;
  for (i = 4; i < 19; i++)
  {
    if (((u32) v) < 113)
    {
      if (((!(i & 0xFFFF)) && (!i)) && (!i))
      {
      }
      D_800EC190[(i * 36) + 20] = (D_800EC190[(i * 36) + 21] = (D_800EC190[(i * 36) + 22] = v));
    }
  }

}
