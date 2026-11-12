/* @SymbolStart: 0x801389FC */
/* @SymbolSize: 64 */
/* kangei_girl_basyo_restore - KANGEI overlay, Address: 0x80138A04, Size: 56 bytes (14 insns) */
#include "types.h"

extern u8 kangei_girl_save_no;
extern u8 now_girl;
extern s16 kangei_now_s;
extern s16 kangei_now_s_in;

extern void kangei_girl_msg_set(void);
extern void sub_sub_step_up(void);

void
kangei_girl_basyo_restore(void)
{
    now_girl = kangei_girl_save_no;
    kangei_girl_msg_set();
    kangei_now_s = 0;
    kangei_now_s_in = 0;
    sub_sub_step_up();
}
