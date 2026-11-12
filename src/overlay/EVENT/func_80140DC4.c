/* @SymbolStart: 0x80140DB4 */
/* @SymbolSize: 176 */
/* @PostProc: swap_instructions --offset1 0x34 --offset2 0x38 */
/* @PostProc: swap_instructions --offset1 0x38 --offset2 0x3C */
/* Address: 0x80140DB4 (absorbed cross-prologue head) .. 0x80140DC4 */
/* Size: 176 bytes (44 instructions) */
#include "types.h"

extern u8 D_800B0866;
extern u8 D_800B0867;
extern u8 D_800B0868;
extern u8 D_800EC190[];
extern volatile u8 D_800EAFA3;
extern volatile u8 D_800EAFE7;
extern s32 ev_flag08;

extern void func_80011DFC();

void func_80140DC4(void)
{
    s16 i;

    D_800B0867 = 0;
    D_800B0866 = 0;
    D_800B0868 = 0;
    for (i = 0; i < 6; i++) {
        *((s32 *) (&D_800EC190[(i * 36) + 144])) &= 0x7FFFFFFF;
    }
    D_800EAFA3 = D_800EAFA3 | 0x80;
    D_800EAFE7 = D_800EAFE7 | 0x80;
    ev_flag08 = 1;
    func_80011DFC();
}
