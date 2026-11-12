/* @SymbolStart: 0x8013709C */
/* @SymbolSize: 72 */
/* date2_girl_serifu_init -- DATE2 overlay, Address: 0x801370A4, Size: 64 bytes (16 insns) */
#include "types.h"

extern u8 now_girl;
extern s16 now_serifu;
extern s16 now_serifu_in;

void date_name_set(s32 arg0);
void date2_girl_ptr_set(void);
void sub_sub_step_up(void);

void date2_girl_serifu_init(void)
{
    now_serifu = 1;
    now_serifu_in = 0;
    date_name_set((s32)now_girl);
    date2_girl_ptr_set();
    sub_sub_step_up();
}
