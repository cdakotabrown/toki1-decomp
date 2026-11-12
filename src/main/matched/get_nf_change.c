/* Function: get_nf_change */
/* Likely Symbol: get_nf_change (from o.bin, offset +24 bytes from 0x80066C14) */
/* Address:  0x80066BFC */
/* Debug Symbol: get_nf_change (from o.bin, high match) */
/* Size:     116 bytes (29 instructions) */
/* Region:   GCC (0x80041000-0x80085FFF) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @CompileFlags: -Wc,-pic1 */
#include "types.h"

extern s32 get_nf(void);
extern u8 now_month;
extern u8 now_day;

s32 get_nf_change(void)
{
    s32 result;
    unsigned int new_var;
    u8 temp;
    u8 temp2;

    result = get_nf();
    temp = now_month;

    if (temp == 9)
    {
        temp2 = now_day;
        if (temp2 >= 24U)
        {
            result = -1;
        }
    }

    new_var = temp;
    if (new_var == 5)
    {
        temp2 = now_day;
        if (temp2 >= 25U)
        {
            result = -1;
        }
    }

    return result;
}
