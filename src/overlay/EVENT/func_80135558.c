/* func_80135558 -- EVENT overlay, Address: 0x80135558, Size: 296 bytes (74 insns) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[42];
} JumpTable;

extern void func_80078950(char *s, u8 idx);
extern void func_800433D0(s32);

extern u8 D_800B1AF6;
extern s32 D_800B1AE4;
extern s32 ev_flag14;
extern char D_8011F5B4;
extern JumpTable D_80120B3C;

void func_80135558(void)
{
    s32 pad_hi;
    JumpTable table;

    table = D_80120B3C;

    func_80078950(&D_8011F5B4, D_800B1AF6);

    table.ptrs[D_800B1AF6](128);

    if (ev_flag14 != 0 && ((u32)D_800B1AE4 % 180) == 0) {
        func_800433D0(1281);
    }
    if (ev_flag14 != 0 && ((u32)D_800B1AE4 % 210) == 10) {
        func_800433D0(1282);
    }
    if (ev_flag14 != 0 && ((u32)D_800B1AE4 % 450) == 20) {
        func_800433D0(1280);
    }
}
