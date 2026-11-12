/* @SymbolSize: 32 */
/* Function: date_call_thunk */
/* Overlay: DATE */
#include "types.h"

extern s32 date_ending_dispatch();

s32 date_call_thunk(s32 a0, s32 a1, s32 a2, s32 a3)
{
    s32 v0 = 0;

    v0 = date_ending_dispatch(a0, a1, a2, a3);
    return v0;
}
