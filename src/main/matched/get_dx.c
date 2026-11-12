/* Address: 0x800A1414 */
/* Size: 216 bytes (54 instructions) */
/* @SymbolStart: 0x800A1414 */
/* @SymbolSize: 216 */
/* Region: Library (0x80086000+), IDO 5.2 codegen, ASPSX-assembled */
/* trailing `return v`).  Re-checked today: `@Compiler: ido52-aspsx` (maspsx) */
/* @Compiler: ido52-gas */
/* @CompileFlags: --ido-fill-load-delay --ido-hoist-load-above --ido-copy-and-bump --ido-div-macro-residue --ido-label-load-nop */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all IDO52_UGEN_FORCE_TARGET_SAFE_DIV=1 */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

extern u8 graph_env;
extern u8 libgpu_graph_reverse_flag;

s32 get_dx(RECT *r) {
    s32 v;

    switch (graph_env) {
    case 1:
        if (libgpu_graph_reverse_flag != 0) {
            v = -r->x - r->w + 1024;
        } else {
            v = r->x;
        }
        return v;
    case 2:
        if (libgpu_graph_reverse_flag != 0) {
            v = -r->x - r->w / 2 + 1024;
        } else {
            v = r->x / 2;
        }
        return v;
    }
    return r->x;
}
