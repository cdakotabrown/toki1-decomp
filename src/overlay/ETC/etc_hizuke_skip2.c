/* @SymbolSize: 80 */
/* Address: 0x8014C2A0 */
/* Size: 80 bytes (20 instructions), ending 0x8014C2F0 with jr ra; nop. */
#include "types.h"

extern void hizuke_advance();
extern void return_schedule();

void etc_hizuke_skip2(void)
{
    hizuke_advance();
    hizuke_advance();
    hizuke_advance();
    hizuke_advance();
    hizuke_advance();
    hizuke_advance();
    return_schedule(1);
}
