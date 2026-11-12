/* @SymbolSize: 32 */
/* Function: date_basyo12_serifu_run */
/* Overlay: DATE */
#include "types.h"

extern s32 serifu_run();

s32 date_basyo12_serifu_run(s32 a0, s32 a1, s32 a2, s32 a3)
{
    s32 v0 = 0;

    v0 = serifu_run(a0, a1, a2, a3);
    return v0;
}
