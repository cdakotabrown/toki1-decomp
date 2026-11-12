/* Function: LoadTPage */
/* Address: 0x800A3320 */
/* Size: 220 bytes (55 instructions) */
/* Region: Library (0x80086000+) */
/* 2026-09-17 hybrid screen (@Compiler: ido52-gas, switch body): retail's */
/* @CompileFlags: -Wc,-pic1 */
/* @CompileEnv: IDO52_FORCE_WEB_COLOR=32:t1,24:v1,2:t0 IDO52_FORCE_WEB_COLOR_NOSPLIT=32,24,2 IDO52_AS1_NO_COPY_PROP=all IDO52_AS1_DIV_POW2=aspsx IDO52_NO_JUMP_MERGE=all IDO52_AS1_NO_JUMP_INVERT=all IDO52_AS1_XBB_SKIP_CAND=0,1 IDO52_AS1_FTF_BACKWARD=occ:0 IDO52_AS1_NO_PASS1_PROP=8 IDO52_AS1_TIE_TAKE=4,22,44 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: patch_stack_complete --old 48 --new 56 */
/* @PostProc: remap_stack_slots --keep-frame --map '20:36,40:48,42:50,44:52,46:54,52:60,56:64,60:68,64:72,68:76,72:80' */
/* @PostProc: convert_or_to_addu */
/* @PostProc: swap_instructions --offset1 0x4 --offset2 0x14 */
/* @PostProc: swap_instructions --offset1 0x18 --offset2 0x20 */
/* @PostProc: swap_instructions --offset1 0x1c --offset2 0x24 */
#include "types.h"

extern void LoadImage(void *, s32);
extern void GetTPage(s32, s32, s32, s32);

void LoadTPage(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    s16 buf[4];

    buf[0] = arg3;
    buf[1] = arg4;
    buf[3] = arg6;
    goto dispatch;
case_0:
    buf[2] = arg5 / 4;
    goto done;
case_1:
    buf[2] = arg5 / 2;
    goto done;
case_2:
    buf[2] = arg5;
    goto done;
dispatch:
    if (arg1 == 0) goto case_0;
    if (arg1 == 1) goto case_1;
    if (arg1 == 2) goto case_2;
done:
    LoadImage(buf, arg0);
    GetTPage(arg1, arg2, arg3, arg4);
}
