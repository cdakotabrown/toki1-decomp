/* @SymbolSize: 108 */
/* @PostProc: swap_registers t8:t9 */
/* @PostProc: swap_registers t7:t8 */
/* @PostProc: swap_registers t6:t7 */
/* func_80133690 -- GYOZI overlay, Address: 0x80133690, Size: 116 bytes (29 insns) */
#include "types.h"

extern u8 system_config_disp(u8 arg);
extern void func_8004DE1C(void);

extern u8 D_800F62CF;
extern s16 D_80145EB4;

void func_80133690(void)
{
    if ((system_config_disp(D_800F62CF) & 0x7F) != 4) {
        D_80145EB4++;
        func_8004DE1C();
        func_8004DE1C();
        func_8004DE1C();
        return;
    }
    func_8004DE1C();
}
