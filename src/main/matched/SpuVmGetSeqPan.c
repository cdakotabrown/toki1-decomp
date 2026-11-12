/* Address: 0x8008E8F0 */
/* Size: 96 bytes (24 instructions) */
/* Region: Library (0x80086000+), IDO 5.2 */
/* @SymbolStart: 0x8008E8F0 */
/* @SymbolSize: 96 */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all */
/* @PostProc: strip_leading_instructions --count auto */
/* @Compiler: ido52-gas */
/* @CompileFlags: --ido-copy-and-bump --ido-fill-load-delay --ido-fill-macro-lui */
#include "types.h"

typedef struct {
    s16 h0;
    s16 h2;
    s16 h4;
    s16 h6;
} Ent8;

extern Ent8 libsnd_seq_vol_tbl_r_view[];
extern s16  D_80131606;

s32 SpuVmGetSeqPan(s16 id, s16 *out) {
    D_80131606 = (s16)(id & 0xFF);
    if (D_80131606 < 34) {
        *out = libsnd_seq_vol_tbl_r_view[D_80131606].h2;
        return D_80131606;
    }
    return -1;
}
