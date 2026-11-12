/* bunka_sd_title_disp - BUNKA_SD overlay, Address: 0x80137824, Size: 56 bytes (14 insns) */
#include "types.h"

extern s32 set_kanji_string(s32, s32, s32, s32, s32);
extern void k_disp_start(s32);

void bunka_sd_title_disp(s32 a0_in)
{
    s32 ret;

    ret = set_kanji_string(-128, 48, 0, a0_in, 0);
    k_disp_start(ret);
}
