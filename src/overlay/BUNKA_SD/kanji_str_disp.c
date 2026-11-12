/* kanji_str_disp - BUNKA_SD overlay, Address: 0x80138860, Size: 56 bytes (14 insns) */
#include "types.h"

extern s32 set_kanji_string(s32, s32, s32, s32, s32);
extern void k_disp_start(s32);

void kanji_str_disp(s32 param0)
{
    s32 ret;
    ret = set_kanji_string(-128, 48, 0, param0, 0);
    k_disp_start(ret);
}
