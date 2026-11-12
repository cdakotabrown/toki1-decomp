/* @SymbolSize: 240 */
/* en_nichi_count_set - EN_NICHI overlay, Address: 0x80136A80, Size: 240 bytes (60 insns) */
#include "types.h"
#include "tokimeki/class_w.h"
extern CLASS_W class_w[];


void en_nichi_count_set(s32 arg0, s32 arg1)
{
    s32 ten;

    ten = 10;
    class_w[144].disp_flag = 132;
    class_w[145].disp_flag = 132;
    class_w[146].disp_flag = 132;
    class_w[147].disp_flag = 132;
    class_w[144].frame = arg0 % ten + 35;
    class_w[145].frame = arg0 / ten + 35;
    if (arg0 < 10) {
        class_w[145].disp_flag = 0;
    }
    class_w[146].frame = arg1 % ten + 35;
    class_w[147].frame = arg1 / ten + 35;
    if (arg1 < 10) {
        class_w[147].disp_flag = 0;
    }
}
