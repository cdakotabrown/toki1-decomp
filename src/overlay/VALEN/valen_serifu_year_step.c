/* @SymbolStart: 0x80132714 */
/* @SymbolSize: 64 */
/* valen_serifu_year_step - VALEN overlay, Address: 0x80132728, Size: 44 bytes (11 insns) */
#include "types.h"

extern u8 now_year;
extern s16 valen_now_s;
extern void sub_sub_step_up(void);

void valen_serifu_year_step(void)
{
    valen_now_s = (s16)((valen_now_s + now_year * 4) - 384);
    sub_sub_step_up();
}
