/* @SymbolStart: 0x80135574 */
/* @SymbolSize: 48 */
#include "types.h"

extern void sub_sub_step_up(void);
extern s16 geko_now_serifu2;
extern s16 geko_now_serifu2_in;

void geko_serifu2_row35_set(void)
{
    geko_now_serifu2 = 35;
    geko_now_serifu2_in = 0;
    sub_sub_step_up();
}
