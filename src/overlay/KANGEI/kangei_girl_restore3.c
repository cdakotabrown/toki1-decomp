/* @SymbolSize: 36 */
/* kangei_girl_restore3 - KANGEI overlay, Address: 0x8013844C, Size: 44 bytes (11 insns) */
/* @PostProc: swap_registers t6:at */
#include "types.h"

extern void kangei_girl_restore(void);

void
kangei_girl_restore3(void)
{
    *(u8 *)(0x80140000 - 25904) = 0;
    kangei_girl_restore();
}
