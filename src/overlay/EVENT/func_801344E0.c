/* Address: 0x801344E0 */
/* Size: 176 bytes (44 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[41];
} JumpTable;

extern u8 D_800B1AF6;
extern s32 ev_flag14;
extern void *D_8011F5A8;
extern JumpTable D_80120888;

extern void func_80078950();
extern void func_800F8590(void);

void func_801344E0(void)
{
    s32 pad_hi;
    JumpTable table;

    table = D_80120888;

    func_80078950(&D_8011F5A8, D_800B1AF6);

    table.ptrs[D_800B1AF6](128);

    if (ev_flag14 != 0) {
        func_800F8590();
    }
}
