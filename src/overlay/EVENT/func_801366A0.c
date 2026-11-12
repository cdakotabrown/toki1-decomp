/* Address: 0x801366A0 */
/* Size: 228 bytes (57 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);
typedef struct
{
  FuncPtr ptrs[47];
} JumpTable;
extern u8 D_800B1AF6;
extern u8 D_800EB02B;
extern JumpTable D_80120E24;
extern void func_8004C90C(s32);
extern void func_8004C984(s32);
extern void func_800FA850(void);
void func_801366A0(void)
{
    s32 pad_hi;
  JumpTable table;
  FuncPtr fn;
  table = D_80120E24;
  table.ptrs[D_800B1AF6](128);
  ;
  if (table.ptrs[D_800B1AF6] == func_8004C90C)
  {
    D_800EB02B |= 0x80;
    if (!D_800B1AF6)
    {
    }
  }
  if (table.ptrs[D_800B1AF6] == func_8004C984)
  {
    if (1)
    {
    }
    D_800EB02B &= ~0x80;
  }
  func_800FA850();
}
