/* Address: 0x8008E8A0 (recorded symbol 0x8008E8B4 is mid-function: the */
/* Size: 80 bytes (20 instructions) */
/* Region: Library (0x80086000+) */
/* @SymbolStart: 0x8008E8A0 */
/* @SymbolSize: 80 */
/* @Compiler: ido52-gas */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    s16 h0;
    s16 h2;
    s16 h4;
    s16 h6;
} Ent8;

extern Ent8 libsnd_seq_vol_tbl_r_view[];
extern s16  D_80131606;

s32 SpuVmGetSeqRVol(s16 id) {
    D_80131606 = id & 0xFF;
    if (D_80131606 < 34) {
        return libsnd_seq_vol_tbl_r_view[D_80131606].h0;
    }
    return -1;
}
