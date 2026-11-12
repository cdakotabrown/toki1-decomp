/* Function: GetODE */
/* Address: 0x800A0B9C   (the recorded symbol 0x800A0BA4 is the PROLOGUE; IDO */
/* Size: 76 bytes (19 instructions) */
/* Region: Library (0x80086000+) */
/* @SymbolStart: 0x800A0B9C */
/* @SymbolSize: 76 */
/* @CompileEnv: IDO52_AS1_XBB_SKIP_CAND=1 IDO52_AS1_NO_BRANCH_FOLD=all IDO52_UGEN_DEST_TEMP=2 */
/* @PostProc: strip_leading_instructions --count auto */
/* @PostProc: convert_or_to_addu */
#include "types.h"

typedef struct {
    u8 pad[56];
    s32 (*func)(void);
} ContRpnCtx;

extern ContRpnCtx *graph_jtb;

s32 GetODE(void)
{
    s32 result;
    s32 ret;

    result = graph_jtb->func();
    if (result & 0x80000000) {
        ret = 1;
    } else {
        ret = 0;
    }
    return ret;
}
