/* @SymbolStart: 0x80132EC8 */
/* @SymbolSize: 128 */
/* Address: 0x80132EE0, Size: 104 bytes (26 insns) */
#include "types.h"

extern s32 step_counter;
extern u8 p_name2;
extern u8 now_girl;

void memcpy(s32 arg0, s32 arg1, s32 arg2);
void get_p_name(void *arg0, s32 arg1);
void sub_sub_step_up(void);

void date2_image_restore(void)
{
    s32 cnt;
    s32 n;

    n = step_counter * 5120;
    memcpy(n + (s32)0x80180000, n + (s32)0x801A0000, 5120);
    cnt = step_counter;
    cnt++;
    step_counter = cnt;
    if (cnt == 18) {
        now_girl = 0;
        get_p_name(&p_name2, 0);
        sub_sub_step_up();
    }
}
