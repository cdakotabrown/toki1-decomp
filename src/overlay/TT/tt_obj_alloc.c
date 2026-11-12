/* tt_obj_alloc - TT overlay, Address: 0x8013BD10, Size: 60 bytes (15 insns) */
/* `0x8013BD34:24:func_8013BD34`.  Until that lands, `@SymbolSize: 60` windows */
/* @SymbolSize: 60 */
#include "types.h"

u8 *tt_obj_alloc(u8 *p, u8 *end)
{
    while (p < end) {
        if (p[0] == 0) {
            return p;
        }
        p += 108;
    }
    return (u8 *)0;
}
