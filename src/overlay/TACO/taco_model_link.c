/* @SymbolSize: 92 */
/* @PostProc: patch_stack_complete --old 32 --new 56 */
/* @PostProc: remap_stack_slots --from 28 --to 40 */
#include "types.h"

extern s32 GsMapModelingData(s32 a0);
extern s32 GsLinkObject4(s32 a0, s32 a1, s32 a2, s32 a3);

void taco_model_link(s32 a0, s32 a1, s32 a2)
{
    s32 t;

    t = a0 + 4;
    GsMapModelingData(t);
    GsLinkObject4(t + 8, a2 + 4, a1, t);
    *(s32 *)(a2 + 8) = a2 + 20;
    *(s32 *)(a2 + 4) = 0;
}
