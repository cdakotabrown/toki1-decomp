/* @SymbolStart: 0x80140E50 */
/* @SymbolSize: 64 */
/* shougatu_girl_yuukou_set - SHOUGATU overlay, Address: 0x80140E58, Size: 56 bytes (14 insns) */
#include "types.h"

extern void sub_sub_step_up(void);
extern u8 now_girl;

void shougatu_girl_yuukou_set(void)
{
    *(s16 *)(0x800EE922 + 56 * now_girl) = 40;
    sub_sub_step_up();
}
