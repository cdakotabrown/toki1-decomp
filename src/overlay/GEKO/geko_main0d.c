/* geko_main0d -- GEKO overlay, Address: 0x80141AD0, Size: 32 bytes (8 insns) */
#include "types.h"

extern void geko_bustup_guard(void);

void geko_main0d(void)
{
    geko_bustup_guard();
}
