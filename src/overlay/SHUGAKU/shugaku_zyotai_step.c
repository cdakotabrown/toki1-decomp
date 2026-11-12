/* @SymbolSize: 48 */
/* shugaku_zyotai_step - SHUGAKU overlay, Address: 0x80139864, Size: 48 bytes (12 insns) */
#include "types.h"

extern u8 now_girl;
extern void get_g_zyotai_s(s32 a);
extern void sub_sub_step_up(void);

void shugaku_zyotai_step(void)
{
    get_g_zyotai_s((u8)now_girl);
    sub_sub_step_up();
}
