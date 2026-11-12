/* @SymbolSize: 56 */
/* date2_serifu_set21 - DATE2 overlay, Address: 0x80136D14, Size: 56 bytes (14 insns) */
#include "types.h"

extern u8 now_girl;
extern s16 date2_now_s;

void date_name_set(s32 arg0);
void sub_sub_step_up(void);

void date2_serifu_set21(void)
{
    date_name_set((s32)now_girl);
    date2_now_s = 21;
    sub_sub_step_up();
}
