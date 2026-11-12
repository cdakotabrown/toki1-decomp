/* Address: 0x8014AD14 */
/* Size: 72 bytes (18 instructions) */
#include "types.h"

extern s32 get_CD_State2(void);
extern s32 dec_bg_init(s32);
extern void indexed_byte_lsb_set(s32, s32);
extern void sub_sub_step_up(void);

void etc_cd_wait(void)
{
    s32 result;
    s32 ret;

    result = get_CD_State2();
    if ((result & 0x0001) == 0) {
        return;
    }
    ret = dec_bg_init(1);
    indexed_byte_lsb_set(1, ret);
    sub_sub_step_up();
}
