/* Function: func_8006B66C */
/* Address: 0x8006B66C */
/* Size: 128 bytes (32 instructions) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: custom_script custom_uwasa_init.py */
/* Region: Game (0x80041000-0x80085FFF) */
#include "types.h"

extern s32 screen_w[];
extern void reserved_date_sort(void);

void func_8006B66C(s32 arg0, s32 entry0) {
    volatile s32 *p_arg0;
    s32 *ptr;
    s32 count;
    s32 t1;
    s32 t3;

    p_arg0 = &arg0;
    count = 0;
    if ((entry0 << 2) < 0) {
        ptr = screen_w;
        do {
            t1 = ptr[340];
            count++;
            t3 = t1 << 2;
            ptr++;
            if (t3 >= 0) {
                break;
            }
        } while (count != 8);
    }
    screen_w[339 + count] = *p_arg0;
    reserved_date_sort();
}
