/* Address: 0x8013DF24 */
/* Size: 164 bytes (41 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[67];
} JumpTable;

extern u8 D_800B1AF6;
extern s32 D_800EECBC;
extern s16 D_801243A4[];
extern char D_8011F828;
extern JumpTable D_80122348;

extern void func_80078950(char *s, u8 a1, s16 a2);

void func_8013DF24(void)
{
    s32 pad_hi;
    JumpTable table;

    table = D_80122348;

    func_80078950(&D_8011F828, D_800B1AF6, D_801243A4[D_800EECBC]);
    table.ptrs[D_800B1AF6](128);
}
