/* @SymbolStart: 0x80133484 */
/* @SymbolSize: 52 */
/* valen_list_advance_step - VALEN overlay, Address: 0x80133484, Size: 52 bytes (13 insns) */
#include "types.h"

extern s16 valen_list_no;
extern void goto_sub_sub_step(s32);

void valen_list_advance_step(void)
{
    s16 cur;

    cur = valen_list_no;
    valen_list_no = (s16)(cur + 1);
    goto_sub_sub_step(1);
}
