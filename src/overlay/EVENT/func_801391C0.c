/* Address: 0x801391C0 */
/* Size: 236 bytes (59 instructions) */
#include "types.h"

typedef void (*FuncPtr)(s32);

typedef struct {
    FuncPtr ptrs[38];
} JumpTable;

extern u8 D_800B1AF6;
extern u16 D_80094714;
extern u16 D_80094718;
extern u8 D_800EB02B;
extern u8 D_800EB02F;
extern s32 ev_flag14;
extern u8 D_800F19AB;
extern char D_8011F6AC;
extern JumpTable D_8012153C;

extern void func_80078950(char *s, u8 a1, u16 a2, u16 a3);

void func_801391C0(void)
{
    s32 pad_hi;
    JumpTable table;

    table = D_8012153C;

    func_80078950(&D_8011F6AC, D_800B1AF6, D_80094714, D_80094718);

    table.ptrs[D_800B1AF6](128);

    if (ev_flag14 != 0) {
        D_800EB02B = D_800EB02B | 0x80;
        D_800EB02F = D_800F19AB;
    } else {
        D_800EB02B = D_800EB02B & 0xFF7F;
    }
}
