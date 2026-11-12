/* @SymbolStart: 0x80136194 */
/* @SymbolSize: 200 */
/* @PostProc: patch_insn_reg_field 0xb4:rd:t4:t3 0xb8:rt:t4:t3 */
/* Address: 0x8013619C */
/* Size: 200 bytes (50 instructions, incl. the 2-insn hoisted head) */
#include "types.h"

extern s32 D_800B1AF0;

extern void func_8004C90C(void);
extern u8 D_800F19AB;
extern u8 D_800EC190[];
void func_8013619C(void) {
    s32 arg0_v = D_800B1AF0;

  s16 i;
  char v;
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
    if (((u32) v) < 97)
    {
      D_800EC190[(i * 36) + 20] = (D_800EC190[(i * 36) + 21] = (D_800EC190[(i * 36) + 22] = v) ^ 0);
    }
  }

}
