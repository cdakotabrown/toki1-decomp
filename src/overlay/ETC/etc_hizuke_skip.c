/* Address: 0x8014C250 */
/* Size: 80 bytes (20 instructions) */
#include "types.h"

extern void hizuke_advance();
extern void return_schedule();

void etc_hizuke_skip(void)
{
    hizuke_advance();
    hizuke_advance();
    hizuke_advance();
    hizuke_advance();
    hizuke_advance();
    hizuke_advance();
    return_schedule(1);
}
