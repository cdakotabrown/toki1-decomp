/* @SymbolStart: 0x801495A0 */
/* @SymbolSize: 48 */
/* date_serifu_row2_set - DATE overlay, Address: 0x801495A8, Size: 48 bytes (12 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern s16 date_now_serifu2;
extern s16 date_now_serifu2_in;

void date_serifu_row2_set(void)
{
    date_now_serifu2 = 2;
    date_now_serifu2_in = 0;
    sub_sub_step_up();
}
