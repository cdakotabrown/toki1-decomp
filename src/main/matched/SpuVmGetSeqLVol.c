/* Address: 0x8008E850 */
/* Size: 80 bytes (20 instructions) */
/* Region: Library (0x80086000+) */
/* @SymbolStart: 0x8008E850 */
/* @SymbolSize: 80 */
/* @Compiler: ido52-gas */
/* @CompileEnv: IDO52_UGEN_FORCE_TARGET_SAFE=all */
/* @PostProc: strip_leading_instructions --count auto */
#include "types.h"

typedef struct {
    s16 id;
    s16 w;
    s16 h;
    s16 pad;
} EntS;

extern EntS libsnd_seq_vol_tbl[];
extern s16  D_80131606;

s32 SpuVmGetSeqLVol(s16 id) {
    D_80131606 = id & 0xFF;
    if (D_80131606 < 34) {
        return libsnd_seq_vol_tbl[D_80131606].w;
    }
    return -1;
}
