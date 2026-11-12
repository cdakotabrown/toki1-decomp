/* Function: LoadClut */
/* Address: 0x800A33FC */
/* Size: 96 bytes (24 instructions) */
/* Region: Library (0x80086000+) */
/* @PostProc: strip_leading_instructions --count 4 */
/* @PostProc: convert_or_to_addu */
/* @PostProc: custom_script func_800A33FC_postproc.py */
#include "types.h"

extern void LoadImage(s16 *a0, s32 a1);
extern void GetClut(s32 a0, s32 a1);

void LoadClut(volatile s32 arg0, volatile s32 arg1, volatile s32 arg2) {
    s16 local[4];
    s32 a1val;
    s32 a2val;

    a1val = arg1;
    a2val = arg2;
    local[1] = (s16)a2val;
    local[0] = (s16)a1val;
    local[3] = 1;
    local[2] = 256;
    LoadImage(local, arg0);
    GetClut(arg1, arg2);
}
