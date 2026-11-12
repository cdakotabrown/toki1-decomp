/* Function: date_basyo18_wait */
/* Overlay: DATE */
/* @SymbolSize: 32 */
#include "types.h"

extern void step_wait(s32);

void date_basyo18_wait(void)
{
    step_wait(1000);
}
