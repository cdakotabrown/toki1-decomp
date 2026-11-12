/* @SymbolSize: 60 */
/* Function: date_cdwait3 */
/* Overlay: DATE */
/* Address: 0x8014A2D0 */
/* Size: 60 bytes (15 instructions) */
#include "types.h"

extern s16 date_open_step;
extern s32 get_CD_State2();

void date_cdwait3(void)
{
    if (get_CD_State2() & 1) {
        date_open_step++;
    }
}
