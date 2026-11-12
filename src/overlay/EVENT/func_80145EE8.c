/* Address: 0x80145EE8 */
/* Size: 200 bytes (50 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[44];
} JumpTable;

extern u8 D_800B1AF6;
extern u32 D_800B1AE4;
extern s32 ev_flag14;
extern char D_8011FA8C;
extern JumpTable D_80123C5C;

extern void func_80078950(char *s, u8 a1);
extern void func_80109FB0(void);

void func_80145EE8(void)
{
    s32 pad_hi;
    JumpTable table;

    table = D_80123C5C;

    func_80078950(&D_8011FA8C, D_800B1AF6);

    table.ptrs[D_800B1AF6](128);

    if (ev_flag14 != 0) {
        if ((D_800B1AE4 % 120U) == 0) {
            func_80109FB0();
        }
    }
}
