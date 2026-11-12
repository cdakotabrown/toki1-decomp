/* Address: 0x8013EA24 */
/* Size: 140 bytes (35 instructions) */
#include "types.h"

typedef void (*FuncPtr)(void);

typedef struct {
    FuncPtr ptrs[19];
} JumpTable;

extern u8 D_800B1AF6;
extern void *D_8011F8A0;
extern JumpTable D_801227FC;

extern void func_80078950();

void func_8013EA24(void)
{
    s32 pad_hi;
    JumpTable table;

    table = D_801227FC;

    func_80078950(&D_8011F8A0, D_800B1AF6);

    table.ptrs[D_800B1AF6]();
}
