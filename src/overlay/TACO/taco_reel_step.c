/* @CompileEnv: IDO52_PRECOLOR_WEB=v0:0 */
/* @PostProc: patch_insn_reg_field 0x10c:rt:t2:t3 0x114:rs:t2:t3 0x114:rt:t3:t4 0x118:rt:t3:t4 */
/* Address: 0x80152280, Size: 288 bytes (72 instructions) */
#include "types.h"

extern s32 taco_rows_disp(s32, s32);
extern s32 taco_reel_row_disp(s32, s32, s32);
extern void taco_reel_result_set(s32);

void taco_reel_step(u8 *P0, u8 *P1, s32 arg2, s32 arg3)
{
    u32 x;

    x = *P1;
    if (x < 30U) goto B8;
    if (x < 35U) goto FC;
B8:
    if (*P0 < 3U) goto L348;
    taco_reel_row_disp(arg2, arg3, x);
    *P1 = *P1 + 1;
    *P0 = 0;
    x = *P1;
    goto L348;
FC:
    if (x >= 35U) goto L344;
    if (x < 30U) goto L348;
    if (*P0 == 0) goto L348;
    taco_reel_row_disp(arg2, arg3, x);
    *P1 = *P1 + 1;
    *P0 = 0;
    x = *P1;
L344:
L348:
    if (x >= 45U) {
        *P1 = 0;
        *P0 = 0;
        taco_reel_result_set(arg2);
        taco_rows_disp(arg2, arg3);
    } else {
        *P0 = *P0 + 1;
    }
}
