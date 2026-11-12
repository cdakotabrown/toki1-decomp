/* @SymbolStart: 0x80142544 */
/* @SymbolSize: 96 */
#include "types.h"

extern u8 now_sub_step;
extern u8 now_sub_sub_step;
extern s32 work_fuku;

extern void white_light(s32);

void name_ent_white_fade(void)
{
    if (now_sub_step | now_sub_sub_step) {
        work_fuku = work_fuku - 1;
    }
    if (work_fuku > 0) {
        white_light(work_fuku);
    }
}
