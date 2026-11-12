/* Address: 0x8013F4E8 */
/* Size: 156 bytes (39 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[38];
} JumpTable;

extern u8 D_800B1AF6;
extern s32 ev_flag04;
extern s32 D_8011F8B8;
extern JumpTable D_80122848;

extern void func_80078950(s32 a0, u8 a1, s32 a2);

void func_8013F4E8(void)
{
    s32 pad_hi;
    JumpTable table;

    table = D_80122848;

    func_80078950((s32)&D_8011F8B8, D_800B1AF6, ev_flag04);

    table.ptrs[D_800B1AF6](128);
}
