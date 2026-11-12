/* @SymbolSize: 64 */
/* date2_girl0_bustup_read -- DATE2 overlay, Address: 0x80133598, Size: 72 bytes (18 insns) */
#include "types.h"

extern u8 now_girl;
extern u8 p_name2[];
void get_p_name(void *arg0, s32 arg1);
void bustup_cd_read(s32 arg0);
void sub_sub_step_up(void);

void date2_girl0_bustup_read(void)
{
    now_girl = 0;
    get_p_name(p_name2, 0);
    bustup_cd_read(21983);
    sub_sub_step_up();
}
